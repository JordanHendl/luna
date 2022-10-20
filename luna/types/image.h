#pragma once
#include <ohm/vulkan/vulkan_impl.h>

#include <string>
namespace luna {
using API = ohm::Vulkan;
class Image {
 public:
  Image() { this->m_id = -1; }

  Image(std::string_view name, ohm::Image<API>&& img) {
    this->m_name = name;
    this->m_img = std::move(img);
  }

  auto set_id(int id) { this->m_id = id; }

  auto id() const -> int { return this->m_id; }

  auto name() const -> std::string_view { return this->m_name.c_str(); }

  auto valid() { return this->m_img.handle() >= 0; }

  auto image() -> ohm::Image<API>& { return this->m_img; }

 private:
  int m_id;
  ohm::Image<API> m_img;
  std::string m_name;
};
}  // namespace luna