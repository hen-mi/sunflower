cmake_minimum_required(VERSION "3.20.0")

project(sunflower)

set(SOURCE_FILES 
                src/main.cpp 
                src/Lexer.cpp 
                src/Parser.cpp
                src/Sunflower.cpp
                src/Evaluator.cpp
)

add_executable(sunflowerLang ${SOURCE_FILES})
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

target_include_directories(sunflowerLang PUBLIC src)

add_custom_command(TARGET sunflowerLang POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/examples" "${CMAKE_BINARY_DIR}/examples"
)