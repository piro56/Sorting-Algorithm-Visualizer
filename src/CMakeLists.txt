project(Sorting)

add_executable(${PROJECT_NAME} main.cpp)

include_directories(../dep/GLFW/include ../dep/glm)


target_include_directories(${PROJECT_NAME}
   PUBLIC ../dep/glm
)

# Link Libraries
# target_include_directories(${PROJECT_NAME} PUBLIC ../dep/glm)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC OpenGL::GL
    PUBLIC sub::simplegl
    PRIVATE glad
    PUBLIC glfw3 ${GLFW_LIBRARIES}
    )
# target_link_libraries(${PROJECT_NAME} PRIVATE glad)
# target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})