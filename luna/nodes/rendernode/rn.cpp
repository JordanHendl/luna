#include <iostream>
#include "luna/io/dlloader.hpp"

LUNA_C_API auto initialize() -> void {
  std::cout << "initializing rendernode!" << std::endl;
}

LUNA_C_API auto update() -> void {
  std::cout << "rendernode update!" << std::endl;
}

LUNA_C_API auto shutdown() -> void {
  std::cout << "shutting down rendernode!" << std::endl;
}
