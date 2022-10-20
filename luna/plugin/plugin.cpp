#include "luna/plugin/plugin.hpp"
#include "luna/plugin/scheduler.hpp"
#include "luna/config/bus.hpp"
#include <string>
#include <utility>
namespace luna {
namespace plugin {
struct PluginConfiguration {
  std::string plugin_location = "";
  Scheduler scheduler;
  auto set_db_location(std::string val) -> void {this->plugin_location = std::string(val) + std::string("/plugins");}
  auto receive_node(std::string val) -> void {this->nodes_to_load.push_back(std::string(val));}

  std::vector<std::string> nodes_to_load;
  cfg::Bus bus;
};

auto config() -> PluginConfiguration& {
  static auto cfg = PluginConfiguration();
  return cfg;
}

auto request_config() -> void {
 auto& cfg = config();
 auto& bus = cfg.bus;

 bus.subscribe("cfg::database_path", &cfg, &PluginConfiguration::set_db_location);
 bus.subscribe("cfg::plugin::nodes", &cfg, &PluginConfiguration::receive_node);
 //TODO subscribe to any configuration requests.
}

auto initialize() -> void {
  auto& cfg = config();
  cfg.scheduler = std::move(Scheduler(cfg.plugin_location));
  for(auto& node : cfg.nodes_to_load) std::cout << node << std::endl;
}

auto shutdown() -> void {
}
}
}