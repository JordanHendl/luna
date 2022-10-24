#include "luna/core/engine.hpp"
#include "luna/plugin/plugin.hpp"
#include "luna/io/io.hpp"
#include "luna/io/event.hpp"
#include <unordered_map>
#include <string>
namespace luna {

  static Scene* current_scene = nullptr;
  static bool should_run = true;

  auto set_current_scene(Scene* scene) -> void {
    current_scene = scene;
  }

  auto get_current_scene() -> Scene* {
    return current_scene;
  }

  auto run() -> int {
    while(running()) {
      luna::plugin::tick();
      luna::poll_events();
    }

    return 0;
  }

  auto running() -> bool {
    return should_run;
  }

  auto stop_engine() -> void {
    should_run = false;
  }
  
  struct Scene::SceneData {
    std::unordered_map<std::string, const GameObject*> m_objects;
  };

  Scene::Scene() {
    this->m_data = std::make_unique<SceneData>();
  }

  Scene::~Scene() {

  }

  auto Scene::add(std::string_view name, const GameObject* render) -> void {
    this->m_data->m_objects[std::string(name)] = render;
  }

  auto Scene::add(std::string_view name, const std::vector<GameObject*> list) -> void {

  }

  auto Scene::get(std::string_view name) const -> const GameObject* {
    return this->m_data->m_objects[std::string(name)];
  }

  auto Scene::get() const -> const std::unordered_map<std::string, const GameObject*>& {
    return this->m_data->m_objects;
  }

  auto Scene::remove(std::string_view name) -> void {

  }

  auto Scene::enable(const TerrainConfig& cfg) -> void {

  }

  auto Scene::attach_window(gfx::Window* window) -> void {

  }

  auto Scene::detach_window() -> void {

  }
}