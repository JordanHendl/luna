#pragma once
#include <string_view>
#include <string>
#include <unordered_map>
#include "luna/io/dlloader.hpp"
namespace luna {
namespace plugin {
class Scheduler {
public:
  Scheduler() = default;
  Scheduler(std::string_view node_path);
  auto load(std::string_view node_name) -> void;
  auto start() -> void;
  auto start(std::string_view node_name) -> void;
  auto stop() -> void;
  auto stop(std::string_view node_name) -> void;
  auto tick() -> void;
private:
  struct Node {
    io::Dlloader loader;
    io::Symbol<void> initalize;
    io::Symbol<void> update;
    io::Symbol<void> shutdown;
  };
  
  std::unordered_map<std::string, Node> m_nodes;
};
}
}