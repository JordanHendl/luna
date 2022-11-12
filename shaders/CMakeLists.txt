set(shader_files
  image2d/image2d.vert.glsl
  image2d/image2d.frag.glsl
)

compile_shader(TARGETS ${shader_files} INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/common)