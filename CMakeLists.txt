cmake_minimum_required(VERSION 3.10)

project(BFS-DFS-Traveling)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

set(project_sources
    src/main.cpp
    src/MainWindow.cpp
    src/Graph.cpp
    src/Vertex.cpp
	src/PathFinder.cpp)

set(project_headers
    include/MainWindow.h
    include/Graph.h
    include/Vertex.h
	include/PathFinder.h)

set(project_ui
    ui/MainWindow.ui)

qt5_wrap_cpp(project_sources_moc ${project_headers})
qt5_wrap_ui(project_ui_wrap ${project_ui})

add_executable(${PROJECT_NAME} 
    ${project_sources} 
    ${project_headers}
    ${project_sources_moc}
    ${project_ui_wrap})

target_link_libraries(${PROJECT_NAME} 
    PUBLIC 
    Qt5::Core Qt5::Gui Qt5::Widgets)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)