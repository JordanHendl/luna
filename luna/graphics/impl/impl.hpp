#pragma once
#include "luna/io/dlloader.hpp"
#include "luna/graphics/types.hpp"
namespace luna {
namespace gfx {
struct GraphicsImplementation {
  struct System {
    // No params, returns nothing
    io::Symbol<void> initialize;
  } system;

  struct Buffer {
    // params: gpu, size | returns: handle
    io::Symbol<int32_t, int, size_t> make_vertex;
    io::Symbol<int32_t, int, size_t> make_uniform;
    io::Symbol<int32_t, int, size_t> make_mappable;

    //params: handle | returns nothing
    io::Symbol<void, int32_t> destroy;
  } buffer;

  struct Image {
    //params: ImageInfo, starting values | returns handle
    io::Symbol<int32_t, ImageInfo, unsigned char*> make;

    //params: handle | returns nothing
    io::Symbol<void, int32_t> destroy;
  } image;
};

auto impl() -> GraphicsImplementation&;
}
}