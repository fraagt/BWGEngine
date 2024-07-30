message(STATUS "Fetching ImGui...")

FetchContent_Declare(ImGui
        GIT_REPOSITORY https://github.com/ocornut/imgui
        GIT_TAG v1.90.9
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

message(STATUS "ImGui Fetched!")

add_library(ImGui STATIC
        imgui/imgui.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp

        imgui/backends/imgui_impl_glfw.cpp
        imgui/backends/imgui_impl_opengl3.cpp
)

target_link_libraries(ImGui PUBLIC glfw ${OPENGL_LIBRARIES})

target_include_directories(ImGui
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/imgui
        ${CMAKE_CURRENT_LIST_DIR}/imgui/backends
)