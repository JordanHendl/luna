#include "luna/core/luna.hpp"
#include "luna/graphics/gfx.hpp"
#include "luna/config/cfg,hpp"
#include "luna/plugin/plugin.hpp"
#include "SDL2/SDL.h"
namespace luna {

  auto init_engine(const char* db_path) -> void {
    SDL_Init(SDL_INIT_EVERYTHING);
    //luna::system:::config_request();
    luna::gfx::request_config();
    luna::plugin::request_config();   
    //luna::io::config_request();

    luna::cfg::initialize(db_path);
    //luna::system::initialize();
    luna::gfx::initialize();
    luna::plugin::initialize();
  }
}