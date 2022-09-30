cmake_minimum_required(VERSION 3.0.0)
project(Stonks VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

file(GLOB src_files CONFIGURE_DEPENDS
     "Xcurse/src/*.cpp"
     "Xcurse/src/Common/*.cpp"
     "Xcurse/src/Display/*.cpp"
     "Xcurse/src/Utils/*.cpp"
     "Xcurse/src/Widgets/*.cpp"
     "Xcurse/src/Window/*.cpp"
     "src/*.cpp"
     "src/*.h"
)

add_executable(Stonks ${src_files})

find_package(OpenSSL REQUIRED)
if (OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

# add_compile_options(-std=c++14 -stdlib=libc++ -lboost_system -lssl -lcrypto -ldl -lpthread)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} boost_system pthread ssl crypto)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
