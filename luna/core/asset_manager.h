#pragma once
#include <ohm/vulkan/vulkan_impl.h>

#include <vector>

#include "luna/types/image.h"
#include "luna/types/model.h"
namespace luna {
using API = ohm::Vulkan;
class Model;
class Image;
struct Assets {
  Assets();
  ~Assets();
  std::vector<Mesh> meshes;
  std::vector<Model> models;
  std::vector<Image> images;
  std::vector<ohm::Pipeline<API>> pipelines;
  std::vector<ohm::Array<API, luna::Vertex>> vertices;
  template <typename Type>
  inline auto insert(std::string_view name, Type&& mv) -> int;
};
template <>
inline auto Assets::insert<Model>(Model&& mv) -> int {
  int index = 0;
  for (auto& model : this->models) {
    if (!model.valid()) {
      model = std::move(mv);
      model.set_id(index);
      return index;
    }
    index++;
  }
  return -1;
}
template <>
inline auto Assets::insert<Image>(Image&& mv) -> int {
  int index = 0;
  for (auto& image : this->images) {
    if (!image.valid()) {
      image = std::move(mv);
      image.set_id(index);
      return index;
    }
    index++;
  }
  return -1;
}
template <>
inline auto Assets::insert<ohm::Pipeline<API>>(ohm::Pipeline<API>&& mv) -> int {
  int index = 0;
  for (auto& pipe : this->pipelines) {
    if (pipe.handle() < 0) {
      pipe = std::move(mv);
      return index;
    }
    index++;
  }
  return -1;
}
template <>
inline auto Assets::insert<ohm::Array<API, luna::Vertex>>(
    ohm::Array<API, luna::Vertex>&& mv) -> int {
  int index = 0;
  for (auto& vert : this->vertices) {
    if (vert.handle() < 0) {
      vert = std::move(mv);
      return index;
    }
    index++;
  }
  return -1;
}
auto assets() -> Assets&;
}  // namespace luna