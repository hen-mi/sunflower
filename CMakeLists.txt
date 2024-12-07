cmake_minimum_required(VERSION "3.20.0")

project(sunflower)

#find_package(LLVM REQUIRED CONFIG)

#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(SOURCE_FILES 
                src/main.cpp 
                src/Lexer.cpp 
                src/Parser.cpp
                src/Sunflower.cpp
                src/Evaluator.cpp
                #src/CodeGen.cpp
                src/Environment.cpp
                src/Callable.cpp
                #src/Resolver.cpp
)

#include_directories(${LLVM_INCLUDE_DIRS})
#separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
#add_definitions(${LLVM_DEFINITIONS_LIST})

add_executable(sunflowerLang ${SOURCE_FILES})
if(MSVC)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

target_include_directories(sunflowerLang PUBLIC src)

add_custom_command(TARGET sunflowerLang POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/examples" "${CMAKE_BINARY_DIR}/examples"
)

#llvm_map_components_to_libnames(llvm_libs support core irreader)
