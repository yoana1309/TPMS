cmake_minimum_required(VERSION 3.18.0)

set(This TPMSTest)

set(Sources
    TPMSTests.cpp
)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
    gtest_main
    gmock_main
    COM
    Resources
    MCUWakeUp
    Sensors
    Timer
    TPMSComponent
)



gtest_discover_tests(${This} DISCOVERY_MODE POST_BUILD)