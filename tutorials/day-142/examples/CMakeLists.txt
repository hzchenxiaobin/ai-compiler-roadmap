# Day 142：JSON 解析器的 CMakeLists（计划指定：多平台构建）
cmake_minimum_required(VERSION 3.16)
project(json_parser VERSION 1.0.0 LANGUAGES CXX)

# C++20 + 警告（多编译器通用——MSVC/GCC/Clang 各认各的）
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra)
endif()

# ===== 可执行文件：解析器演示 =====
add_executable(json_demo json_demo.cpp)

# ===== 测试（GoogleTest——find_package 的正规集成） =====
option(JSON_BUILD_TESTS "Build tests" ON)
if (JSON_BUILD_TESTS)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    add_executable(json_tests json_tests.cpp)
    target_link_libraries(json_tests PRIVATE GTest::gtest GTest::gtest_main)
    include(GoogleTest)
    gtest_discover_tests(json_tests)
endif()

# ===== 检查目标（sanitize——一键消毒柜） =====
add_executable(json_sanitize json_sanitize.cpp)
target_compile_options(json_sanitize PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/fsanitize=address>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined -fno-sanitize-recover=all>
)
target_link_options(json_sanitize PRIVATE
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-fsanitize=address,undefined>
)
