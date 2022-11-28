#pragma once
#include <string_view>
#include "luna/graphics/material/material.hpp"
#include <unordered_map>
namespace luna {
namespace gfx {
class Material;
class RenderPass;
class MaterialManager {
public:
  MaterialManager() = default;
  ~MaterialManager() = default;
  auto initialize(const RenderPass& pass, std::string_view database_path) -> void;
  auto request(std::string_view name) const -> const Material*;
private:
  std::unordered_map<std::string, std::unique_ptr<Material>> map;
};
}
}