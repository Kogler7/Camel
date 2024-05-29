
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
 * Supported by: National Key Research and Development Program of China
 */


// Generated from OpenCML.g4 by ANTLR 4.13.1

#pragma once


#include "antlr4-runtime.h"




class  OpenCMLLexer : public antlr4::Lexer {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, T__23 = 24, T__24 = 25, T__25 = 26, 
    T__26 = 27, T__27 = 28, T__28 = 29, T__29 = 30, T__30 = 31, T__31 = 32, 
    T__32 = 33, T__33 = 34, T__34 = 35, T__35 = 36, T__36 = 37, T__37 = 38, 
    T__38 = 39, T__39 = 40, T__40 = 41, SEP = 42, AS = 43, IS = 44, LET = 45, 
    USE = 46, FUNC = 47, TYPE = 48, WITH = 49, RETURN = 50, INNER = 51, 
    OUTER = 52, ATOMIC = 53, STATIC = 54, SYNC = 55, NULL_ = 56, TRUE = 57, 
    FALSE = 58, INTEGER_TYPE = 59, INTEGER32_TYPE = 60, INTEGER64_TYPE = 61, 
    REAL_TYPE = 62, FLOAT_TYPE = 63, DOUBLE_TYPE = 64, NUMBER_TYPE = 65, 
    STRING_TYPE = 66, BOOL_TYPE = 67, CHAR_TYPE = 68, SET_TYPE = 69, MAP_TYPE = 70, 
    LIST_TYPE = 71, DICT_TYPE = 72, ARRAY_TYPE = 73, TUPLE_TYPE = 74, UNION_TYPE = 75, 
    VECTOR_TYPE = 76, TENSOR_TYPE = 77, ANY_TYPE = 78, VOID_TYPE = 79, FUNCTOR_TYPE = 80, 
    BLANK = 81, LIN_CMT_HASH = 82, LIN_CMT_SLASH = 83, BLK_CMT = 84, MULTI_STR = 85, 
    IDENTIFIER = 86, UNIT = 87, STRING = 88, FSTRING = 89, INTEGER = 90, 
    REAL = 91
  };

  explicit OpenCMLLexer(antlr4::CharStream *input);

  ~OpenCMLLexer() override;


  std::string getGrammarFileName() const override;

  const std::vector<std::string>& getRuleNames() const override;

  const std::vector<std::string>& getChannelNames() const override;

  const std::vector<std::string>& getModeNames() const override;

  const antlr4::dfa::Vocabulary& getVocabulary() const override;

  antlr4::atn::SerializedATNView getSerializedATN() const override;

  const antlr4::atn::ATN& getATN() const override;

  // By default the static state used to implement the lexer is lazily initialized during the first
  // call to the constructor. You can call this function if you wish to initialize the static state
  // ahead of time.
  static void initialize();

private:

  // Individual action functions triggered by action() above.

  // Individual semantic predicate functions triggered by sempred() above.

};

