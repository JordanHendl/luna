/*
 * Copyright (C) 2020 Jordan Hendl
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>
#include <assimp/Importer.hpp>
#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <string>

#include "lum_loader.h"

namespace luna {
inline namespace v1 {
using Mesh = ModelLoader::Mesh;
using Meshes = std::vector<Mesh>;

static const unsigned long long MAGIC = 0x26656d696b750a;
static const unsigned VERSION = 1;

static void writeUnsigned(std::ofstream& stream, unsigned val);
static void writeMagic(std::ofstream& stream, unsigned long long magic);
static void writeBytes(std::ofstream& stream, const unsigned char* bytes,
                       unsigned size);
static void writeString(std::ofstream& stream, std::string& str);
static void writeBones(std::ofstream& stream, Mesh& mesh);
static void process(Meshes& meshes, aiNode* node, const aiScene* scene);
static Mesh process(aiMesh& mesh, const aiScene& scene);
static void process(Mesh& mesh, const aiMaterial& material, aiTextureType type);
void writeUnsigned(std::ofstream& stream, unsigned val) {
  stream.write(reinterpret_cast<char*>(&val), sizeof(unsigned));
}

void writeMagic(std::ofstream& stream, unsigned long long val) {
  stream.write(reinterpret_cast<char*>(&val), sizeof(unsigned long long));
}

void writeBytes(std::ofstream& stream, const unsigned char* bytes,
                unsigned size) {
  stream.write(reinterpret_cast<const char*>(bytes), size);
}

void writeString(std::ofstream& stream, std::string& val) {
  unsigned sz;

  sz = val.size();
  stream.write(reinterpret_cast<char*>(&sz), sizeof(unsigned));
  stream.write(reinterpret_cast<char*>(&val[0]), sz);
}

void writeBones(std::ofstream& stream, Mesh& mesh) {
  writeString(stream, mesh.name);
  writeUnsigned(stream, mesh.m_vertices.size());
  writeUnsigned(stream, mesh.m_indices.size());

  writeUnsigned(stream, mesh.material.m_diffuse.size());
  writeUnsigned(stream, mesh.material.m_specular.size());
  writeUnsigned(stream, mesh.material.m_normal.size());
  writeUnsigned(stream, mesh.material.m_height.size());

  for (auto& vert : mesh.m_vertices) {
    writeBytes(stream, reinterpret_cast<unsigned char*>(vert.position),
               sizeof(float) * 4);
    writeBytes(stream, reinterpret_cast<unsigned char*>(vert.normals),
               sizeof(float) * 4);
    writeBytes(stream, reinterpret_cast<unsigned char*>(vert.weights),
               sizeof(float) * 4);
    writeBytes(stream, reinterpret_cast<unsigned char*>(vert.ids),
               sizeof(unsigned) * 4);
    writeBytes(stream, reinterpret_cast<unsigned char*>(vert.uvs),
               sizeof(float) * 2);
  }

  writeBytes(stream, reinterpret_cast<unsigned char*>(mesh.m_indices.data()),
             sizeof(unsigned) * mesh.m_indices.size());

  for (auto& str : mesh.material.m_diffuse) writeString(stream, str);
  for (auto& str : mesh.material.m_specular) writeString(stream, str);
  for (auto& str : mesh.material.m_normal) writeString(stream, str);
  for (auto& str : mesh.material.m_height) writeString(stream, str);
}

void process(Meshes& meshes, aiNode& node, const aiScene& scene) {
  for (unsigned index = 0; index < node.mNumMeshes; index++) {
    auto mesh = scene.mMeshes[node.mMeshes[index]];
    if (mesh) {
      meshes.push_back(process(*mesh, scene));
    }
  }

  for (unsigned index = 0; index < node.mNumChildren; index++) {
    process(meshes, *node.mChildren[index], scene);
  }
}

Mesh process(aiMesh& mesh, const aiScene& scene) {
  Mesh output;
  unsigned index;

  output.m_vertices.resize(mesh.mNumVertices);
  output.m_indices.reserve(mesh.mNumFaces * 3);

  output.name = mesh.mName.C_Str();

  for (index = 0; index < mesh.mNumVertices; index++) {
    output.m_vertices[index].position[0] = mesh.mVertices[index].x;
    output.m_vertices[index].position[1] = mesh.mVertices[index].y;
    output.m_vertices[index].position[2] = mesh.mVertices[index].z;
    output.m_vertices[index].position[3] = 1.0f;

    if (mesh.mNormals) {
      output.m_vertices[index].normals[0] = mesh.mNormals[index].x;
      output.m_vertices[index].normals[1] = mesh.mNormals[index].y;
      output.m_vertices[index].normals[2] = mesh.mNormals[index].z;
      output.m_vertices[index].normals[3] = 1.0f;
    }

    if (mesh.mTextureCoords[0]) {
      output.m_vertices[index].uvs[0] = mesh.mTextureCoords[0][index].x;
      output.m_vertices[index].uvs[1] = mesh.mTextureCoords[0][index].y;
    } else {
      output.m_vertices[index].uvs[0] = 0.0f;
      output.m_vertices[index].uvs[1] = 0.0f;
    }
  }

  for (index = 0; index < mesh.mNumBones; index++) {
    auto& bone = mesh.mBones[index];

    for (unsigned weight_index = 0;
         weight_index < std::min(bone->mNumWeights, 4u); weight_index++) {
      output.m_vertices[bone->mWeights[weight_index].mVertexId]
          .weights[weight_index] = bone->mWeights[weight_index].mWeight;
      output.m_vertices[bone->mWeights[weight_index].mVertexId]
          .ids[weight_index] = index;
    }
  }

  for (index = 0; index < mesh.mNumFaces; index++) {
    auto& face = mesh.mFaces[index];

    for (unsigned id = 0; id < face.mNumIndices; id++) {
      output.m_indices.push_back(face.mIndices[id]);
    }
  }

  auto& material = *scene.mMaterials[mesh.mMaterialIndex];

  process(output, material, aiTextureType::aiTextureType_DIFFUSE);
  process(output, material, aiTextureType::aiTextureType_SPECULAR);
  process(output, material, aiTextureType::aiTextureType_NORMALS);
  process(output, material, aiTextureType::aiTextureType_HEIGHT);

  // TODO Texture loading and handling.
  return output;
}

void process(Mesh& mesh, const aiMaterial& material, aiTextureType type) {
  aiString str;

  for (unsigned index = 0; index < material.GetTextureCount(type); index++) {
    material.GetTexture(type, index, &str);

    switch (type) {
      case aiTextureType::aiTextureType_DIFFUSE:
        mesh.material.m_diffuse.push_back(str.C_Str());
        break;
      case aiTextureType::aiTextureType_SPECULAR:
        mesh.material.m_specular.push_back(str.C_Str());
        break;
      case aiTextureType::aiTextureType_NORMALS:
        mesh.material.m_normal.push_back(str.C_Str());
        break;
      case aiTextureType::aiTextureType_HEIGHT:
        mesh.material.m_height.push_back(str.C_Str());
        break;
      default:
        break;
    };
  }
}

ModelLoader::ModelLoader() {}

ModelLoader::~ModelLoader() { this->model_meshes.clear(); }

auto ModelLoader::generate(const char* path) -> std::vector<Mesh> {
  auto importer = Assimp::Importer();
  auto model_meshes = std::vector<Mesh>;
  const auto* scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_CalcTangentSpace |
                aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cout << "Error: Could not load model at " << path << ".\n";
    return false;
  }

  process(model_meshes, *scene->mRootNode, *scene);

  return model_meshes;
}

auto ModelLoader::save(const char* path) -> void {
  std::ofstream stream;

  stream.open(path, std::ios::binary);

  if (stream) {
    writeMagic(stream, MAGIC);
    writeUnsigned(stream, VERSION);

    writeUnsigned(stream, this->model_meshes.size());
    for (auto& mesh : this->model_meshes) {
      writeBones(stream, mesh);
    }
  } else {
#if defined(__unix__) || defined(_WIN32)
    constexpr const char* COLOR_END = "\x1B[m";
    constexpr const char* COLOR_RED = "\u001b[31m";
#else
    constexpr const char* COLOR_END = "";
    constexpr const char* COLOR_RED = "";
#endif

    std::cout << COLOR_RED << "Unable to open file for saving :" << path
              << COLOR_END << std::endl;
  }

  stream.close();
}

void ModelLoader::reset() { this->model_meshes.clear(); }
}  // namespace v1
}  // namespace luna