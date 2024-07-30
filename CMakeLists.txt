cmake_minimum_required(VERSION 3.27)
project(BWGEngine)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static")

add_subdirectory(extern)

add_executable(${PROJECT_NAME} src/main.cpp
        src/application.hpp
        src/application.cpp
        src/utils/GLCommon.hpp
        src/model.cpp
        src/model.hpp
        src/shader.cpp
        src/shader.hpp
        src/utils/files.hpp
        src/utils/OBJConverter/objconverter.cpp
        src/input.hpp
        src/utils/assets.hpp
        src/utils/not_defined.hpp
        src/mesh.hpp
        src/transform.hpp
        src/camera.hpp
        src/player.hpp
        src/object.hpp
        src/utils/utils.hpp
        src/utils/utils.cpp
        src/utils/vector_utils.hpp
        src/time.hpp
)

set(ASSETS_SOURCE_DIR "${CMAKE_SOURCE_DIR}/assets")
set(ASSETS_DEST_DIR "${CMAKE_BINARY_DIR}/assets")

# Create a custom target for copying assets
add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${ASSETS_SOURCE_DIR} ${ASSETS_DEST_DIR}
        COMMENT "Copying assets directory to build folder"
)

# Make the executable depend on the custom target
add_dependencies(${PROJECT_NAME} copy_assets)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw glm::glm glad stb tinyobjloader ImGui)

#file(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/assets)
#file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})