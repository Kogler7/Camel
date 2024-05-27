/**
 * Copyright (c) 2022 Beijing Jiaotong University
 * PhotLab is licensed under [Open Source License].
 * You can use this software according to the terms and conditions of the [Open
 * Source License]. You may obtain a copy of [Open Source License] at:
 * [https://open.source.license/]
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY
 * KIND, EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * NON-INFRINGEMENT, MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the [Open Source License] for more details.
 *
 * Author: Zhenjie Wei
 * Created: May. 5, 2024
 * Supported by: National Key Research and Development Program of China
 */

#include "value.h"
#include "entity.h"

bool SetValue::resolved() {
    if (resolved_) {
        return true;
    }
    for (const auto &e : data_) {
        if (!e->resolved()) {
            return false;
        }
    }
    resolved_ = true;
    return true;
}

const value_ptr_t SetValue::convert(type_ptr_t target, bool inplace) {
    if (target == type_ || type_->equals(target)) {
        // same type, no need to convert
        return shared_from_this();
    }
    if (target->structured()) {
        switch (target->code()) {
        case TypeCode::SET:
            /* code */
            break;

        default:
            break;
        }
    }
    throw ValueConvError("Cannot convert " + type_->toString() + " to " + typeCodeToString(target->code()));
}

const value_ptr_t SetValue::clone(bool deep) const {
    std::set<entity_ptr_t> cloned;
    for (const auto &e : data_) {
        cloned.insert(e);
    }
    return std::make_shared<SetValue>(type_, cloned);
}

const std::string SetValue::toString() const {
    std::string str = "{";
    for (const auto &e : data_) {
        str += e->dataStr() + ", ";
    }
    str += "}";
    return str;
}

bool ListValue::resolved() {
    if (resolved_) {
        return true;
    }
    for (const auto &e : data_) {
        if (!e->resolved()) {
            return false;
        }
    }
    resolved_ = true;
    return true;
}

const value_ptr_t ListValue::convert(type_ptr_t target, bool inplace) {
    // TODO
    if (target == type_ || type_->equals(target)) {
        // same type, no need to convert
        return shared_from_this();
    }
    if (target->structured()) {
        switch (target->code()) {
        case TypeCode::SET:
            /* code */
            break;

        default:
            break;
        }
    }
    throw ValueConvError("Cannot convert " + type_->toString() + " to " + typeCodeToString(target->code()));
}

const value_ptr_t ListValue::clone(bool) const {
    std::vector<entity_ptr_t> cloned;
    for (const auto &e : data_) {
        cloned.push_back(e);
    }
    return std::make_shared<ListValue>(cloned);
}

const std::string ListValue::toString() const {
    std::string str = "[";
    for (const auto &e : data_) {
        str += e->dataStr() + ", ";
    }
    str += "]";
    return str;
}