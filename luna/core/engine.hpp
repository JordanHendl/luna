#pragma once
#include <glm/glm.hpp>
#include <string_view>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
namespace luna  {
namespace gfx {
  class Renderable;
  class Window;
  class RenderPass;
  class CommandBuffer;
}

//TODO the below code probably shouldn't be here, but I'm not sure where to put it currently.
class Scene;
class TerrainConfig;

auto set_current_scene(Scene* scene) -> void;
auto get_current_scene() -> Scene*;

// Call to start main engine loop. Stalls until @running is false.
auto run() -> int;

// Returns true while the engine is supposed to be running. This is used to basically say 'please clean up' to the entire engine.
auto running() -> bool;

// Call this if you want to stop the engine. Sets @running to false.
auto stop_engine() -> void;

auto request_config() -> void;

class GameObject {
  public:
    auto position() const -> const glm::vec3& {return this->m_pos;}
    auto set_position(glm::vec3 pos) -> void {this->m_pos = pos;};
    virtual auto update(float delta_time) -> void = 0;
  protected:
    // User needs to create this.
    std::unique_ptr<gfx::Renderable> m_renderable;
    glm::vec3 m_pos;
};

class Scene {
public: 
  Scene();
  ~Scene();

  template<typename Renderable, typename ...Args>
  auto add(std::string_view name, const GameObject* obj, Args... args) -> void {
    this->add(name, {obj, std::make_unique<Renderable>(args...)});
  }

  auto get(std::string_view name) const -> const GameObject*;
  auto get() -> std::unordered_map<std::string, std::pair<const GameObject*, std::unique_ptr<gfx::Renderable>>>&;
  auto remove(std::string_view name) -> void;
  auto enable(const TerrainConfig& cfg) -> void;
  auto camera() -> const glm::mat4&;
  auto set_camera(const glm::mat4& mat) -> void;
  auto window() -> gfx::Window*;
  auto attach_window(gfx::Window* window) -> void;
  auto detach_window() -> void;
  auto render_pass() -> gfx::RenderPass&;
  auto start() -> gfx::CommandBuffer&;
  auto end() -> void;
private:
  
  auto add(std::string_view name, std::pair<const GameObject*, std::unique_ptr<gfx::Renderable>> obj_pair) -> void;
  struct SceneData;
  std::unique_ptr<SceneData> m_data;
};
}