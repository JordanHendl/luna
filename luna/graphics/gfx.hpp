#pragma once
#include "luna/graphics/types.hpp"
#include "luna/graphics/renderable.hpp"
namespace luna {
namespace gfx {
auto request_config() -> void;
auto initialize() -> void;
auto shutdown() -> void;
}
}