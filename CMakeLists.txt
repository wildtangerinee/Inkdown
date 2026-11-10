cmake_minimum_required(VERSION 3.21)

project(Inkdown LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(Inkdown
    src/app/main.cpp
    src/editor/MarkdownEditor.cpp
)

target_include_directories(Inkdown PRIVATE src)
target_link_libraries(Inkdown PRIVATE Qt6::Widgets)

set_target_properties(Inkdown PROPERTIES MACOSX_BUNDLE ON)
