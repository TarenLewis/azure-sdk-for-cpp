# Copyright (c) Microsoft Corporation. All rights reserved.
# SPDX-License-Identifier: MIT

cmake_minimum_required (VERSION 3.13)

set(TARGET_NAME "azure-core-test")

# Create test data for FileUpload test (100K) by writing 1K * 100 times
set(RANGE 0)
set(1K "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX")
file(WRITE ${CMAKE_BINARY_DIR}/fileData "")
while(RANGE LESS 100)
     file(APPEND ${CMAKE_BINARY_DIR}/fileData "${1K}")
     MATH(EXPR RANGE "${RANGE}+1")
endwhile()
add_compile_definitions(AZURE_TEST_DATA_PATH="${CMAKE_BINARY_DIR}")

project (${TARGET_NAME} LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(BUILD_TRANSPORT_CURL)
  SET(CURL_OPTIONS_TESTS curl_options.cpp)
  SET(CURL_TRANSPORT_TESTS transport_adapter_curl.cpp)
endif()

include(GoogleTest)

# This symbol helps to disable tests by adding a pre-processor condition `#if !defined(TEST_DISABLED)`
# The symbol is define to disable any test surrounded with this.
add_compile_definitions(TEST_DISABLED)

add_executable (
     ${TARGET_NAME}
     context.cpp
     ${CURL_OPTIONS_TESTS}
     curl_session_test.cpp
     datetime.cpp
     http.cpp
     json.cpp
     logging.cpp
     main.cpp
     nullable.cpp
     pipeline.cpp
     policy.cpp
     simplified_header.cpp
     string.cpp
     telemetry_policy.cpp
     transport_adapter_base.cpp
     ${CURL_TRANSPORT_TESTS}
     url.cpp
     uuid.cpp
     )

# Adding private headers from CORE to the tests so we can test the private APIs with no relative paths include.
target_include_directories (${TARGET_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../src>)

target_link_libraries(${TARGET_NAME} PRIVATE azure-core gtest gmock)

# gtest_add_tests will scan the test from azure-core-test and call add_test
# for each test to ctest. This enables `ctest -r` to run specific tests directly.
gtest_add_tests(TARGET ${TARGET_NAME}
     TEST_PREFIX azure-core.)

