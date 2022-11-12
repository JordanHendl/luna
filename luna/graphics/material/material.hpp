#pragma once
#include "luna/graphics/impl/impl.hpp"
#include <string_view>
namespace luna {
namespace gfx {
class Material {
public:
  Material();
  Material(std::string_view vertex, std::string_view fragment);
  ~Material();
private:

};
}
}