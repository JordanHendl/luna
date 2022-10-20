#pragma once
#include <ohm/vulkan/vulkan_impl.h>

#include <glm/glm.hpp>
namespace luna {
using API = ohm::Vulkan;
struct Vertex {
  glm::vec4 pos;
  glm::vec2 uv;
  glm::ivec4 bone_ids;
  glm::vec4 bone_weights;
};

struct Mesh {
  std::string name;
  ohm::Array<API, Vertex> vertices;
  ohm::Array<API, uint32_t> indices;
  std::vector<int> textures;
};

class Model {
 public:
  Model() { this->m_id = -1; };
  Model(std::string_view name, std::vector<int> meshes) {
    this->m_name = name;
    this->m_mesh_ids = meshes;
  }

  auto meshes() -> std::vector<int>& { return this->m_mesh_ids; };

  auto name() -> std::string_view { return this->m_name; }

  auto set_id(int id) { this->m_id = id; }

  auto id() { return this->m_id; }
  auto valid() -> bool { return !this->m_mesh_ids.empty(); }

 private:
  int m_id;
  std::string m_name;
  std::vector<int> m_mesh_ids;
};
}  // namespace luna