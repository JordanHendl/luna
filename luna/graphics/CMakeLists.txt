add_subdirectory(impl)
add_subdirectory(vulkan)
find_package(SDL2 REQUIRED)
set(gfx_files
  gfx.cpp
  renderable.cpp
  shader.cpp
  asset_manager.cpp
  
  material/material.cpp
  material/material_manager.cpp
)

set(gfx_api
  gfx.hpp
  window.hpp
  renderable.hpp
  types.hpp
  shader.hpp

  material/material.hpp
  material/material_manager.hpp
)

add_library(gfx SHARED ${gfx_files})
target_link_libraries(gfx PUBLIC io config gfx_impl SDL2::SDL2 shaderc_combined spirv_reflect)
target_include_directories(gfx PUBLIC ${spirv-reflect_include_dirs})