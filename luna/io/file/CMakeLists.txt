set(luna_io_file_loaders_srcs
  image.cpp
)

set(luna_io_file_loaders_hdrs
  image.h
)

add_library(luna_io_file_loaders STATIC ${luna_io_file_loaders_srcs})
target_link_libraries(luna_io_file_loaders ${stb_tools_libraries})
target_include_directories(luna_io_file_loaders PRIVATE ${stb_tools_include_dirs})