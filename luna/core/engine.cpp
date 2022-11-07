#include "luna/core/engine.hpp"
#include "luna/graphics/impl/impl.hpp"
#include "luna/graphics/renderable.hpp"
#include "luna/plugin/plugin.hpp"
#include "luna/log/log.hpp"
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
    luna::log_debug("Beginning main engine loop...");
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
    const unsigned num_cmds = 3u;

    std::unordered_map<std::string, std::pair<const GameObject*, std::unique_ptr<gfx::Renderable>>> m_objects;
    std::vector<gfx::CommandBuffer> m_cmds;
    std::size_t m_curr_cmd = 0;
    gfx::Window* m_window = nullptr;
    gfx::RenderPass m_pass;

    SceneData() {
      this->m_cmds.resize(num_cmds);

      auto rp_info = gfx::RenderPassInfo();
      rp_info.attachments = {}; // @TODO fill out attachments properly like a good boy
    }
  };

  Scene::Scene() {
    this->m_data = std::make_unique<SceneData>();
  }

  Scene::~Scene() {

  }

  auto Scene::add(std::string_view name, std::pair<const GameObject*, std::unique_ptr<gfx::Renderable>> obj_pair) -> void {
    this->m_data->m_objects[std::string(name)].first = obj_pair.first;
    this->m_data->m_objects[std::string(name)].second = std::move(obj_pair.second);
  }

  auto Scene::get(std::string_view name) const -> const GameObject* {
    return this->m_data->m_objects[std::string(name)].first;
  }

  auto Scene::get() -> std::unordered_map<std::string, std::pair<const GameObject*, std::unique_ptr<gfx::Renderable>>>& {
    return this->m_data->m_objects;
  }

  auto Scene::remove(std::string_view name) -> void {

  }

  auto Scene::enable(const TerrainConfig& cfg) -> void {

  }

  auto Scene::attach_window(gfx::Window* window) -> void {
    this->m_data->m_window = window;
  }

  auto Scene::detach_window() -> void {
    this->m_data->m_window = nullptr;
  }
  
  auto Scene::window() -> gfx::Window* {
    return this->m_data->m_window;
  }

  auto Scene::render_pass() -> gfx::RenderPass& {
    return this->m_data->m_pass;
  }

  auto Scene::start() -> gfx::CommandBuffer& {
    return this->m_data->m_cmds[this->m_data->m_curr_cmd];
  }

  auto Scene::end() -> void {
    this->m_data->m_curr_cmd = this->m_data->m_curr_cmd + 1 % this->m_data->num_cmds;
  }
}