cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME elem)
project(${PROJECT_NAME} LANGUAGES CXX)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_CURRENT_SOURCE_DIR}")
    set(ELEM_STANDALONE TRUE)
endif()

# Options
option(BUILD_SHARED_LIBS "Build as shared/dynamic library" ON)
option(ELEM_BUILD_EXAMPLES "Build the example programs" ${ELEM_STANDALONE})
option(DYNAMIC_RUNTIME_LIBRARY "Set runtime library to dynamic" ${BUILD_SHARED_LIBS})

# Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINE_HIDDEN YES)


# Enable folder view in solution explorer
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE SOURCE_FILES
    include/${PROJECT_NAME}/*.h
    src/*.h
    src/*.cpp
)

# Microsoft Runtime Library shenanigens
if(NOT DYNAMIC_RUNTIME_LIBRARY) 
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if (BUILD_SHARED_LIBS)
    add_compile_definitions(ELEM_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
else()
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES 
    SOVERSION 0
    VERSION 0.1.1
)

target_include_directories(${PROJECT_NAME} PUBLIC include)



# ---------------- DEPENDENCIES ---------------------#

# elemd
find_package(elemd CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE elemd::elemd)


# ---------------- EXAMPLES ---------------------#

if(APPLE)
    set(CONVERTER_POSTFIX "mac.sh")
    set(SHARED_LIB_NAME "lib${PROJECT_NAME}.dylib")
elseif(WIN32) 
    set(CONVERTER_POSTFIX "win.bat")
    set(SHARED_LIB_NAME "${PROJECT_NAME}.dll")
else()
    set(CONVERTER_POSTFIX "linux.sh")
    set(SHARED_LIB_NAME "lib${PROJECT_NAME}.so")
endif()

if (ELEM_BUILD_EXAMPLES)
    add_subdirectory(examples/calculator)
    add_subdirectory(examples/elemx)
    add_subdirectory(examples/tasker)
    add_subdirectory(examples/slack)

    set_target_properties(calculator PROPERTIES FOLDER examples)
    set_target_properties(elemx PROPERTIES FOLDER examples)
    set_target_properties(tasker PROPERTIES FOLDER examples)
    set_target_properties(slack PROPERTIES FOLDER examples)

    if (BUILD_SHARED_LIBS)
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            DEPENDS calculator parser
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/calculator/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/elemx/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/tasker/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME}
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} examples/slack/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>/${SHARED_LIB_NAME} 
            COMMENT "Copy library"
        )
    endif()
endif()