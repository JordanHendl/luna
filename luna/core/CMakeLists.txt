set(luna_files
  luna.cpp
)
add_library(luna SHARED ${luna_files})
target_link_libraries(luna gfx config plugin)