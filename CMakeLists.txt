cmake_minimum_required(VERSION 3.10)
project(DinoGame)

set(CMAKE_CXX_STANDARD 17)

# 尝试查找EGE头文件
find_path(EGE_INCLUDE_DIR graphics.h)

if(EGE_INCLUDE_DIR)
    message(STATUS "Found EGE headers in: ${EGE_INCLUDE_DIR}")
else()
    message(WARNING "EGE graphics.h not found in standard paths. Make sure EGE is installed in compiler's include directory.")
endif()

# 尝试查找EGE库文件
find_library(EGE_LIBRARY
    NAMES graphics libgraphics libgraphics64
    PATHS ENV LIB
    PATHS_DEFAULT
)

if(EGE_LIBRARY)
    message(STATUS "Found EGE library: ${EGE_LIBRARY}")
else()
    # 如果找不到具体的库文件，使用库名让链接器自行查找
    set(EGE_LIBRARY graphics)
    message(STATUS "EGE library will be resolved by linker")
endif()

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
    -mwindows  # 隐藏控制台窗口
)

# 设置输出目录
set_target_properties(dino_game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)