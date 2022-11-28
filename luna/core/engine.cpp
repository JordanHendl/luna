#include "luna/core/engine.hpp"
#include "luna/graphics/impl/impl.hpp"
#include "luna/graphics/renderable.hpp"
#include "luna/graphics/material/material_manager.hpp"
#include "luna/plugin/plugin.hpp"
#include "luna/log/log.hpp"
#include "luna/io/io.hpp"
#include "luna/io/event.hpp"
#include "luna/config/bus.hpp"
#include <unordered_map>
#include <string>
namespace luna {

  static Scene* current_scene = nullptr;
  static bool should_run = true;

  struct EngineConfiguration {
    std::string db_location = "";

    auto set_db_location(std::string val) -> void {this->db_location = val;}
    
    cfg::Bus bus;
  };

  auto config() -> EngineConfiguration& {
    static auto cfg = EngineConfiguration();
    return cfg;
  }

  auto request_config() -> void {
    auto& cfg = config();
    auto& bus = cfg.bus;

    bus.subscribe("cfg::database_path", &cfg, &EngineConfiguration::set_db_location);
    //TODO subscribe to any configuration requests.
  }

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
    gfx::MaterialManager material_manager;
    std::unordered_map<std::string, std::pair<const GameObject*, std::unique_ptr<gfx::Renderable>>> m_objects;
    std::vector<gfx::CommandBuffer> m_cmds;
    std::size_t m_curr_cmd = 0;
    gfx::Window* m_window = nullptr;
    gfx::RenderPass m_pass;

    SceneData() {
      this->m_cmds.resize(num_cmds);
      auto rp_info = gfx::RenderPassInfo();
      auto subpass = gfx::Subpass();
      auto attachment = gfx::Attachment();

      attachment.info.format = gfx::ImageFormat::RGBA8;
      attachment.info.width = 1280;
      attachment.info.height = 1024;
      attachment.info.gpu = 0;
      subpass.attachments.push_back(attachment);

      rp_info.width = 1280;
      rp_info.height = 1024;
      rp_info.subpasses.push_back(subpass);
      rp_info.gpu = 0;
      this->m_pass = gfx::RenderPass(rp_info /*swapchain goes here*/);

      for(auto& cmd : this->m_cmds)  {
        cmd = gfx::CommandBuffer(0);
      }

      this->material_manager.initialize(this->m_pass, config().db_location);
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

    this->m_data->m_objects[std::string(name)].second->generate_descriptor(this->m_data->material_manager);
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
    auto& cmd = this->m_data->m_cmds[this->m_data->m_curr_cmd];
    gfx::impl().cmd.begin_recording(cmd.handle());
    gfx::impl().cmd.start_rp(cmd.handle(), this->m_data->m_pass.handle());
    return this->m_data->m_cmds[this->m_data->m_curr_cmd];
  }

  auto Scene::end() -> void {
    auto& cmd = this->m_data->m_cmds[this->m_data->m_curr_cmd];
    gfx::impl().cmd.end_rp(cmd.handle());
    gfx::impl().cmd.end_recording(cmd.handle());
    
    this->m_data->m_curr_cmd++;
    if(this->m_data->m_curr_cmd >= this->m_data->num_cmds) this->m_data->m_curr_cmd = 0;
  }
}