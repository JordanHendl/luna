#include "luna/graphics/asset_manager.hpp"
#include "luna/graphics/types.hpp"
#include "luna/graphics/impl/impl.hpp"
#include "luna/io/io.hpp"
#include "luna/io/database/database.hpp"
#include "luna/io/file/image.hpp"
namespace luna {
namespace gfx {

auto convert(luna::ImageInfo<unsigned char> info) -> gfx::ImageInfo {
  auto tmp = gfx::ImageInfo();
  tmp.width = info.width;
  tmp.height = info.height;
  tmp.format = gfx::ImageFormat::RGBA8;
  return tmp;
}

auto AssetManager::loadImage(std::string_view name) -> Asset<gfx::Image> {
  auto& db = io::database();
  auto img = db.image(name);
  auto loader = luna::loadRGBA8(img.value().begin());

  auto image_info = convert(loader);
  image_info.gpu = 0;

  auto ptr = std::make_shared<gfx::Image>(image_info, loader.pixels.data());
  return Asset(std::string(name), ptr);
}
}
}