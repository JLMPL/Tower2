#include "Geometry.hpp"
#include "Core/Convert.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

namespace gfx
{

LOCAL std::vector<i32> m_redirect;

LOCAL i8 addJointsToSkeleton(Mesh& mesh, const aiNode& node)
{
    // i8 jointIndex = mesh.skeleton.joints.size();

    // anim::Joint joint;
    // joint.name = node.mName.C_Str();

    // mesh.skeleton.joints.push_back(joint);
    // mesh.skeleton.joints.back().index = mesh.skeleton.joints.size()-1;

    // for (u32 i = 0; i < node.mNumChildren; i++)
    // {
    //     i8 childJointIndex = addJointsToSkeleton(mesh, *node.mChildren[i]);
    //     // mesh.skeleton.joints[jointIndex].children.push_back(childJointIndex);
    // }

    // return jointIndex;
}

LOCAL void addMeshesAndJoints(Mesh& mesh, const aiScene& scene, bool cloth)
{
    for (u32 i = 0; i < scene.mNumMeshes; i++)
    {
        aiMesh* inMesh = scene.mMeshes[i];

        Mesh::Entry entry;

        std::vector<f32> hashes;
        m_redirect.resize(inMesh->mNumVertices);

        for (u32 i = 0; i < inMesh->mNumVertices; i++)
        {
            Vertex vert;
            vert.pos = core::conv::toGlm(inMesh->mVertices[i]);
            vert.normal = core::conv::toGlm(inMesh->mNormals[i]);
            vert.tan = core::conv::toGlm(inMesh->mTangents[i]);
            vert.bitan = core::conv::toGlm(inMesh->mBitangents[i]);

            if (inMesh->GetNumColorChannels() > 0)
                vert.color = core::conv::toGlm(inMesh->mColors[0][i]);
            else
                vert.color = vec4(1);

            if (inMesh->GetNumUVChannels() > 0)
                vert.uv = {inMesh->mTextureCoords[0][i].x, inMesh->mTextureCoords[0][i].y};
            else
                vert.uv = vec2(0);

            if (!cloth)
                entry.vertices.push_back(vert);
            else
            {
                bool isUnique = true;

                for (auto j = 0; j < hashes.size(); j++)
                {
                    f32& storedHash = hashes[j];

                    f32 vertHash = getVertexHash(vert);

                    if (vertHash == storedHash)
                    {
                        m_redirect[i] = j;
                        isUnique = false;
                        break;
                    }
                }

                if (isUnique)
                {
                    entry.vertices.push_back(vert);
                    hashes.push_back(getVertexHash(vert));
                    m_redirect[i] = entry.vertices.size()-1;
                }
            }
        }

        entry.weightsData.resize(entry.vertices.size());

        u32 materialIndex = inMesh->mMaterialIndex;

        auto material = scene.mMaterials[materialIndex];

        aiString name;
        material->Get(AI_MATKEY_NAME, name);

        printf("skinned material: %s\n", name.C_Str());

        entry.material = MaterialCache.load<MaterialLoader>(entt::hashed_string(name.C_Str()), name.C_Str());

        for (u32 i = 0; i < inMesh->mNumFaces; i++)
        {
            aiFace face = inMesh->mFaces[i];
            assert(face.mNumIndices == 3);

            if (cloth)
            {
                entry.indices.push_back(m_redirect[face.mIndices[0]]);
                entry.indices.push_back(m_redirect[face.mIndices[1]]);
                entry.indices.push_back(m_redirect[face.mIndices[2]]);
            }
            else
            {
                entry.indices.push_back(face.mIndices[0]);
                entry.indices.push_back(face.mIndices[1]);
                entry.indices.push_back(face.mIndices[2]);
            }
        }

        mesh.entries.push_back(entry);

        // for (u32 i = 0; i < inMesh->mNumBones; i++)
        // {
        //     anim::Joint* joint = getSkeletonJoint(&mesh.skeleton, inMesh->mBones[i]->mName.C_Str());
        //     joint->offsetMatrix = core::conv::toGlm(inMesh->mBones[i]->mOffsetMatrix);
        // }
    }
}

LOCAL void addVertexWeightData(VertexWeightData& vwd, const i32& id, f32 weight)
{
    for (u32 i = 0; i < 4; i++)
    {
        if (vwd.weights[i] == 0.f)
        {
            vwd.joints[i] = id;
            vwd.weights[i] = weight;
            return;
        }
    }
}

LOCAL void doTheShitWithWeights(const anim::Skeleton& skeleton, const aiMesh& inMesh, Mesh::Entry& entry, bool cloth)
{
    for (u32 i = 0; i < skeleton.joints.size(); i++)
    {
        auto& joint = skeleton.joints[i];
        const aiBone* bone = nullptr;

        for (u32 i = 0; i < inMesh.mNumBones; i++)
        {
            if (std::string(inMesh.mBones[i]->mName.C_Str()) == std::string(joint.name.c_str()))
            {
                bone = inMesh.mBones[i];
                break;
            }
        }

        if (bone)
        for (u32 j = 0; j < bone->mNumWeights; j++)
        {
            u32 vertexIndex = bone->mWeights[j].mVertexId;
            if (cloth)
                addVertexWeightData(entry.weightsData[m_redirect[vertexIndex]], i, bone->mWeights[j].mWeight);
            else
                addVertexWeightData(entry.weightsData[vertexIndex], i, bone->mWeights[j].mWeight);
        }
    }
}

LOCAL void genBufferObjects(Mesh::Entry& entry)
{
    createVertexArray(entry.vao);
    entry.vao.numIndices = entry.indices.size();
    bindVertexArray(entry.vao);

    entry.dbo.init(GL_ARRAY_BUFFER);
    entry.dbo.bind();
    entry.dbo.setData(sizeof(Vertex) * entry.vertices.size(), &entry.vertices[0], GL_STATIC_DRAW);

    setVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    setVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, normal));
    setVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, color));
    setVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, uv));
    setVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, tan));
    setVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, bitan));

    entry.wbo.init(GL_ARRAY_BUFFER);
    entry.wbo.bind();
    entry.wbo.setData(sizeof(VertexWeightData) * entry.weightsData.size(), &entry.weightsData[0], GL_STATIC_DRAW);
    GL(glEnableVertexAttribArray(4));
    GL(glVertexAttribIPointer(4, 4, GL_INT, sizeof(VertexWeightData), 0));
    setVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexWeightData), (const GLvoid*)(offsetof(VertexWeightData, weights)));

    entry.ibo.init(GL_ELEMENT_ARRAY_BUFFER);
    entry.ibo.bind();
    entry.ibo.setData(sizeof(GLuint) * entry.indices.size(), &entry.indices[0], GL_STATIC_DRAW);

    unbindVertexArray(entry.vao);
}

void loadSkinnedMeshFromFile(Mesh& mesh, const anim::Skeleton& skeleton, const std::string& path, bool cloth)
{
    Assimp::Importer Importer;
    const aiScene* scene = Importer.ReadFile(path.c_str(),
        aiProcess_LimitBoneWeights |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices
    );

    if (!scene)
    {
        Log::error("Could not load Mesh %s\n", path.c_str());
        return;
    }

    mesh.name = path;

    // addJointsToSkeleton(mesh, *scene->mRootNode);
    addMeshesAndJoints(mesh, *scene, cloth);

    for (u32 i = 0; i < scene->mNumMeshes; i++)
        doTheShitWithWeights(skeleton, *scene->mMeshes[i], mesh.entries[i], cloth);

    for (auto& ent : mesh.entries)
        genBufferObjects(ent);
}

}
