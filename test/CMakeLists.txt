cmake_minimum_required(VERSION 3.10)

add_executable(logger TestLogger.cpp)	
target_link_libraries(logger logger_shared)
