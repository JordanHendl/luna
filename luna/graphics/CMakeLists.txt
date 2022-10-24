add_subdirectory(impl)
add_subdirectory(vulkan)
find_package(SDL2 REQUIRED)
set(gfx_files
  gfx.cpp
  window.cpp
  types.cpp
)

set(gfx_api
  gfx.hpp
  window.hpp
  types.hpp
)

add_library(gfx SHARED ${gfx_files})
target_link_libraries(gfx PUBLIC io config gfx_impl SDL2::SDL2)