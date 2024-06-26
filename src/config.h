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
 * Created: Mar. 17, 2024
 * Supported by: National Key Research and Development Program of China
 */

#define CAMEL_VERSION "0.1.0"

#include <string>

// enum class mode {run, dump, help};

extern std::string targetFile;
extern std::string outputFile;

extern std::string errorFormat;

extern std::vector<std::string> includeDirs;
extern std::vector<std::string> scriptsDirs;

extern bool profile;
extern bool verbose;
extern bool noCache;

extern bool syntaxOnly;
extern bool semanticOnly;

extern bool dumpCST, dumpAST, dumpGIR, dumpONNX, dumpTokens, format;

extern unsigned int repeat;

extern unsigned int maxThreads;
extern unsigned int serverPort;

bool parseArgs(int argc, char *argv[]);