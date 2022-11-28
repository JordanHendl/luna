#pragma once 
#include "luna/io/database/database.hpp"
#include <vector>
#include <string_view>

namespace luna {
namespace io {
  auto database() -> luna::Database&;
  auto load_bytes(std::string_view) -> std::vector<unsigned char>;
}
}