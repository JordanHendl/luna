set(rendernode_files
  rn.cpp
)

add_library(rendernode SHARED ${rendernode_files})
target_link_libraries(rendernode luna)