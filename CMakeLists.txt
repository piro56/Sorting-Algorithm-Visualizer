cmake_minimum_required(VERSION 3.10)

# set project name
project(Sorting)

# specify the C++ standard

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -DDEBUG -Wall -Wextra")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g")
find_package( OpenGL REQUIRED )
# glm
set(GLM_LIB_NAME "glm")
set(GLM_INC_PATH dep/glm)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(dep/SimpleGL)
add_subdirectory(dep/IMGUI)
add_subdirectory(dep/GLAD)
# add_subdirectory(dep/glm)