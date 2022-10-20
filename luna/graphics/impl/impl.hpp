#pragma once
#include "luna/io/dlloader.hpp"
#include "luna/graphics/types.hpp"
namespace luna {
namespace gfx {
struct GraphicsImplementation {
  io::Symbol<void> initialize;
  io::Symbol<int32_t, int, size_t> make_vertex_buffer;
  io::Symbol<int32_t, int, size_t> make_uniform_buffer;
  io::Symbol<int32_t, int, size_t> make_mappable_buffer;
  io::Symbol<void, int32_t> destroy_buffer;
  io::Symbol<int32_t, ImageInfo, unsigned char*> make_image;
  io::Symbol<void, int32_t> destroy_image;
};

auto impl() -> GraphicsImplementation&;
}
}