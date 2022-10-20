#pragma once
#include "luna/graphics/types.hpp"
#include <vector>
namespace luna {
namespace gfx {
class TerrainConfig;
class Scene {
public: 
  Scene();
  ~Scene();
  auto add(std::string_view name, const GameObject& render) -> void;
  auto add(std::string_view name, const std::vector<GameObject> list) -> void;
  auto get(std::string_view name) -> const Renderable*;
  auto remove(std::string_view name) -> void;
  auto enable(const TerrainConfig& cfg) -> void;
};
}
}