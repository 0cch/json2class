cmake_minimum_required(VERSION 3.15)
project(json2class VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4 /WX /arch:AVX2 /utf-8)
    add_compile_options(/Zc:preprocessor)
else()
    add_compile_options(-Wall -Wextra -Werror)
    if(NOT APPLE)
        add_compile_options(-mavx2)
    endif()
endif()

find_package(nlohmann_json REQUIRED)

add_executable(json2class 
    main.cpp
    json_class_generator.cpp
)
target_link_libraries(json2class PRIVATE nlohmann_json::nlohmann_json)

add_executable(test_json2class person_example.cpp)
target_link_libraries(test_json2class PRIVATE nlohmann_json::nlohmann_json)