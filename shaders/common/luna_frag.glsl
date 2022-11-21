#ifndef LUNA_FRAG_GLSL
#define LUNA_FRAG_GLSL

layout( location = 0 ) in  vec2 luna_frag_coords ;

layout( location = 0 ) out vec4 luna_frag_color ;

layout( binding = 0 ) uniform sampler2D textures[ 1024 ] ; 
#endif