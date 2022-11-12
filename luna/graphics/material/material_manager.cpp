#include "luna/graphics/material/material_manager.hpp"
#include "luna/graphics/material/material.hpp"
#include "luna/io/json.hpp"
#include "luna/log/log.hpp"
#include <unordered_map>
#include <istream>
#include <fstream>
namespace luna {
namespace gfx {

using json = nlohmann::json;

auto get_materials() -> std::unordered_map<std::string, std::unique_ptr<Material>>& {
  static auto map = std::unordered_map<std::string, std::unique_ptr<Material>>();
  return map;
};

auto MaterialManager::initialize_materials(std::string_view database_path) -> void {
  auto& materials = get_materials();
  const auto material_path = std::string(database_path) + "/materials.json";
  const auto shader_path = std::string(database_path) + "shaders/";

  luna::log_debug("Initializing materials...");
  
  auto stream = std::ifstream(material_path);
  LunaAssert(stream, "Could not load material.json file in database! This is required.");
  auto json = json::parse(stream);
  auto mat_token = json["materials"];
  for(auto token = mat_token.begin(); token != mat_token.end(); ++token) {
    luna::log_debug("Found material ", token.key());
    auto mat_name = std::string(token.key());
    auto vertex_shader = std::string("");
    auto fragment_shader = std::string("");
    for(auto param = token.value().begin(); param != token.value().end(); ++param) {
      luna::log_debug("-- With parameter \"", param.key(), "\" : ", param.value());
      if(param.key() == std::string("vertex")) vertex_shader = param.value();
      if(param.key() == std::string("fragment")) fragment_shader = param.value();
    }

    materials[mat_name] =  std::make_unique<Material>(vertex_shader, fragment_shader);
  }

}

auto MaterialManager::request(std::string_view mat_name) -> const Material* {
  auto& materials = get_materials();
  auto iter = materials.find(std::string(mat_name));

  LunaAssert(iter != materials.end(), "Could not find material ", mat_name, " in the list of loaded materials.");
  return iter->second.get();
}
}
}