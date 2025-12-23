cmake_minimum_required(VERSION 3.10)
project(DinoGame)

set(CMAKE_CXX_STANDARD 17)

# 直接使用编译器内置的EGE库
# 由于EGE库已安装到编译器的include和lib目录中，直接包含头文件即可
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# 查找EGE库文件
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(EGE_LIB_NAME graphics64)
else()
    set(EGE_LIB_NAME graphics)
endif()

find_library(EGE_LIBRARY NAMES ${EGE_LIB_NAME} PATHS ${CMAKE_CURRENT_SOURCE_DIR}/lib)
if(EGE_LIBRARY)
    message(STATUS "Found EGE library: ${EGE_LIBRARY}")
else()
    # 如果在项目目录下没找到，使用链接器能找到的库
    set(EGE_LIBRARY ${EGE_LIB_NAME})
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
)

# 设置输出目录
set_target_properties(dino_game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)