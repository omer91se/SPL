cmake_minimum_required(VERSION 3.12)
project(Clion)

set(CMAKE_CXX_STANDARD 11)

add_executable(Clion main.cpp src/Customer.cpp include/Dish.cpp)