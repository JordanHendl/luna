#include <iostream>
#include "luna/io/dlloader.hpp"
#include "luna/core/luna.hpp"
#include "luna/graphics/gfx.hpp"
#include "luna/log/log.hpp"
#include "luna/io/event.hpp"
#include <string_view>
class TestObject : public luna::GameObject {
  public:
  TestObject()  {
    this->m_renderable = std::make_unique<luna::gfx::Image2D>(&this->m_pos, "default", "default");
  }

  virtual ~TestObject() {};
  auto update(float delta_time) -> void override {}
};

struct TestNodeData {
  luna::EventRegister events;
  luna::Scene scene;
  luna::gfx::Window window;
  TestObject obj;
} data;

auto check_event(const luna::Event& event) -> void {
  luna::log_debug("Test Node: Event Recieved! Event is: ", luna::to_string(event));
  if(event.type() == luna::Event::Type::WindowExit) {
    luna::stop_engine();
  }
}

LUNA_C_API auto initialize() -> void {
  luna::log_debug("Test Node initializing!");

  luna::set_current_scene(&data.scene);
  data.scene.attach_window(&data.window);
  data.scene.add("test_object", &data.obj);
  data.events.add(&check_event);
}

LUNA_C_API auto update() -> void {
luna::log_debug("test node update");
}

LUNA_C_API auto shutdown() -> void {
  luna::log_debug("Test node shutting down!");
}
