cmake_minimum_required(VERSION 3.10)
project(DinoGame)

set(CMAKE_CXX_STANDARD 17)

# EGE库文件已放置在编译器的标准include和lib目录中，CMake和编译器会自动找到它们

# 设置EGE库名称
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(EGE_LIB_NAME graphics64)
else()
    set(EGE_LIB_NAME graphics)
endif()

# 由于库文件已在编译器标准路径中，直接使用库名称
set(EGE_LIBRARY ${EGE_LIB_NAME})

message(STATUS "Using EGE library from compiler standard paths")

# 源文件
set(SOURCES
    src/OptimizedMain.cpp
    src/OptimizedDinoGame.cpp
)

# 创建可执行文件
add_executable(dino_game ${SOURCES})

# 链接库
target_link_libraries(dino_game 
    ${EGE_LIBRARY}
    gdi32
    user32
    kernel32
    gdiplus
    winmm
    -static
)

# 设置输出目录
set_target_properties(dino_game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)