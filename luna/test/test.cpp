#include <gtest/gtest.h>
#include <ohm/vulkan/vulkan_impl.h>

#include <utility>

#include "luna/bus/bus.h"
#include "luna/core/luna.h"

const char* vert_shader =
#include "img2d.vert.glsl"
    ;

const char* frag_shader =
#include "img2d.frag.glsl"
    ;

using API = ohm::Vulkan;
namespace assets {
auto test_add_image() {
  auto& assets = luna::assets();
  auto raw_img = ohm::Image<API>(0, {1280, 1024});
  auto image = luna::Image("test_image", std::move(raw_img));
  auto id = assets.insert(std::move(image));
  return id >= 0;
}

auto test_add_pipeline() {
  auto& assets = luna::assets();
  auto scene = luna::Scene();
  auto pipeline = ohm::Pipeline<API>(scene.render_pass(),
                                     {{{"texture2d.vert.glsl", vert_shader},
                                       {"texture2d.frag.glsl", frag_shader}}});
  auto id = assets.insert(std::move(pipeline));
  return id >= 0;
}
}  // namespace assets
namespace node {
auto test_creation() {
  auto scene = luna::Scene();
  return true;
}

auto test_get_next() {
  auto scene = luna::Scene();
  auto next_index = scene.find_available();
  return next_index >= 0;
}

auto test_root_node() {
  auto& assets = luna::assets();
  auto scene = luna::Scene();
  auto raw_img = ohm::Image<API>(0, {.width = 1280, .height = 1024});
  auto vertices =
      ohm::Array<API, luna::Vertex>(0, 8, ohm::HeapType::HostVisible);
  auto pipeline = ohm::Pipeline<API>(scene.render_pass(),
                                     {{{"texture2d.vert.glsl", vert_shader},
                                       {"texture2d.frag.glsl", frag_shader}}});
  auto image = luna::Image("test_image", std::move(raw_img));

  auto pipeline_id = assets.insert(std::move(pipeline));
  auto vertex_id = assets.insert(std::move(vertices));
  auto tex_id = assets.insert(std::move(image));
  auto& root = scene.root();

  // node.add<node_type      >( name       , type_constructor_parameters);
  root.add<luna::Texture2D>("texture_2d", tex_id, pipeline_id, vertex_id);

  // Make sure children was added and is referenced correctly.
  return (!root.children().empty()) &&
         scene.graph()[root.children()[0]]->name() == std::string("texture_2d");
}
}  // namespace node
namespace bus {
auto test_creation() -> bool {
  auto bus = luna::Bus();
  return true;
}
auto test_function_subscribe() -> bool {
  auto test_func = [](int* value) -> void { *value = 1; };

  using fptr = void (*)(int* value);
  auto bus = luna::Bus();
  auto func_ptr =
      static_cast<fptr>(test_func);  // Standard says lambda with no captures is
                                     // convertable to function ptr
  bus.subscribe("test_function", func_ptr);
  return true;
}
auto test_function_publish() -> bool {
  auto test_func = [](int* value) -> void { *value = 1; };

  using fptr = void (*)(int* value);
  auto tmp = 0;
  auto bus = luna::Bus();
  auto func_ptr =
      static_cast<fptr>(test_func);  // Standard says lambda with no captures is
                                     // convertable to function ptr
  bus.subscribe("test_function", func_ptr);
  bus.push("test_function", &tmp);
  return tmp == 1;
}
auto test_reference_subscribe() -> bool {
  struct tmp {
    bool received = false;
    auto recieve_value(const int& value) -> void { received = true; }
  };
  auto t = tmp();
  auto bus = luna::Bus();
  bus.subscribe("tmp", &t, &tmp::recieve_value);
  return true;
}
auto test_reference_publish() -> bool {
  struct tmp {
    bool received = false;
    auto recieve_value(const int& value) -> void {
      if (value == 2) received = true;
    }
  };
  auto t = tmp();
  auto bus = luna::Bus();
  auto val = 2;
  bus.subscribe("tmp", &t, &tmp::recieve_value);
  bus.push("tmp", val);
  return t.received;
}
auto test_subscribe() -> bool {
  struct tmp {
    bool received = false;
    auto recieve_value(int value) -> void { received = true; }
  };
  auto t = tmp();
  auto bus = luna::Bus();
  bus.subscribe("tmp", &t, &tmp::recieve_value);
  return true;
}
auto test_publish() -> bool {
  struct tmp {
    bool received = false;
    auto recieve_value(int value) -> void {
      if (value == 2) received = true;
    }
  };
  auto t = tmp();
  auto bus = luna::Bus();
  auto val = 300;
  bus.subscribe("tmp", &t, &tmp::recieve_value);
  bus.push("tmp2", val);
  if (t.received) return false;
  val = 2;
  bus.push("tmp", val);
  return t.received;
}
auto test_blasting() -> bool {
  struct sender {
    auto get_value() -> int { return 1; }
  };
  struct receiver {
    int value = 0;
    auto receive_value(int value) { this->value = value; }
  };

  auto s = sender();
  auto r = receiver();
  auto bus = luna::Bus();
  bus.subscribe("test_publish", &r, &receiver::receive_value);
  bus.publish("test_publish", &s, &sender::get_value);
  bus.blast();
  return r.value == 1;
}
}  // namespace bus
TEST(Luna, Assets) {
  EXPECT_TRUE(assets::test_add_image());
  EXPECT_TRUE(assets::test_add_pipeline());
}
TEST(Luna, Node) {
  EXPECT_TRUE(node::test_creation());
  EXPECT_TRUE(node::test_get_next());
  EXPECT_TRUE(node::test_root_node());
}
TEST(Luna, Bus) {
  EXPECT_TRUE(bus::test_creation());
  EXPECT_TRUE(bus::test_subscribe());
  EXPECT_TRUE(bus::test_publish());
  EXPECT_TRUE(bus::test_reference_subscribe());
  EXPECT_TRUE(bus::test_reference_publish());
  EXPECT_TRUE(bus::test_function_subscribe());
  EXPECT_TRUE(bus::test_function_publish());
  EXPECT_TRUE(bus::test_blasting());
}

int main(int argc, char* argv[]) {
  ohm::System<API>::initialize();
  testing::InitGoogleTest(&argc, argv);
  auto success = RUN_ALL_TESTS();
  return success;
}
