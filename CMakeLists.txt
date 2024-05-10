cmake_minimum_required(VERSION 3.10)

set(LOCAL_PROJECT_NAME "Sculptor")
set(LOCAL_PROJECT_VERSION "0.0.1")

project(${LOCAL_PROJECT_NAME}
        VERSION ${LOCAL_PROJECT_VERSION}
        LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOIL_H_PATH "${PROJECT_SOURCE_DIR}/Libraries/SOIL/includes/SOIL")

set(IWYU_ENABLED FALSE)
# set(IWYU_ENABLED TRUE)


file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list "${PROJECT_SOURCE_DIR}/src/*.hpp")
    SET(dir_list "")
    FOREACH (file_path ${new_list})
        # GET_FILENAME_COMPONENT(file_name ${file_path} NAME)
        # message(STATUS "${file_name}")
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        # message(STATUS "${dir_path}")
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH ()
        
    FILE(GLOB_RECURSE new_list "${PROJECT_SOURCE_DIR}/libraries/*.hpp")
    FOREACH (file_path ${new_list})
        # GET_FILENAME_COMPONENT(file_name ${file_path} NAME)
        # message(STATUS "${file_name}")
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        # message(STATUS "${dir_path}")
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH ()

    FILE(GLOB_RECURSE new_list "${PROJECT_SOURCE_DIR}/libraries/*.h")
    FOREACH (file_path ${new_list})
        # GET_FILENAME_COMPONENT(file_name ${file_path} NAME)
        # message(STATUS "${file_name}")
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        # message(STATUS "${dir_path}")
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
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND IWYU_ENABLED)
find_program(iwyu_path NAMES include-what-you-use iwyu REQUIRED)
set(iwyu_path_and_options ${iwyu_path})
set_property(TARGET ${LOCAL_PROJECT_NAME} PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path_and_options}) 
endif ()


# ---SOIL---
# add_library(SOIL INTERFACE)
add_subdirectory("libraries/SOIL")
target_link_libraries(${LOCAL_PROJECT_NAME} PUBLIC soil)
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${SOIL_H_PATH})


# ---GLM---
# find_package(glm REQUIRED)
add_subdirectory("libraries/GLM")
target_link_libraries(${LOCAL_PROJECT_NAME} PUBLIC glm::glm)
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE glm)


add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Werror)

add_compile_options(-fexperimental-library)
add_compile_options(-ltbb)

file(CREATE_LINK
  "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json"
  SYMBOLIC
)
