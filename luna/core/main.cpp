#include "luna/core/luna.hpp"
#include <iostream>
auto main(int argc, const char* argv[]) -> int {
  if(argc < 2) {
    std::cout << "Usage: " << argv[0] << " <path_to_config_dir>" << std::endl;
    return 0;
  }
  luna::init_engine(argv[1]);
  return luna::run();
}