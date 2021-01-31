#include "Geometry.hpp"
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

LOCAL void loadMesh(Mesh& mesh, const aiScene& scene, const aiMesh& inMesh, bool isDae)
{
    Mesh::Entry entry;

    for (u32 i = 0; i < inMesh.mNumVertices; i++)
    {
        Vertex vert;
        if (isDae)
            vert.pos = vec3(math::rotate(-90.0_rad, vec3(1,0,0)) * vec4(core::conv::toGlm(inMesh.mVertices[i]), 1));
        else
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

LOCAL void loadClothMesh(Mesh& mesh, const aiScene& scene, const aiMesh& inMesh, bool isDae)
{
    Mesh::Entry entry;

    std::vector<f32> hashes;
    std::vector<i32> redirect(inMesh.mNumVertices);

    for (u32 i = 0; i < inMesh.mNumVertices; i++)
    {
        Vertex vert;
        if (isDae)
            vert.pos = vec3(math::rotate(-90.0_rad, vec3(1,0,0)) * vec4(core::conv::toGlm(inMesh.mVertices[i]), 1));
        else
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

            f32 vertHash = getVertexHash(vert);

            if (vertHash == storedHash)
            {
                redirect[i] = j;
                isUnique = false;
                break;
            }
        }

        if (isUnique)
        {
            entry.vertices.push_back(vert);
            hashes.push_back(getVertexHash(vert));
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

LOCAL void setupBuffers(Mesh::Entry& ent)
{
    createVertexArray(ent.vao);
    ent.vao.numIndices = ent.indices.size();
    bindVertexArray(ent.vao);

    ent.dbo.init(GL_ARRAY_BUFFER);
    ent.dbo.bind();
    ent.dbo.setData(sizeof(Vertex) * ent.vertices.size(), &ent.vertices[0], GL_STATIC_DRAW);

    setVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    setVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, normal));
    setVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, color));
    setVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, uv));
    setVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, tan));
    setVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, bitan));

    ent.ibo.init(GL_ELEMENT_ARRAY_BUFFER);
    ent.ibo.bind();
    ent.ibo.setData(sizeof(GLuint) * ent.indices.size(), &ent.indices[0], GL_STATIC_DRAW);

    unbindVertexArray(ent.vao);
    ent.dbo.unbind();
    ent.ibo.unbind();
}

void loadStaticMeshFromFile(Mesh& mesh, const std::string& path, bool cloth)
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

    bool isDae = (path.substr(path.size()-3, 3) == "dae") ? true : false;

    for (u32 i = 0; i < scene->mNumMeshes; i++)
    {
        if (!cloth)
            loadMesh(mesh, *scene, *scene->mMeshes[i], isDae);
        else
            loadClothMesh(mesh, *scene, *scene->mMeshes[i], isDae);
    }

    for (auto& ent : mesh.entries)
        setupBuffers(ent);
}

}
