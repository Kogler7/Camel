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
 * Created: Apr. 9, 2024
 * Supported by: National Key Research and Development Program of China
 */

#pragma once

#include <any>
#include <memory>
#include <set>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

enum class TypeCode {
    // primitive types
    INT32 = 0b00'000000,
    INT64 = 0b00'000001,
    FLOAT = 0b00'000010,
    DOUBLE = 0b00'000011,
    STRING = 0b00'000100,
    BOOL = 0b00'000101,
    CHAR = 0b00'000110,
    // structured types
    SET = 0b01'000000,
    MAP = 0b01'000001,
    ARRAY = 0b01'000010,
    LIST = 0b01'000011,
    DICT = 0b01'000100,
    UNION = 0b01'000101,
    VECTOR = 0b01'000110,
    MATRIX = 0b01'000111,
    // special types
    ANY = 0b10'000000,
    VOID = 0b10'000001,
    FUNCTOR = 0b10'000010,
};

enum class TypeConv {
    SAFE = 1,
    UNSAFE = -1,
    FORBIDDEN = 0,
};

std::string typeCodeToString(TypeCode code);

extern const signed char primeTypeConvMatrix[7][7];

class Type;
class PrimeType;
class StructType;
class SpecialType;
class SetType;
class MapType;
class ArrayType;
class ListType;
class DictType;
class UnionType;
class VectorType;
class MatrixType;

using type_ptr_t = std::shared_ptr<const Type>;

class Type {
  protected:
    TypeCode code_;

  public:
    Type() = delete;
    Type(TypeCode type) : code_(type) {}
    virtual ~Type() = default;

    const TypeCode &code() const { return code_; }

    bool primitive() const { return (static_cast<int>(code_) & 0b11'000000) == 0b00'000000; }
    bool structured() const { return (static_cast<int>(code_) & 0b11'000000) == 0b01'000000; }
    bool special() const { return (static_cast<int>(code_) & 0b11'000000) == 0b10'000000; }

    virtual std::string toString() const { return typeCodeToString(code_); }

    virtual bool operator==(const Type &other) const { return code_ == other.code_; }
    virtual bool operator!=(const Type &other) const { return code_ != other.code_; }

    bool equals(const type_ptr_t &type) const { return *type == *this; }

    virtual TypeConv convertibility(const Type &other) const {
        if (code_ == other.code_) {
            return TypeConv::SAFE;
        }
        return TypeConv::FORBIDDEN;
    }
};

class PrimeType : public Type {
  public:
    PrimeType() = delete;
    PrimeType(TypeCode code) : Type(code) {}

    TypeConv convertibility(const Type &other) const override;
};

class StructType : public Type {
  public:
    StructType() = delete;
    StructType(TypeCode code) : Type(code) {}

    virtual std::string toString() const override = 0;

    virtual bool operator==(const Type &other) const override = 0;
    virtual bool operator!=(const Type &other) const override = 0;

    virtual TypeConv convertibility(const Type &other) const override = 0;
};

class SpecialType : public Type {
  public:
    SpecialType() = delete;
    SpecialType(TypeCode code) : Type(code) {}

    TypeConv convertibility(const Type &other) const override;
};

class SetType : public StructType {
  private:
    type_ptr_t valueType_;

  public:
    SetType() = delete;
    SetType(const type_ptr_t &valueType) : StructType(TypeCode::SET), valueType_(valueType) {}

    type_ptr_t valueType() const { return valueType_; }

    std::string toString() const override { return "set<" + valueType_->toString() + ">"; }

    bool operator==(const Type &other) const override {
        if (other.code() != TypeCode::SET) {
            return false;
        }
        const SetType &otherMap = dynamic_cast<const SetType &>(other);

        return valueType_->equals(otherMap.valueType_);
    }
    bool operator!=(const Type &other) const override {
        if (other.code() != TypeCode::SET) {
            return true;
        }
        const SetType &otherMap = dynamic_cast<const SetType &>(other);

        return !valueType_->equals(otherMap.valueType_);
    }

    TypeConv convertibility(const Type &other) const override;
};

class MapType : public StructType {
  private:
    type_ptr_t keyType_;
    type_ptr_t valueType_;

  public:
    MapType() = delete;
    MapType(const type_ptr_t &keyType, const type_ptr_t &valueType)
        : StructType(TypeCode::MAP), keyType_(keyType), valueType_(valueType) {}

    type_ptr_t keyType() const { return keyType_; }
    type_ptr_t valueType() const { return valueType_; }

    std::string toString() const override {
        return "map<" + keyType_->toString() + ", " + valueType_->toString() + ">";
    }

    bool operator==(const Type &other) const override {
        if (other.code() != TypeCode::MAP) {
            return false;
        }
        const MapType &otherMap = dynamic_cast<const MapType &>(other);

        return keyType_->equals(otherMap.keyType_) && valueType_->equals(otherMap.valueType_);
    }
    bool operator!=(const Type &other) const override {
        if (other.code() != TypeCode::MAP) {
            return true;
        }
        const MapType &otherMap = dynamic_cast<const MapType &>(other);

        return !keyType_->equals(otherMap.keyType_) || !valueType_->equals(otherMap.valueType_);
    }

    TypeConv convertibility(const Type &other) const override;
};

class ArrayType : public StructType {
  private:
    type_ptr_t elementType_;
    size_t size_;

  public:
    ArrayType() = delete;
    ArrayType(const type_ptr_t &elementType, size_t size)
        : StructType(TypeCode::ARRAY), elementType_(elementType), size_(size) {}

    size_t size() const { return size_; }
    type_ptr_t elementType() const { return elementType_; }

    std::string toString() const override {
        return "array<" + elementType_->toString() + ", " + std::to_string(size_) + ">";
    }

    bool operator==(const Type &other) const override {
        if (other.code() != TypeCode::ARRAY) {
            return false;
        }
        const ArrayType &otherArray = dynamic_cast<const ArrayType &>(other);
        return size_ == otherArray.size_ && elementType_->equals(otherArray.elementType_);
    }
    bool operator!=(const Type &other) const override {
        if (other.code() != TypeCode::ARRAY) {
            return true;
        }
        const ArrayType &otherArray = dynamic_cast<const ArrayType &>(other);
        return size_ != otherArray.size_ || !elementType_->equals(otherArray.elementType_);
    }

    TypeConv convertibility(const Type &other) const override;
};

class DictType : public StructType {
  private:
    // field name -> field type with default value
    std::unordered_map<std::string, type_ptr_t> fields_;

  public:
    DictType() : StructType(TypeCode::DICT) {}

    std::string toString() const override {
        std::string result = "{";
        for (const auto &field : fields_) {
            result += field.first + ": " + field.second->toString() + ", ";
        }
        result.pop_back();
        result.pop_back();
        result += "}";
        return result;
    }

    bool operator==(const Type &other) const override {
        if (other.code() != TypeCode::DICT) {
            return false;
        }
        const DictType &otherDict = dynamic_cast<const DictType &>(other);

        if (fields_.size() != otherDict.fields_.size()) {
            return false;
        }
        for (const auto &field : otherDict.fields_) {
            const auto &ident = field.first;
            const auto &type = field.second;
            if (!fields_.count(ident)) {
                return false;
            }
            const auto &fieldType = fields_.at(ident);
            if (fieldType->code() != type->code()) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const Type &other) const override { return !(*this == other); }

    bool add(const std::string &name, const type_ptr_t &type) {
        if (has(name)) {
            return false;
        }
        fields_[name] = type;
        return true;
    }

    bool del(const std::string &name) { return fields_.erase(name) > 0; }

    bool has(const std::string &name) const { return fields_.find(name) != fields_.end(); }

    void set(const std::string &name, const type_ptr_t &type) { fields_.at(name) = type; }

    type_ptr_t get(const std::string &name) const { return fields_.at(name); }

    type_ptr_t operator|(const DictType &other) const {
        auto result = std::make_shared<DictType>();
        for (const auto &field : fields_) {
            result->add(field.first, field.second);
        }
        for (const auto &field : other.fields_) {
            const auto &ident = field.first;
            const auto &type = field.second;
            if (!result->has(ident)) {
                result->add(ident, type);
            } else {
                // if the field already exists, use the rhs type and value
                result->set(ident, type);
            }
        }
        return result;
    }

    type_ptr_t operator&(const DictType &other) const {
        auto result = std::make_shared<DictType>();
        for (const auto &field : fields_) {
            const auto &ident = field.first;
            if (other.has(ident)) {
                const type_ptr_t &otherType = other.get(ident);
                result->add(ident, otherType);
            }
        }
        return result;
    }

    TypeConv convertibility(const Type &other) const override;
};

class UnionType : public StructType {
  private:
    std::set<type_ptr_t> types_;

    void insertUnion(const UnionType &other) {
        // flatten the union type
        for (const auto &type : other.types_) {
            if (type->code() == TypeCode::UNION)
                insertUnion(dynamic_cast<const UnionType &>(*type));
            else
                types_.insert(type);
        }
    }

  public:
    UnionType() : StructType(TypeCode::UNION) {}
    UnionType(const std::initializer_list<type_ptr_t> &types) : StructType(TypeCode::UNION) {
        for (const auto &type : types) {
            if (type->code() == TypeCode::UNION)
                insertUnion(dynamic_cast<const UnionType &>(*type));
            else
                types_.insert(type);
        }
    }
    UnionType(const std::vector<type_ptr_t> &types) : StructType(TypeCode::UNION) {
        for (const auto &type : types) {
            if (type->code() == TypeCode::UNION)
                insertUnion(dynamic_cast<const UnionType &>(*type));
            else
                types_.insert(type);
        }
    }

    std::string toString() const override {
        std::string result = "";
        for (const auto &type : types_) {
            result += type->toString() + " | ";
        }
        result.pop_back();
        result.pop_back();
        result.pop_back();
        return result;
    }

    bool operator==(const Type &other) const override {
        if (other.code() != TypeCode::UNION) {
            return false;
        }
        const UnionType &otherUnion = dynamic_cast<const UnionType &>(other);

        if (types_.size() != otherUnion.types_.size()) {
            return false;
        }
        for (const auto &type : otherUnion.types_) {
            if (types_.find(type) == types_.end()) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const Type &other) const override { return !(*this == other); }

    void add(const type_ptr_t &type) { types_.insert(type); }
    bool has(const type_ptr_t &type) const { return types_.find(type) != types_.end(); }

    TypeConv convertibility(const Type &other) const override;
};

class VectorType : public StructType {
  private:
    type_ptr_t elementType_;
    size_t size_;

  public:
    VectorType(const type_ptr_t &elementType, size_t size)
        : StructType(TypeCode::VECTOR), elementType_(elementType), size_(size) {
        // element type must be a primitive type
        if (!elementType->primitive()) {
            throw std::invalid_argument("Vector element type must be primitive");
        }
    }

    size_t size() const { return size_; }
    type_ptr_t elementType() const { return elementType_; }

    std::string toString() const override {
        return "vector<" + elementType_->toString() + ", " + std::to_string(size_) + ">";
    }

    bool operator==(const Type &other) const override {
        if (other.code() != TypeCode::VECTOR) {
            return false;
        }
        const VectorType &otherVector = dynamic_cast<const VectorType &>(other);
        return size_ == otherVector.size_ && elementType_->equals(otherVector.elementType_);
    }
    bool operator!=(const Type &other) const override {
        if (other.code() != TypeCode::VECTOR) {
            return true;
        }
        const VectorType &otherVector = dynamic_cast<const VectorType &>(other);
        return size_ != otherVector.size_ || !elementType_->equals(otherVector.elementType_);
    }

    TypeConv convertibility(const Type &other) const override;
};

class MatrixType : public StructType {
  private:
    type_ptr_t elementType_;
    std::vector<size_t> shape_;

  public:
    MatrixType(const type_ptr_t &elementType, const std::vector<size_t> &shape)
        : StructType(TypeCode::MATRIX), elementType_(elementType), shape_(shape) {
        if (shape_.size() == 0) {
            throw std::invalid_argument("Matrix shape must at least have 1 dim");
        }
        // element type must be a primitive type
        if (!elementType->primitive()) {
            throw std::invalid_argument("Matrix element type must be primitive");
        }
    }

    std::vector<size_t> shape() const { return shape_; }
    type_ptr_t elementType() const { return elementType_; }

    std::string toString() const override {
        std::string result = "matrix<" + elementType_->toString() + ", [";
        for (const auto &dim : shape_) {
            result += std::to_string(dim) + ", ";
        }
        result.pop_back();
        result.pop_back();
        result += "]>";
        return result;
    }

    bool operator==(const Type &other) const override {
        if (other.code() != TypeCode::MATRIX) {
            return false;
        }
        const MatrixType &otherMatrix = dynamic_cast<const MatrixType &>(other);
        return shape_ == otherMatrix.shape_ && elementType_->equals(otherMatrix.elementType_);
    }
    bool operator!=(const Type &other) const override {
        if (other.code() != TypeCode::MATRIX) {
            return true;
        }
        const MatrixType &otherMatrix = dynamic_cast<const MatrixType &>(other);
        return shape_ != otherMatrix.shape_ || !elementType_->equals(otherMatrix.elementType_);
    }

    TypeConv convertibility(const Type &other) const override;
};

class ListType : public StructType {
  public:
    ListType() : StructType(TypeCode::LIST) {}

    std::string toString() const override { return "list"; }

    bool operator==(const Type &other) const override { return true; }
    bool operator!=(const Type &other) const override { return false; }

    TypeConv convertibility(const Type &other) const override;
};

extern type_ptr_t int32TypePtr;
extern type_ptr_t int64TypePtr;
extern type_ptr_t floatTypePtr;
extern type_ptr_t doubleTypePtr;
extern type_ptr_t stringTypePtr;
extern type_ptr_t boolTypePtr;
extern type_ptr_t charTypePtr;

extern type_ptr_t intTypePtr;
extern type_ptr_t realTypePtr;
extern type_ptr_t numberTypePtr;

extern type_ptr_t anyTypePtr;
extern type_ptr_t voidTypePtr;
extern type_ptr_t functorTypePtr;

void initTypes();