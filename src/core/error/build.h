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
 * Created: May. 28, 2024
 * Supported by: National Key Research and Development Program of China
 */

#pragma once

#include <antlr4-runtime.h>
#include <exception>
#include <string>

class BuildException : public std::exception {
  private:
    std::string message;

  public:
    BuildException(const std::string &msg, antlr4::Token *token) {
        message = "line " + std::to_string(token->getLine()) + ", column " +
                  std::to_string(token->getCharPositionInLine()) + ": " + msg;
    }
    const char *what() const noexcept override { return message.c_str(); }
};
