#pragma once
#include <memory>
#include <vector>
#include "luna/graphics/types.hpp"
#include "luna/graphics/vulkan/vk_wrappers/descriptor.hpp"
#include "luna/graphics/vulkan/vk_wrappers/shader.hpp"
namespace luna {
namespace vulkan {
class Buffer;
class Device;
class Texture;
class RenderPass;
class Descriptor;
class Pipeline {
 public:
  Pipeline();
  Pipeline(Device& gpu, const gfx::PipelineInfo& info);
  Pipeline(RenderPass& gpu, const gfx::PipelineInfo& info);
  Pipeline(Pipeline&& mv);
  ~Pipeline();
  auto operator=(Pipeline&& mv) -> Pipeline&;
  auto descriptor() -> int32_t;
  auto initialized() const -> bool { return this->m_pipeline; }
  auto device() const -> const Device& { return *this->m_device; }
  auto graphics() const -> bool { return this->m_render_pass; }
  auto shader() const -> const Shader& { return *this->m_shader; }
  auto pipeline() const -> vk::Pipeline { return this->m_pipeline; }
  auto layout() const -> vk::PipelineLayout { return this->m_layout; }
  auto bind_point() const -> vk::PipelineBindPoint {return this->graphics() ? vk::PipelineBindPoint::eGraphics : vk::PipelineBindPoint::eCompute;}
  auto valid() const  -> bool {return this->m_pipeline;}
 private:
  using Viewports = std::vector<vk::Viewport>;
  using Scissors = std::vector<vk::Rect2D>;

  RenderPass* m_render_pass;
  Scissors m_scissors;
  Viewports m_viewports;
  DescriptorPool m_pool;
  Device* m_device;
  std::unique_ptr<Shader> m_shader;
  vk::Pipeline m_pipeline;
  vk::PipelineLayout m_layout;
  vk::PipelineCache m_cache;
  vk::ShaderStageFlags m_push_constant_flags;
  unsigned m_push_constant_size;
  bool m_depth_test;

  vk::PipelineViewportStateCreateInfo m_viewport_info;
  vk::PipelineColorBlendStateCreateInfo m_color_blend_info;
  vk::PipelineRasterizationStateCreateInfo m_rasterization_info;
  vk::PipelineInputAssemblyStateCreateInfo m_assembly_info;
  vk::PipelineMultisampleStateCreateInfo m_multisample_info;
  vk::PipelineDepthStencilStateCreateInfo m_depth_stencil_info;
  vk::SampleMask m_sample_mask;
  std::vector<vk::PipelineColorBlendAttachmentState> m_color_blend_attachments;

  inline auto createLayout() -> void;
  inline auto createPipeline() -> void;
  inline auto addViewport(const gfx::Viewport& viewport) -> void;
  inline auto parse(const gfx::PipelineInfo& info) -> void;
  inline auto init_params() -> void;
};
}  // namespace vulkan
}  // namespace luna