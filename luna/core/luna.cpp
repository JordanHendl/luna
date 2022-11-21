#include "luna/core/luna.hpp"
#include "luna/graphics/gfx.hpp"
#include "luna/config/cfg,hpp"
#include "luna/log/log.hpp"
#include "luna/plugin/plugin.hpp"
#include "SDL2/SDL.h"
#include <iostream>
#include <iomanip>
namespace luna {

  auto log_banner() -> void {
    luna::log_debug(std::setfill('='), std::setw(100));
    luna::log_debug("| |                       |  ___|          (_)           ");
    luna::log_debug("| |    _   _ _ __   __ _  | |__ _ __   __ _ _ _ __   ___ ");
    luna::log_debug("| |   | | | | '_ \\ / _` | |  __| '_ \\ / _` | | '_ \\ / _ \\");
    luna::log_debug("| |___| |_| | | | | (_| | | |__| | | | (_| | | | | |  __/");
    luna::log_debug("\\_____/\\__,_|_| |_|\\__,_| \\____/_| |_|\\__, |_|_| |_|\\___|");
    luna::log_debug("                                       __/ |             ");
    luna::log_debug("                                      |___/              ");
    luna::log_debug(std::setfill('='), std::setw(100));
  }

  auto init_engine(const char* db_path) -> void {
    // Output the banner to logs to be cool.
    log_banner();

    SDL_Init(SDL_INIT_EVERYTHING);

    //luna::system:::config_request();
    luna::gfx::request_config();
    luna::plugin::request_config();   
    luna::request_config();
    //luna::io::config_request();
    
    luna::log_debug("Initializing configuration...");
    luna::cfg::initialize(db_path);
    luna::log_debug(std::setfill('-'), std::setw(100));
    //luna::system::initialize();
    luna::log_debug("Initializing graphics...");
    luna::gfx::initialize();
    luna::log_debug(std::setfill('-'), std::setw(100));
    luna::log_debug("Initializing scheduler...");
    luna::plugin::initialize();
    luna::log_debug(std::setfill('-'), std::setw(100));
  }
}