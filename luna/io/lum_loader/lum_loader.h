

/*
 * Copyright (C) 2020 Jordan Hendl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once
#include <string>
#include <vector>

namespace luna {
inline namespace v1 {
class ModelLoader {
 public:
  struct Vertex {
    float position[4] = {0.f, 0.f, 0.f, 0.f};
    float normals[4] = {0.f, 0.f, 0.f, 0.f};
    float weights[4] = {0.f, 0.f, 0.f, 0.f};
    unsigned ids[4] = {0, 0, 0, 0};
    float uvs[2] = {0.f, 0.f};
  };

  struct Material {
    std::vector<std::string> m_diffuse;
    std::vector<std::string> m_specular;
    std::vector<std::string> m_normal;
    std::vector<std::string> m_height;
  };

  struct Mesh {
    std::string name;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned> m_indices;
    Material material;
  };

  ModelLoader();
  ~ModelLoader();
  auto generate(const char* path) -> std::vector<Mesh>;
  auto save(const char* path) -> void;
  auto reset() -> void;

 private:
  std::vector<Mesh> model_meshes;
};
}  // namespace v1
}  // namespace luna