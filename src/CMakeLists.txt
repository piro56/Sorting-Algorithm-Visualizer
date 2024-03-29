project(Sorting)
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g")
add_executable(${PROJECT_NAME} main.cpp SortingRects.cpp SortAlgs.cpp)

# Copies asset directory into build

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
)

add_dependencies(${PROJECT_NAME} copy_assets)

include_directories(../dep/GLFW/include ../dep/glm ../dep/IMGUI .)


target_include_directories(${PROJECT_NAME}
   PUBLIC ../dep/glm
)

# Link Libraries
# target_include_directories(${PROJECT_NAME} PUBLIC ../dep/glm)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC OpenGL::GL
    PUBLIC sub::simplegl
    PUBLIC imgui
    PRIVATE glad
    PUBLIC glfw3 ${GLFW_LIBRARIES}
    )
# target_link_libraries(${PROJECT_NAME} PRIVATE glad)
# target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})