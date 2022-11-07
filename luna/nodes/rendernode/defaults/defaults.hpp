#pragma once 
#include "luna/graphics/impl/impl.hpp"
#include "luna/io/file/image.hpp"
#include "luna/nodes/rendernode/defaults/default_image.hpp"
#include <memory>
namespace rn {
struct DefaultValues {
  luna::gfx::VertexBuffer image_vertices;
  luna::gfx::Image image;
  luna::gfx::VertexBuffer model;

  DefaultValues() {
    this->make_default_image();
  }

  private:
  auto make_default_image() -> void {
    auto default_image = luna::loadRGBA8(sizeof(DEFAULT_png), DEFAULT_png);
    auto info = luna::gfx::ImageInfo();
    info.format = luna::gfx::ImageFormat::RGBA8;
    info.width = default_image.width;
    info.height = default_image.height;
    info.num_mips = 1;

    this->image = std::move(luna::gfx::Image(info, default_image.pixels.data()));
  }
};
}