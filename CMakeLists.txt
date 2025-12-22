cmake_minimum_required(VERSION 3.10)
project(DinoGame)

set(CMAKE_CXX_STANDARD 17)

# 设置编译器路径
set(CMAKE_C_COMPILER "E:/CLion 2025.2.2/bin/mingw/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "E:/CLion 2025.2.2/bin/mingw/bin/g++.exe")

# 设置EGE库路径
set(EGE_ROOT "E:/CLion 2025.2.2/bin/mingw")

# 包含目录
include_directories(${EGE_ROOT}/include)

# 源文件
set(SOURCES
    src/OptimizedMain.cpp
    src/OptimizedDinoGame.cpp
)

# 创建可执行文件
add_executable(dino_game ${SOURCES})

# 链接库
target_link_libraries(dino_game 
    ${EGE_ROOT}/lib/libgraphics.a
    gdi32
    user32
    kernel32
    gdiplus
    -static
)

# 设置输出目录
set_target_properties(dino_game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)