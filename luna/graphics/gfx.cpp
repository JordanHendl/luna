#include "luna/graphics/gfx.hpp"
#include "luna/graphics/impl/impl.hpp"
#include "luna/config/bus.hpp"
#include "luna/io/dlloader.hpp"
#include "luna/error/error.hpp"
namespace luna {
namespace gfx {
  struct GraphicsConfiguration {
    std::string db_location = "";
    std::string backend = "vulkan";
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
    auto dlloader = io::Dlloader();
    if(!cfg.db_location.empty()) {
      LunaAssert(cfg.backend == "vulkan", "Other graphics backends are not supported. Only vulkan is available.");
      //TODO: Make this ifdef and have a windows version.
      auto lib_path = cfg.db_location + std::string("/backend/libluna_vulkan.so");
      dlloader.load(lib_path.c_str());
      imp.initialize = dlloader.symbol("initialize");
      imp.make_vertex_buffer = dlloader.symbol("make_vertex_buffer");
      imp.make_uniform_buffer = dlloader.symbol("make_uniform_buffer");
      imp.make_mappable_buffer = dlloader.symbol("make_mappable_buffer");
      imp.destroy_buffer = dlloader.symbol("destroy_buffer");

      imp.make_image = dlloader.symbol("make_image");
      imp.destroy_image = dlloader.symbol("destroy_image");
      imp.initialize();
    }
  }
}
}