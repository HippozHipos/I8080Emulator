# CMakeList.txt : CMake project for BadTwitter, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Intel8080")

#compile 3rdParty               _______________________

#set variables
set(IMGUI_INCLUDES thirdparty/imgui/ thirdparty/imgui/backends)


#D3D11
set(BACKENDS_DIR thirdparty/imgui/backends)
file(GLOB DX_FILES 
        ${BACKENDS_DIR}/imgui_impl_dx11.cpp
        ${BACKENDS_DIR}/imgui/imgui_impl_win32.cpp
)

include_directories(${IMGUI_INCLUDES} src)
#include_directories("C:\Program Files (x86)\Windows Kits\10\Lib")
#link_directories("C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64")

#note: we only need to include the imgui cmakelist unlike the glfw
add_subdirectory(${IMGUI_INCLUDES})

#end compile 3rdParty           _______________________

#compile my files
file(GLOB source_files CONFIGURE_DEPENDS
        "*.h"
        "*.cpp"
        "src/*.cpp"
        "src/*.h"
        "src/PlatformWin/*.h"
        "src/PlatformWin/*.cpp"
        "src/dx11/*.h"
        "src/dx11/*.cpp"
        "src/imgooey/*.h"
        "src/imgooey/*.cpp"
        )
            
#set(DUMB_DIR src/mainfiles)
#file (GLOB_RECURSE retarded_dir_files CMAKE_CONFIGURE_DEPENDS
#        ${DUMB_DIR}/*.h
#        ${DUMB_DIR}/*.cpp
#)


# Add source to this project's executable.
# add_executable ( BadTwitter "BadTwitter.cpp" "BadTwitter.h" "src/Manager.cpp" "src/Manager.h" )
#add_executable (BadTwitter ${source_files})
add_executable(${PROJECT_NAME} ${source_files} "src/Error.h" "src/Util.cpp" "src/Util.h"   "src/Error.cpp" "src/Timer.h" "src/Timer.cpp")

#target_link_libraries(BadTwitter ${OPENGL_LIBRARIES} OpenGL::GL glfw imgui)
#find_package(D3DX11_LIBRARY)
#find_library(D3DX11_LIBRARY d3d11.lib "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64")
#find_file(D3D11 d3d11.lib "C:\Program Files (x86)\Windows Kits\10\Lib\10.0.22000.0\um\x64")
target_link_libraries(${PROJECT_NAME} ${D3D11} d3d11.lib d3d11 dxguid imgui)

 set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.