#pragma once
#include <cstddef>
namespace luna {
namespace plugin {
auto request_config() -> void;
auto initialize() -> void;
auto shutdown() -> void;
auto num_nodes() -> std::size_t;
auto tick() -> bool;
}
}