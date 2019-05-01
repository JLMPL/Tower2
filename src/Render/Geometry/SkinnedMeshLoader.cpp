#include "SkinnedMeshLoader.hpp"
#include "Core/Convert.hpp"
#include "Render/MaterialManager.hpp"

namespace gfx
{

i8 SkinnedMeshLoader::addNodesToSkeleton(SkinnedMesh& mesh, const aiNode& node)
{
    i8 jointIndex = mesh.skeleton.joints.size();

    anim::Joint joint;
    joint.name = node.mName.C_Str();

    for (u32 i = 0; i < anim::Joint::MaxChildren; i++)
        joint.children[i] = -1;

    std::string nome = joint.name.c_str();

    mesh.skeleton.joints.push_back(joint);
    mesh.skeleton.joints.back().index = mesh.skeleton.joints.size()-1;

    for (u32 i = 0; i < node.mNumChildren; i++)
    {
        i8 childJointIndex = addNodesToSkeleton(mesh, *node.mChildren[i]);
        mesh.skeleton.joints[jointIndex].children[i] = childJointIndex;
    }

    return jointIndex;
}

void SkinnedMeshLoader::addMeshesAndJoints(SkinnedMesh& mesh, const aiScene& scene)
{
    for (u32 i = 0; i < scene.mNumMeshes; i++)
    {
        aiMesh* inMesh = scene.mMeshes[i];

        SkinnedMesh::Entry entry;
        entry.weightsData.resize(inMesh->mNumVertices);

        for (u32 i = 0; i < inMesh->mNumVertices; i++)
        {
            SkinVertex vert;
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

            entry.vertices.push_back(vert);
        }

        u32 materialIndex = inMesh->mMaterialIndex;

        auto material = scene.mMaterials[materialIndex];

        aiString name;
        material->Get(AI_MATKEY_NAME, name);

        entry.material = g_MatMgr.getMaterial(name.C_Str());

        for (u32 i = 0; i < inMesh->mNumFaces; i++)
        {
            aiFace face = inMesh->mFaces[i];
            assert(face.mNumIndices == 3);

            entry.indices.push_back(face.mIndices[0]);
            entry.indices.push_back(face.mIndices[1]);
            entry.indices.push_back(face.mIndices[2]);
        }

        mesh.entries.push_back(entry);

        for (u32 i = 0; i < inMesh->mNumBones; i++)
        {
            anim::Joint* joint = mesh.skeleton.findJoint(inMesh->mBones[i]->mName.C_Str());
            joint->offsetMatrix = core::conv::toGlm(inMesh->mBones[i]->mOffsetMatrix);
        }
    }
}

void SkinnedMeshLoader::doTheShitWithWeights(SkinnedMesh& mesh, const aiMesh& inMesh, SkinnedMesh::Entry& entry)
{
    for (u32 i = 0; i < mesh.skeleton.joints.size(); i++)
    {
        auto& joint = mesh.skeleton.joints[i];
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
            entry.weightsData[vertexIndex].addData(i, bone->mWeights[j].mWeight);
        }
    }
}

void SkinnedMeshLoader::loadFromFile(SkinnedMesh& mesh, const std::string& path)
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
        Log::error("Could not load SkinnedMesh %s\n", path.c_str());
        return;
    }

    mesh.name = path;

    addNodesToSkeleton(mesh, *scene->mRootNode);
    addMeshesAndJoints(mesh, *scene);

    for (u32 i = 0; i < scene->mNumMeshes; i++)
        doTheShitWithWeights(mesh, *scene->mMeshes[i], mesh.entries[i]);

    for (auto& ent : mesh.entries)
        genBufferObjects(ent);
}

void SkinnedMeshLoader::genBufferObjects(SkinnedMesh::Entry& entry)
{
    entry.vao.init();
    entry.vao.setIndexNumber(entry.indices.size());
    entry.vao.bind();

    entry.dbo.init(GL_ARRAY_BUFFER);
    entry.dbo.bind();
    entry.dbo.setData(sizeof(SkinVertex) * entry.vertices.size(), &entry.vertices[0], GL_STATIC_DRAW);

    entry.vao.vertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), 0);
    entry.vao.vertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (const GLvoid*)offsetof(SkinVertex, normal));
    entry.vao.vertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (const GLvoid*)offsetof(SkinVertex, color));
    entry.vao.vertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (const GLvoid*)offsetof(SkinVertex, uv));
    entry.vao.vertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (const GLvoid*)offsetof(SkinVertex, tan));
    entry.vao.vertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (const GLvoid*)offsetof(SkinVertex, bitan));

    entry.wbo.init(GL_ARRAY_BUFFER);
    entry.wbo.bind();
    entry.wbo.setData(sizeof(VertexWeightData) * entry.weightsData.size(), &entry.weightsData[0], GL_STATIC_DRAW);
    GL(glEnableVertexAttribArray(4));
    GL(glVertexAttribIPointer(4, 4, GL_INT, sizeof(VertexWeightData), 0));
    entry.vao.vertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexWeightData), (const GLvoid*)(offsetof(VertexWeightData, weights)));

    entry.ibo.init(GL_ELEMENT_ARRAY_BUFFER);
    entry.ibo.bind();
    entry.ibo.setData(sizeof(GLuint) * entry.indices.size(), &entry.indices[0], GL_STATIC_DRAW);

    entry.vao.unbind();
}

}
