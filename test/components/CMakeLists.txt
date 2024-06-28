cmake_minimum_required(VERSION 3.18.0)

set(This TPMSTest)

set(Sources
    TPMSTests.cpp
)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
    gtest_main
    gmock_main
    MessageReceiver
    MessageSender
    SignalProcessor
    Monitor
    E2E
    Resources
)

gtest_discover_tests(${This}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    DISCOVERY_MODE PRE_TEST
    EXTRA_ARGS --gtest_output=xml:${CMAKE_BINARY_DIR}/report.xml
)