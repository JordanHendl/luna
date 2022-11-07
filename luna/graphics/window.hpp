#pragma once
#include <string>
#include <memory>

namespace luna {
namespace gfx {
struct WindowInfo {
  std::string title;
  size_t width;
  size_t height;
  bool borderless;
  bool fullscreen;
  bool resizable;
  bool shown;
  bool capture_mouse;
  bool vsync;

  WindowInfo(std::string_view name, size_t width = 1280, size_t height = 1024,
             bool resizable = false) {
    this->title = name;
    this->width = width;
    this->height = height;
    this->resizable = resizable;
    this->borderless = false;
    this->fullscreen = false;
    this->shown = true;
    this->capture_mouse = false;
    this->vsync = false;
  }

  WindowInfo(size_t width, size_t height) {
    this->title = "Window";
    this->width = width;
    this->height = height;
    this->borderless = false;
    this->fullscreen = false;
    this->resizable = false;
    this->shown = true;
    this->capture_mouse = false;
    this->vsync = false;
  }

  WindowInfo() {
    this->title = "Window";
    this->width = 1280;
    this->height = 1024;
    this->borderless = false;
    this->fullscreen = false;
    this->resizable = false;
    this->shown = true;
    this->capture_mouse = false;
    this->vsync = false;
  }
};
  class Window {
    public:
      Window();
      explicit Window(WindowInfo info);
      explicit Window(Window&& mv);
      ~Window();
      auto operator=(Window&& mv) -> Window&;
      auto update(WindowInfo info) -> void;
      auto info() const -> const WindowInfo&;
      auto set_title(std::string_view title) -> void;
      auto set_borderless(bool value) -> void;
      auto set_fullscreen(bool value) -> void;
      auto set_width(size_t value) -> void;
      auto set_height(size_t value) -> void;
      auto set_xpos(size_t value) -> void;
      auto set_ypos(size_t value) -> void;
      auto set_resizable(bool value) -> void;
      auto set_monitor(size_t value) -> void;
      auto set_minimized(bool value) -> void;
      auto set_maximized(bool value) -> void;
      auto set_shown(bool value) -> void;
      auto set_mouse_capture(bool value) -> void;
      auto title() -> std::string_view;
      auto borderless() -> bool;
      auto fullscreen() -> bool;
      auto has_focus() -> bool;
      auto width() -> size_t;
      auto height() -> size_t;
      auto xpos() -> size_t;
      auto ypos() -> size_t;
      auto monitor() -> size_t;
      auto resizable() -> bool;
      auto minimized() -> bool;
      auto maximized() -> bool;
      auto swapchain() -> int32_t;
    private:
      struct WindowData;
      std::unique_ptr<WindowData> m_data;
  };
}
}