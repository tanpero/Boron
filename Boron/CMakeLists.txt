cmake_minimum_required (VERSION 3.8)

add_library (Boron SHARED "boron.cpp")
add_executable (tests Boron "tests.cpp")

# TODO: 如有需要，请添加测试并安装目标。

