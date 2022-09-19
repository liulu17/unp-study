cmake_minimum_required(VERSION 3.10)

project(test VERSION 1.0)

add_executable(server server.c wrapper.c wrapper.h)
add_executable(client client.c wrapper.c wrapper.h)
add_executable(test test.c)
add_executable(print_env print_env.c)
add_executable(echo_server echo_server.c wrapper.c wrapper.h)
add_executable(echo_client echo_client.c wrapper.c wrapper.h)