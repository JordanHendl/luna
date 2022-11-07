#include <iostream>
#include "luna/io/dlloader.hpp"
#include "luna/core/engine.hpp"
#include "luna/io/event.hpp"
#include "luna/log/log.hpp"
#include "luna/graphics/renderable.hpp"
#include "luna/graphics/impl/impl.hpp"
#include "luna/nodes/rendernode/defaults/defaults.hpp"

namespace rn {
  struct GlobalResources {
    luna::gfx::VertexBuffer buffer;
  } data;
}
LUNA_C_API auto initialize() -> void {
  luna::log_debug("Rendernode initializing!");
}

LUNA_C_API auto update() -> void {
  //Main renderloop
  luna::log_debug("rendernode update");

  // Grab defaults
  static auto defaults = rn::DefaultValues();

  //Grab scene
  auto* scene = luna::get_current_scene();

  //For each scene...
  if(scene) {

    // Grab objects, swapchain, render_pass, and latest available command buffer for this scene.
    const auto& objects = scene->get();

    // Start the current render frame.
    auto& cmd_buffer = scene->start();

    // Then, for every object in this scene...
    for(auto& obj : objects) {
      
      // Grab the 'renderable' content of the object.
      auto* renderable = obj.second.second.get();
      auto* object     = obj.second.first;

      // Set the object's per-frame info (Model, VP matrices, etc...) 
      renderable->set_per_frame_info(scene, object->position());

      // Set the cmd buffer to the use the object's descriptor when drawing from this point on.
      cmd_buffer.set_descriptor(renderable->descriptor());

      // Render it
      luna::log_debug("Rendernode rendering object ", obj.first);
      renderable->render(cmd_buffer);
    }

    // End scene execution.
    scene->end();

  } else {
    luna::log_debug("Rendernode has no valid scene!");
  }
}

LUNA_C_API auto shutdown() -> void {
  luna::log_debug("Rendernode shutting down!");
}
