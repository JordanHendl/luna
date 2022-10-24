#pragma once
#include "luna/graphics/window.hpp"
#include "luna/graphics/renderer.hpp"
#include "luna/graphics/types.hpp"
namespace luna {
namespace gfx {
auto request_config() -> void;
auto initialize() -> void;
auto shutdown() -> void;
}
}