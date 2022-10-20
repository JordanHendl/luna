#include "luna/config/bus.hpp"

namespace luna {
namespace cfg {
struct BusData {
  bool init = true;
};
auto get_data() -> BusData& {
  static auto data = BusData();
  return data;
}

auto Bus::initialized() -> bool {
  return get_data().init;
}

auto Bus::shutdown() -> void {
  get_data().init = false;
}

auto Bus::get_subscriptions() -> Bus::KeyMap& {
  static auto map = Bus::KeyMap();
  return map;
}
}  // namespace cfg
}  // namespace luna