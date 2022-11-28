#include "luna/io/database/database.hpp"

#include <fstream>

#include "luna/io/json.hpp"
namespace luna {
inline namespace v1 {
using json = nlohmann::json;
struct Database::DatabaseData {
  std::string base_path;
  json base;
  json models;
  json images;
  json audio;
};

Database::Database(std::string_view base_path) {
  auto stream = std::fstream(std::string(base_path) + "/data.json");
  if (stream) {
    this->data->base = json::parse(stream);
    auto model_iter = this->data->base.find("models");
    if (model_iter != this->data->base.end()) {
      auto model_path = model_iter->get<std::string>();
      this->data->models = json::parse(std::string(base_path) + model_path);
    }
    auto image_iter = this->data->base.find("images");
    if (image_iter != this->data->base.end()) {
      auto image_path = image_iter->get<std::string>();
      this->data->images = json::parse(std::string(base_path) + image_path);
    }
    auto audio_iter = this->data->base.find("audio");
    if (audio_iter != this->data->base.end()) {
      auto audio_path = audio_iter->get<std::string>();
      this->data->audio = json::parse(std::string(base_path) + audio_path);
    }
  }
}

Database::~Database() {}
auto Database::audio(std::string_view key) -> std::optional<std::string_view> {
  auto iter = data->audio.find(key);
  if (iter != data->audio.end()) {
    return iter->get<std::string_view>();
  }
  return {};
}

auto Database::image(std::string_view key) -> std::optional<std::string_view> {
  auto iter = data->images.find(key);
  if (iter != data->images.end()) {
    return iter->get<std::string_view>();
  }
  return {};
}

auto Database::model(std::string_view key)
    -> std::optional<Database::ModelInfo> {
  auto iter = data->models.find(key);
  if (iter != data->models.end()) {
    auto tmp = ModelInfo();

    auto find_textures = [&iter, &tmp](std::string_view type) {
      auto vec = std::vector<std::string>();
      auto tex_iter = iter->find(type);
      if (tex_iter != iter->end()) {
        for (auto& tex : *tex_iter) {
          vec.push_back(tex.get<std::string>());
        }
      }
      return vec;
    };

    auto path_iter = iter->find("model");
    if (path_iter != iter->end())
      tmp.model_path = path_iter->get<std::string>();
    tmp.albedos = find_textures("albedos");
    tmp.normals = find_textures("normals");
  }
  return {};
}
}  // namespace v1
}  // namespace luna