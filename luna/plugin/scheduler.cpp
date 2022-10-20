#include "luna/plugin/scheduler.hpp"
#include "luna/log/log.hpp"
#include <filesystem>
#include <iostream>
namespace luna {
namespace plugin {


Scheduler::Scheduler(std::string_view node_path) {
  const auto fs_path = std::filesystem::path(node_path.begin()); 
  for(const auto& dir : std::filesystem::recursive_directory_iterator(fs_path)) {
    log_debug("Scheduler loading and initializing plugin ", dir.path());
    auto path = std::string(dir.path());
    auto base_filename = std::string(path.substr(path.find_last_of("/\\") + 1));
    const auto p = std::string::size_type(base_filename.find_last_of('.'));
    auto file_without_extension = base_filename.substr(0, p);

    auto& loaded = this->m_nodes[file_without_extension];
    loaded.loader.load(path.c_str());
    loaded.initalize = loaded.loader.symbol("initialize");
    loaded.shutdown = loaded.loader.symbol("shutdown");
    loaded.update = loaded.loader.symbol("update");

    loaded.initalize();
  }
}

auto Scheduler::load(std::string_view node_name) -> void {

}

auto Scheduler::start() -> void {

}

auto Scheduler::start(std::string_view node_name = "") -> void {

}

auto Scheduler::stop() -> void {

}

auto Scheduler::stop(std::string_view node_name = "") -> void {
  for(auto& node : this->m_nodes) {
    node.second.shutdown();
  }
}

auto Scheduler::tick() -> void {

}

//std::unordered_map<std::string, Node> m_nodes;
}
}