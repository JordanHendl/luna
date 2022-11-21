#version 450
#extension GL_GOOGLE_include_directive    : enable
#extension GL_ARB_separate_shader_objects : enable
#include "luna_frag.glsl" 

flat layout( location = 1 ) in  uint index;

void main()
{  
  vec4 color ;
   
  color = texture( textures[ index ], luna_frag_coords ) ;
  if( color.a < 0.1 ) discard ;
  luna_frag_color = color ;
}