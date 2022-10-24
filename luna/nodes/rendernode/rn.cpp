#include <iostream>
#include "luna/io/dlloader.hpp"
#include "luna/core/engine.hpp"
#include "luna/io/event.hpp"
#include "luna/log/log.hpp"
LUNA_C_API auto initialize() -> void {
  luna::log_debug("Rendernode initializing!");
}

LUNA_C_API auto update() -> void {
  luna::log_debug("rendernode update");

  auto* scene = luna::get_current_scene();
  if(scene) {
    const auto& objects = scene->get();
    for(const auto& obj : objects) {
      // do rendering.
      luna::log_debug(obj.first);
    }
  }
}

LUNA_C_API auto shutdown() -> void {
  luna::log_debug("Rendernode shutting down!");
}
