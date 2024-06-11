if(EXISTS "C:/diplomna/TPMS/build/test/components/TPMSTest.exe")
  if(NOT EXISTS "C:/diplomna/TPMS/build/test/components/TPMSTest[1]_tests.cmake" OR
     NOT "C:/diplomna/TPMS/build/test/components/TPMSTest[1]_tests.cmake" IS_NEWER_THAN "C:/diplomna/TPMS/build/test/components/TPMSTest.exe" OR
     NOT "C:/diplomna/TPMS/build/test/components/TPMSTest[1]_tests.cmake" IS_NEWER_THAN "${CMAKE_CURRENT_LIST_FILE}")
    include("C:/Program Files/CMake/share/cmake-3.29/Modules/GoogleTestAddTests.cmake")
    gtest_discover_tests_impl(
      TEST_EXECUTABLE [==[C:/diplomna/TPMS/build/test/components/TPMSTest.exe]==]
      TEST_EXECUTOR [==[]==]
      TEST_WORKING_DIR [==[C:/diplomna/TPMS/build]==]
      TEST_EXTRA_ARGS [==[--gtest_output=xml:C:/diplomna/TPMS/build/report.xml]==]
      TEST_PROPERTIES [==[]==]
      TEST_PREFIX [==[]==]
      TEST_SUFFIX [==[]==]
      TEST_FILTER [==[]==]
      NO_PRETTY_TYPES [==[FALSE]==]
      NO_PRETTY_VALUES [==[FALSE]==]
      TEST_LIST [==[TPMSTest_TESTS]==]
      CTEST_FILE [==[C:/diplomna/TPMS/build/test/components/TPMSTest[1]_tests.cmake]==]
      TEST_DISCOVERY_TIMEOUT [==[5]==]
      TEST_XML_OUTPUT_DIR [==[]==]
    )
  endif()
  include("C:/diplomna/TPMS/build/test/components/TPMSTest[1]_tests.cmake")
else()
  add_test(TPMSTest_NOT_BUILT TPMSTest_NOT_BUILT)
endif()
