#include "luna/core/luna.hpp"

#include <iomanip>
#include <iostream>

#include "luna/config/cfg,hpp"
#include "luna/plugin/plugin.hpp"
namespace luna {

auto log_banner() -> void {
  std::cout << std::setfill('=') << std::setw(100) << std::endl;
  std::cout << "____                                  " << std::endl;
  std::cout << "`MM'                                  " << std::endl;
  std::cout << " MM                                   " << std::endl;
  std::cout << " MM     ___   ___ ___  __      ___    " << std::endl;
  std::cout << " MM     `MM    MM `MM 6MMb   6MMMMb   " << std::endl;
  std::cout << " MM      MM    MM  MMM9 `Mb 8M'  `Mb  " << std::endl;
  std::cout << " MM      MM    MM  MM'   MM     ,oMM  " << std::endl;
  std::cout << " MM      MM    MM  MM    MM ,6MM9'MM  " << std::endl;
  std::cout << " MM      MM    MM  MM    MM MM'   MM  " << std::endl;
  std::cout << " MM    / YM.   MM  MM    MM MM.  ,MM  " << std::endl;
  std::cout << "_MMMMMMM  YMMM9MM__MM_  _MM_`YMMM9'Yb." << std::endl;
  std::cout << std::setfill('=') << std::setw(100) << std::endl << std::endl;
}

auto init_engine(const char* db_path) -> void {
  // Output the banner to logs to be cool.
  log_banner();
  luna::plugin::request_config();
  luna::cfg::initialize(db_path);
  luna::plugin::initialize();
}

auto run() -> int {
  auto running = true;
  if(luna::plugin::num_nodes() == 0) {
    std::cout << "No nodes found to be loaded. Exitting" << std::endl;
    return 0;
  }

  while(running) {
    running = luna::plugin::tick();
  }
  return 0;
}
}  // namespace luna