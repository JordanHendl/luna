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

class GameObject {
  public:
    auto position() const -> const glm::vec3& {return this->m_pos;}
    auto set_position(glm::vec3 pos) -> void {this->m_pos = pos;};
    auto renderable() -> const gfx::Renderable* {return this->m_renderable.get();}
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
  auto add(std::string_view name, const GameObject* render) -> void;
  auto add(std::string_view name, const std::vector<GameObject*> list) -> void;
  auto get(std::string_view name) const -> const GameObject*;
  auto get() const -> const std::unordered_map<std::string, const GameObject*>&;
  auto remove(std::string_view name) -> void;
  auto enable(const TerrainConfig& cfg) -> void;
  auto camera() -> const glm::mat4&;
  auto set_camera(const glm::mat4& mat) -> void;
  auto attach_window(gfx::Window* window) -> void;
  auto detach_window() -> void;
private:
  struct SceneData;
  std::unique_ptr<SceneData> m_data;
};
}