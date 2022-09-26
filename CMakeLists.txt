cmake_minimum_required(VERSION 3.0.0)
project(Stonks VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Stonks stonks.cpp config.h avParser.h)

find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

# add_compile_options(-std=c++14 -stdlib=libc++ -lboost_system -lssl -lcrypto -ldl -lpthread)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} boost_system pthread ssl crypto)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
