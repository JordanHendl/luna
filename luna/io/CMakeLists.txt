set(io_files
  dlloader.cpp
)

add_library(io STATIC ${io_files})
target_link_libraries(io PUBLIC ${CMAKE_DL_LIBS})