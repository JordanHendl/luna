#pragma once
#include <cstddef>
#include <string>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace luna {
namespace gfx {
enum class ImageFormat {
  RGBA8,
  BRGA8,
  RGBA32F,
};

enum class AttachmentType {
  Color,
  Depth,
};

struct ImageInfo {
  int gpu = 0;
  size_t width = 1280;
  size_t height = 1024;
  size_t layers = 1;
  ImageFormat format = ImageFormat::RGBA8;
  size_t num_mips = 1;
};

struct PipelineInfo {
  int gpu = 0;
  std::vector<const unsigned char*> spirv;
};

struct RenderPassInfo {
  std::vector<std::pair<std::string, std::pair<ImageFormat, AttachmentType>>> attachments;  
};
}
}