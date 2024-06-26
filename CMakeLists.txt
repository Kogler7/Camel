cmake_minimum_required(VERSION 3.25)
project(camel)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_MACOSX_RPATH OFF)

if(MSVC)
	message(STATUS "Using MSVC")
	set(CMAKE_CXX_FLAGS /source-charset:utf-8)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /O2 /Ot") #
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
	# 4003:宏定义参数不足 4005:宏重定义 4819:文件名中有非ASCII字符
    add_compile_options(/wd4003 /wd4005 /wd4819 /O2) #
else()
	message(STATUS "Using GCC or Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

file(GLOB_RECURSE SRC_FILES
	"${PROJECT_SOURCE_DIR}/src/*.cpp"
	"${PROJECT_SOURCE_DIR}/src/*.h"
	"${PROJECT_SOURCE_DIR}/test/*.cpp"
	"${PROJECT_SOURCE_DIR}/test/*.h"
)

find_package(antlr4-runtime CONFIG REQUIRED)
find_package(clipp REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(
	${PROJECT_NAME} PUBLIC
	lib/
	src/
)

target_link_libraries(${PROJECT_NAME} PRIVATE clipp::clipp)

if(TARGET antlr4_shared)
    target_link_libraries(${PROJECT_NAME} PRIVATE antlr4_shared)
else()
    target_link_libraries(${PROJECT_NAME} PRIVATE antlr4_static)
endif()