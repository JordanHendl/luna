#include "luna/graphics/impl/impl.hpp"
namespace luna {
namespace gfx {
auto impl() -> GraphicsImplementation& {
  static auto p = GraphicsImplementation();
  return p;
}
}
}