find_package(SDL2 REQUIRED)
set(luna_files
  luna.cpp
  engine.cpp
)
add_library(luna SHARED ${luna_files})
target_link_libraries(luna gfx config plugin io SDL2::SDL2)