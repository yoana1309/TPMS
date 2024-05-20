cmake_minimum_required(VERSION 3.18.0)

set(This TPMSTest)

set(Sources
    TPMSTests.cpp
)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
    gtest_main
    TPMSComponent
)

add_test(
    NAME ${This}
    COMMAND ${This}
)