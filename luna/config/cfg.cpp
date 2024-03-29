#include <filesystem>
#include <fstream>

#include "luna/config/bus.hpp"
#include "luna/config/cfg,hpp"
#include "luna/io/json.hpp"
using json = nlohmann::json;
namespace luna {
namespace cfg {
struct ConfigurationData {
  Bus bus;

  ~ConfigurationData() { Bus::shutdown(); }
};
auto data() -> ConfigurationData& {
  static auto cfg = ConfigurationData();
  return cfg;
}

template <typename cfg, typename string>
auto find_and_parse(cfg& config, string str) -> void {
  auto& bus = data().bus;
  if (config.contains(str)) {
    auto subsystem = config[str];
    for (auto token = subsystem.begin(); token != subsystem.end(); ++token) {
      auto concatenated_str = std::string("cfg::") + std::string(str) +
                              "::" + std::string(token.key());
      auto val = token.value();

      auto handle = [&token, &bus, &concatenated_str](json::reference& value) {
        if (value.is_number()) {
          bus.publish_by_val<float>(concatenated_str, value);
        } else if (value.is_string()) {
          bus.publish_by_val<std::string>(concatenated_str, value);
        } else if (value.is_boolean()) {
          bus.publish_by_val<bool>(concatenated_str, value);
        }
      };

      if (val.is_object()) {
        bus.publish_by_val<json::reference&>(concatenated_str, val);
      } else if (val.is_array()) {
        for (auto index = 0u; index < val.size(); index++) {
          handle(val[index]);
        }
      } else {
        handle(val);
      }
    }
  }
}
auto parse_and_send(std::string& path) -> void {
  auto stream = std::ifstream(path);
  auto json = json::parse(stream);

  find_and_parse(json, "plugin");
}

auto initialize(std::string_view db_path) -> void {
  data().bus.publish_by_val<std::string>("cfg::base_path",
                                         std::string(db_path));
  auto config_path = std::string(db_path) + "/config.json";
  if (std::filesystem::exists(config_path)) {
    parse_and_send(config_path);
  } else {
    std::cout << "\nConfig " << config_path
              << " does not exist in the base path. Cannot parse any "
                 "configuration data. Getting default values.\n";
  }
}

auto shutdown() -> void {}
}  // namespace cfg
}  // namespace luna