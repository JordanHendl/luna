set(testnode_files
  test.cpp
)

add_library(usertestnode SHARED ${testnode_files})
target_link_libraries(usertestnode luna)