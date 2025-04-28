cmake_minimum_required(VERSION 3.10)

project(Task1)

set(CMAKE_CXX_STANDARD 11)

# 设置

if(MSVC)
    # 从环境变量中读取路径
    include_directories("$ENV{INCLUDE}")
    link_directories("$ENV{LIB}")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# 定义源文件和头文件目录
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# 添加可执行文件
add_executable(Task1 ${SOURCE_DIR}/main.cpp)

# 添加包含目录
target_include_directories(Task1 PUBLIC ${INCLUDE_DIR})

# 添加库文件
set(LIB1_SRC
    ${LIB_DIR}/res.cpp
    ${LIB_DIR}/Dijk.cpp
    ${LIB_DIR}/PathS.cpp
    ${LIB_DIR}/Console.cpp
    ${LIB_DIR}/Color.cpp
)
add_library(lib1 ${LIB1_SRC})

# 添加库的包含目录
target_include_directories(lib1 PUBLIC ${INCLUDE_DIR})

# 链接库
target_link_libraries(Task1 lib1)



