#ifndef LUNA_VERTEX_GLSL
#define LUNA_VERTEX_GLSL

layout(location = 0) in vec3 luna_position;
layout(location = 1) in vec3 luna_normals;
layout(location = 2) in vec2 luna_tex_coords;
layout(location = 3) in vec3 luna_tangents;
layout(location = 4) in ivec4 luna_bone_ids;
layout(location = 5) in vec4 luna_bone_weights;

layout(location = 0) out vec2 luna_frag_coords;
#endif