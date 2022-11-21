#include "luna/graphics/material/material.hpp"
#include "luna/graphics/shader.hpp"
#include "luna/log/log.hpp"
#include <fstream>
#include <cstring>
namespace luna {
namespace gfx {

auto load_shader(std::string_view path) {
  auto shdr = std::ifstream(path.cbegin(), std::ios::binary);
  LunaAssert(shdr, "Unable to load shader ", path, ".");

  log_debug("Loading shader ", path);
  auto tmp = std::vector<char>();
  auto out = std::vector<uint32_t>();
  shdr.seekg(0, std::ios::end);
  tmp.reserve(shdr.tellg());
  shdr.seekg(0, std::ios::beg);
  tmp.assign((std::istreambuf_iterator<char>(shdr)),
             std::istreambuf_iterator<char>());
  shdr.close();

  out.resize(tmp.size() / sizeof(uint32_t));
  std::memcpy(out.data(), tmp.data(), tmp.size());
  return out;
}

Material::Material() {

}

Material::Material(const RenderPass& rp, std::string_view vert, std::string_view frag) {
  auto info = gfx::PipelineInfo();
  auto viewport = gfx::Viewport();
  info.gpu = 0;
  info.spirv = {{gfx::ShaderType::Vertex, load_shader(vert)}, {gfx::ShaderType::Fragment, load_shader(frag)}};
  viewport.width = rp.info().width;
  viewport.height = rp.info().height;
  info.viewports.push_back(viewport);
  this->m_pipeline = Pipeline(rp, info);
}

Material::~Material() {
  
}
}
}