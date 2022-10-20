#pragma once
#include <iostream>
namespace luna {
namespace log {
auto request_config() -> void;
auto initialize() -> void;
auto shutdown() -> void;
}

template<typename T, typename ...Args>
auto log_debug(T const & arg, Args ... args) -> void {
  std::cout << ": DEBUG : ";
  ((std::cout << std::forward<Args>(args)), ...);
}
}