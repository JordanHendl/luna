#pragma once
#include <string_view>
namespace luna {
namespace gfx {
class Material;
class MaterialManager {
public:
  static auto initialize_materials(std::string_view database_path) -> void;
  MaterialManager();
  ~MaterialManager();
  auto request(std::string_view name) -> const Material*;
private:

};
}
}