cmake_minimum_required(VERSION 3.13)

project(main CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE RELEASE)

add_compile_options(
                -Wall
                -Wextra
                -fstack-protector-all
#                -fsanitize=thread
#                -fsanitize=address
#                -g 
                -O3
        )

add_link_options(
#                -fsanitize=thread
#                -fsanitize=address
        )

add_executable(
        ${PROJECT_NAME}
        main.cpp
        )
