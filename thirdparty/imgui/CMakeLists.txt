cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)
project(imgui)

file(GLOB SOURCE_FILES 
     imgui.cpp  
     imgui_demo.cpp
     imgui_draw.cpp
     imgui_tables.cpp
     imgui_widgets.cpp

     backends/imgui_impl_win32.cpp
     backends/imgui_impl_dx11.cpp
     
)

    
file(GLOB HEADER_FILES 
    imgui.h
    imconfig.h
    imgui_internal.h
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h

    backends/imgui_impl_win32.h
    backends/imgui_impl_dx11.h
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
