#include "luna/graphics/gfx.hpp"
#include "luna/graphics/impl/impl.hpp"
#include "luna/graphics/material/material_manager.hpp"
#include "luna/config/bus.hpp"
#include "luna/io/dlloader.hpp"
#include "luna/error/error.hpp"
namespace luna {
namespace gfx {
  struct GraphicsConfiguration {
    std::string db_location = "";
    std::string backend = "vulkan";
    io::Dlloader dlloader;

    auto set_db_location(std::string val) -> void {this->db_location = val;}
    
    cfg::Bus bus;
  };

  auto config() -> GraphicsConfiguration& {
    static auto cfg = GraphicsConfiguration();
    return cfg;
  }

  auto request_config() -> void {
    auto& cfg = config();
    auto& bus = cfg.bus;

    bus.subscribe("cfg::database_path", &cfg, &GraphicsConfiguration::set_db_location);
    //TODO subscribe to any configuration requests.
  }

  auto initialize() -> void {
    auto& cfg = config();
    auto& imp = impl();
    auto& dlloader = cfg.dlloader;
    
    if(!cfg.db_location.empty()) {
      LunaAssert(cfg.backend == "vulkan", "Other graphics backends are not supported. Only vulkan is available currently.");
      //  TODO: Make this ifdef and have a windows version.
      auto lib_path = cfg.db_location + std::string("/backend/libluna_vulkan.so");

      // Do backend initialization. 
      dlloader.load(lib_path.c_str());
      imp.system.initialize = dlloader.symbol("initialize");
      imp.buffer.make_vertex = dlloader.symbol("make_vertex_buffer");
      imp.buffer.make_uniform = dlloader.symbol("make_uniform_buffer");
      imp.buffer.make_mappable = dlloader.symbol("make_mappable_buffer");
      imp.buffer.destroy = dlloader.symbol("destroy_buffer");

      imp.image.make = dlloader.symbol("make_image");
      imp.image.destroy = dlloader.symbol("destroy_image");

      imp.render_pass.make = dlloader.symbol("make_render_pass");
      imp.render_pass.destroy = dlloader.symbol("destroy_render_pass");
      //imp.render_pass.get_image = dlloader.symbol("get_render_pass_image");

      imp.pipeline.make_render = dlloader.symbol("make_render_pipeline");
      imp.pipeline.make_compute = dlloader.symbol("make_compute_pipeline");
      imp.pipeline.destroy = dlloader.symbol("destroy_pipeline");

      imp.descriptor.make = dlloader.symbol("make_descriptor");
      imp.descriptor.destroy = dlloader.symbol("destroy_descriptor");

      imp.cmd.make = dlloader.symbol("make_command_buffer");
      imp.cmd.destroy = dlloader.symbol("destroy_command_buffer");
      imp.cmd.bind_descriptor = dlloader.symbol("set_descriptor");
      imp.cmd.begin_recording = dlloader.symbol("begin_command_buffer");
      imp.cmd.end_recording = dlloader.symbol("end_command_buffer");
      imp.cmd.start_rp = dlloader.symbol("begin_render_pass");
      imp.cmd.end_rp = dlloader.symbol("end_render_pass");

      imp.window.make = dlloader.symbol("make_window");
      imp.window.destroy = dlloader.symbol("destroy_window");
      
      imp.system.initialize();
    }
  }
}
}