#include "luna/io/io.hpp"
#include "luna/io/database/database.hpp"
#include <iostream>
#include <fstream>
#include <istream>

namespace luna {
namespace io {
  auto database() -> luna::Database& {
    static auto db = luna::Database("./");
    return db;
  }

  auto load_bytes(std::string_view path) -> std::vector<unsigned char> {
    auto input = std::ifstream();
    auto bytes = std::vector<unsigned char>();
  
    input.open(path.begin(), std::ios::binary);
  
    if (input) {
      // Copy stream's contents into char buffer.
      input.seekg(0, std::ios::end);
      bytes.reserve(input.tellg());
      input.seekg(0, std::ios::beg);
  
      bytes.assign((std::istreambuf_iterator<char>(input)),
                   std::istreambuf_iterator<char>());
    }

    return bytes;
    }
}
}