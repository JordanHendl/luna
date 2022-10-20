find_package(GTest)

if(GTest_FOUND)
  add_executable(gfx_init test.cpp)
  target_link_libraries(gfx_init GTest::GTest luna)
  add_test(NAME gfx_init COMMAND gfx_init)
endif()