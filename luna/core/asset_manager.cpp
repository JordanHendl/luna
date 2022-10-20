#include "asset_manager.h"

namespace luna {

constexpr auto MAX_ASSETS = 2048;
Assets::Assets() {
  this->images.resize(MAX_ASSETS);
  this->pipelines.resize(MAX_ASSETS);
}

Assets::~Assets() {}
auto assets() -> Assets& {
  static Assets assets;
  return assets;
}
}  // namespace luna