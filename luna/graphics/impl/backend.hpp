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
    io::Symbol<int32_t, ImageInfo, const unsigned char*> make;

    //params: handle | returns nothing
    io::Symbol<void, int32_t> destroy;
  } image;

  struct Descriptor {
    io::Symbol<int32_t, int32_t> make;
    io::Symbol<void, int32_t> destroy;

    io::Symbol<void, std::string_view, int32_t> bind_vertex_buffer;
    io::Symbol<void, std::string_view, int32_t> bind_uniform_buffer;
    io::Symbol<void, std::string_view, int32_t> bind_storage_buffer;
    io::Symbol<void, std::string_view, int32_t> bind_mappable_buffer;
    io::Symbol<void, std::string_view, int32_t> bind_image;
  } descriptor;

  struct Pipeline {
    io::Symbol<int32_t, PipelineInfo> make;
    io::Symbol<void, int32_t> destroy;
  } pipeline;

  struct RenderPass {
    io::Symbol<int32_t, RenderPassInfo, int32_t> make;
    io::Symbol<void, int32_t> destroy;
    io::Symbol<int32_t, std::size_t, std::size_t> get_image;
  } render_pass;

  struct CommandBuffer {
    //params: gpu, parnet | returns handle
    io::Symbol<int32_t, int, int32_t> make;
    
    //params: render pass handle | returns nothing
    io::Symbol<void, int32_t> start_rp;
    
    //params: render pass handle | returns nothing
    io::Symbol<void> end_rp;

    //params: nothing | returns nothing
    io::Symbol<void> begin_recording;

    //params: nothing | returns nothing
    io::Symbol<void> end_recording;

    //params: descriptor handle | returns nothing
    io::Symbol<void, int32_t, int32_t> bind_descriptor;

    //params: vertex buffer handle | returns nothing
    io::Symbol<void, int32_t> draw_vertex;

    //params: vertex buffer handle, index buffer handle | returns nothing
    io::Symbol<void, int32_t, int32_t> draw_indexed;

    //params: handle | returns nothing
    io::Symbol<void, int32_t> destroy;
  } cmd;
};

auto impl() -> GraphicsImplementation&;
}
}