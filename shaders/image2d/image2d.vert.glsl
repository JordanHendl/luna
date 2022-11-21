#version 450
#extension GL_GOOGLE_include_directive    : enable
#extension GL_ARB_separate_shader_objects : enable
#include "luna_vertex.glsl"

     
flat layout( location = 1 ) out uint texture_index  ;

layout( binding = 1 ) uniform ProjectionMatrix
{
  mat4 proj ; 
} proj_mat;

layout( binding = 2 ) buffer Transformations
{
  mat4 transforms[] ;
} transform;

layout( binding = 3 ) buffer TextureIds
{
  uint texture_ids[] ;
} tex_ids;

void main()
{
  mat4 model          ;
  mat4 projection     ;
  vec4 position       ;
  vec2 tex            ;

  position      = vec4( luna_position.x, luna_position.y, 0.0, 1.0 ) ;
  model         = transform.transforms [ gl_InstanceIndex ]          ;
  texture_index = tex_ids.texture_ids[ gl_InstanceIndex ]            ;
  projection    = proj_mat.proj                                      ;

  luna_frag_coords   = vec2( luna_tex_coords.x, luna_tex_coords.y );
  gl_Position = projection * model * position ;  
}