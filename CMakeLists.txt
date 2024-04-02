cmake_minimum_required(VERSION 3.10)

project(${LOCAL_PROJECT_NAME}
        VERSION ${LOCAL_PROJECT_VERSION}
        LANGUAGES C CXX)


set(LOCAL_PROJECT_NAME "Sculptor")
set(LOCAL_PROJECT_VERSION "0.0.1")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CIMG_NEED_X11 0)
set(CIMG_NEED_PNG 1)
set(CIMG_NEED_JPG 1)

set(CIMG_H_PATH "${PROJECT_SOURCE_DIR}/Libraries/CImg")
set(SOIL_H_PATH "${PROJECT_SOURCE_DIR}/Libraries/SOIL/includes/SOIL")


file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list "${PROJECT_SOURCE_DIR}/src/*.hpp")
    SET(dir_list "")
    FOREACH (file_path ${new_list})
        # GET_FILENAME_COMPONENT(file_name ${file_path} NAME)
        # message(STATUS "[INFO] Found ${file_name} header.")
        # message(STATUS "${file_name}")
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH ()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")

add_executable(${LOCAL_PROJECT_NAME} ${SOURCES})
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${header_dir_list})


# ---OpenMP---
find_package(OpenMP REQUIRED)
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)


# ---CImg---
add_library(CImg_Lib INTERFACE)
target_include_directories(CImg_Lib INTERFACE ${CIMG_H_PATH})
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE CImg_Lib)

if(CIMG_NEED_X11)
    message(STATUS "Looking for X11...")
    find_package(X11 REQUIRED)
    target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
else()
    target_compile_definitions(${LOCAL_PROJECT_NAME} PRIVATE cimg_display=0)
endif()

if(CIMG_NEED_JPG)
    message(STATUS "Looking for libjpeg...")
    find_package(JPEG REQUIRED)
    target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${JPEG_INCLUDE_DIR})
    target_link_libraries (${LOCAL_PROJECT_NAME} PRIVATE ${JPEG_LIBRARY})
    target_compile_definitions(${LOCAL_PROJECT_NAME} PRIVATE cimg_use_jpeg=1)
endif()

if(CIMG_NEED_PNG)
    message(STATUS "Looking for libpng...")
    find_package(PNG REQUIRED)
    target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${PNG_INCLUDE_DIR})
    target_link_libraries (${LOCAL_PROJECT_NAME} PRIVATE ${PNG_LIBRARY})
    target_compile_definitions(${LOCAL_PROJECT_NAME} PRIVATE cimg_use_png=1)
endif()


# ---GLFW---
find_package(glfw3 REQUIRED)
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE glfw3)


# ---GLEW---
find_package(GLEW REQUIRED)
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIRS})
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE glew32)


# ---OpenGL---
find_package(OpenGL REQUIRED)
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE openGL32)


# ---IWYU---
if (CMAKE_BUILD_TYPE STREQUAL  "Debug")
    find_program(iwyu_path NAMES include-what-you-use iwyu REQUIRED)
    set(iwyu_path_and_options ${iwyu_path})
    set_property(TARGET ${LOCAL_PROJECT_NAME} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options}) 
endif ()


# ---SOIL---
# add_library(SOIL INTERFACE)
add_subdirectory("libraries/SOIL")
target_link_libraries(${LOCAL_PROJECT_NAME} PUBLIC soil)
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${SOIL_H_PATH})
