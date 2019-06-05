#include "StaticMeshLoader.hpp"
#include "Core/Convert.hpp"
#include "Debug/Log.hpp"
#include "Render/MaterialManager.hpp"
#include "Core/Random.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <fstream>
#include <sstream>

namespace gfx
{

void StaticMeshLoader::loadMesh(StaticMesh& mesh, const aiScene& scene, const aiMesh& inMesh)
{
    StaticMesh::Entry entry;

    for (u32 i = 0; i < inMesh.mNumVertices; i++)
    {
        Vertex vert;
        vert.pos = core::conv::toGlm(inMesh.mVertices[i]);
        vert.normal = core::conv::toGlm(inMesh.mNormals[i]);
        vert.tan = core::conv::toGlm(inMesh.mTangents[i]);
        vert.bitan = core::conv::toGlm(inMesh.mBitangents[i]);

        if (inMesh.GetNumColorChannels() > 0)
            vert.color = core::conv::toGlm(inMesh.mColors[0][i]);
        else
            vert.color = vec4(1);

        if (inMesh.GetNumUVChannels() > 0)
            vert.uv = {inMesh.mTextureCoords[0][i].x, inMesh.mTextureCoords[0][i].y};
        else
            vert.uv = vec2(0);

        entry.vertices.push_back(vert);
    }

    printf("verts.size %d\n", entry.vertices.size());

    u32 materialIndex = inMesh.mMaterialIndex;

    auto material = scene.mMaterials[materialIndex];

    aiString name;
    material->Get(AI_MATKEY_NAME, name);

    entry.material = g_MatMgr.getMaterial(name.C_Str());

    if (!entry.material)
        Log::info("in %s\n", mesh.name.c_str());

    for (u32 i = 0; i < inMesh.mNumFaces; i++)
    {
        aiFace face = inMesh.mFaces[i];
        assert(face.mNumIndices == 3);

        entry.indices.push_back(face.mIndices[0]);
        entry.indices.push_back(face.mIndices[1]);
        entry.indices.push_back(face.mIndices[2]);
    }

    mesh.entries.push_back(entry);
}

void StaticMeshLoader::loadClothMesh(StaticMesh& mesh, const aiScene& scene, const aiMesh& inMesh)
{
    StaticMesh::Entry entry;

    std::vector<f32> hashes;
    std::vector<i32> redirect(inMesh.mNumVertices);

    for (u32 i = 0; i < inMesh.mNumVertices; i++)
    {
        Vertex vert;
        vert.pos = core::conv::toGlm(inMesh.mVertices[i]);
        vert.normal = core::conv::toGlm(inMesh.mNormals[i]);
        vert.tan = core::conv::toGlm(inMesh.mTangents[i]);
        vert.bitan = core::conv::toGlm(inMesh.mBitangents[i]);

        if (inMesh.GetNumColorChannels() > 0)
            vert.color = core::conv::toGlm(inMesh.mColors[0][i]);
        else
            vert.color = vec4(1);

        if (inMesh.GetNumUVChannels() > 0)
            vert.uv = {inMesh.mTextureCoords[0][i].x, inMesh.mTextureCoords[0][i].y};
        else
            vert.uv = vec2(0);

        bool isUnique = true;

        for (auto j = 0; j < hashes.size(); j++)
        {
            f32& storedHash = hashes[j];

            f32 vertHash = vert.getHash();

            if (vertHash > storedHash - EPSILON && vertHash < storedHash + EPSILON)
            {
                redirect[i] = j;
                isUnique = false;
                break;
            }
        }

        if (isUnique)
        {
            entry.vertices.push_back(vert);
            hashes.push_back(vert.getHash());
            redirect[i] = entry.vertices.size()-1;
        }
    }

    u32 materialIndex = inMesh.mMaterialIndex;

    auto material = scene.mMaterials[materialIndex];

    aiString name;
    material->Get(AI_MATKEY_NAME, name);

    entry.material = g_MatMgr.getMaterial(name.C_Str());

    if (!entry.material)
        Log::info("in %s\n", mesh.name.c_str());

    for (u32 i = 0; i < inMesh.mNumFaces; i++)
    {
        aiFace face = inMesh.mFaces[i];
        assert(face.mNumIndices == 3);

        entry.indices.push_back(redirect[face.mIndices[0]]);
        entry.indices.push_back(redirect[face.mIndices[1]]);
        entry.indices.push_back(redirect[face.mIndices[2]]);
    }

    mesh.entries.push_back(entry);
}

void StaticMeshLoader::setupBuffers(StaticMesh::Entry& ent)
{
    ent.vao.init();
    ent.vao.setIndexNumber(ent.indices.size());
    ent.vao.bind();

    ent.dbo.init(GL_ARRAY_BUFFER);
    ent.dbo.bind();
    ent.dbo.setData(sizeof(Vertex) * ent.vertices.size(), &ent.vertices[0], GL_STATIC_DRAW);

    ent.vao.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    ent.vao.vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, normal));
    ent.vao.vertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, color));
    ent.vao.vertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, uv));
    ent.vao.vertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, tan));
    ent.vao.vertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, bitan));

    ent.ibo.init(GL_ELEMENT_ARRAY_BUFFER);
    ent.ibo.bind();
    ent.ibo.setData(sizeof(GLuint) * ent.indices.size(), &ent.indices[0], GL_STATIC_DRAW);

    ent.vao.unbind();
    ent.dbo.unbind();
    ent.ibo.unbind();
}

void StaticMeshLoader::loadFromFile(StaticMesh& mesh, const std::string& path, bool cloth)
{
    Assimp::Importer Importer;
    const aiScene* scene = Importer.ReadFile(path.c_str(),
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices
    );

    if (!scene)
    {
        Log::error("Could not load StaticMesh %s\n", path.c_str());
        return;
    }

    mesh.name = path;

    for (u32 i = 0; i < scene->mNumMeshes; i++)
    {
        if (!cloth)
            loadMesh(mesh, *scene, *scene->mMeshes[i]);
        else
            loadClothMesh(mesh, *scene, *scene->mMeshes[i]);
    }

    for (auto& ent : mesh.entries)
        setupBuffers(ent);
}

}
