add_subdirectory(database)

# add_subdirectory(lum_loader)
add_subdirectory(file)
find_package(SDL2 REQUIRED)

set(io_files
  dlloader.cpp
  event.cpp
  io.cpp
)

add_library(io STATIC ${io_files})
target_link_libraries(io PUBLIC ${CMAKE_DL_LIBS} SDL2::SDL2 luna_io_file_loaders database)