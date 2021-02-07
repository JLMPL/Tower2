// #pragma once
// #include "RenderEntity.hpp"
// #include "Render/Geometry/Geometry.hpp"
// #include "Render/VertexArray.hpp"
// #include "Render/ArrayBuffer.hpp"

// namespace phys
// {
//     class Cloth;
// }

// namespace gfx
// {
//     class BasePass;
//     class Material;
// }

// class RenderCloth : public RenderEntity
// {
// public:
//     using Ptr = std::unique_ptr<RenderCloth>;

//     RenderCloth() = default;
//     RenderCloth(phys::Cloth* cloth);
//     ~RenderCloth() = default;

//     void updateGeometry();
//     void render();

//     const gfx::Material* getMaterial() const;

//     Type getType() const override final;

// private:
//     void updatePositions();
//     void updateNormals();
//     void updateBuffers();

// private:
//     phys::Cloth* m_cloth = nullptr;
//     gfx::Mesh* m_mesh = nullptr;

//     std::vector<gfx::Vertex> m_vertices;

//     gfx::VertexArray m_vao;
//     gfx::ArrayBuffer m_dbo;
//     gfx::ArrayBuffer m_ibo;

//     friend class gfx::BasePass;
// };
