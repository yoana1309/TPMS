if(EXISTS "C:/diplomna/TPMS/build/test/components/TPMSTest[1]_tests.cmake")
  include("C:/diplomna/TPMS/build/test/components/TPMSTest[1]_tests.cmake")
else()
  add_test(TPMSTest_NOT_BUILT TPMSTest_NOT_BUILT)
endif()
