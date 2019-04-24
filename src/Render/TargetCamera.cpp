#include "TargetCamera.hpp"
#include "Core/Config.hpp"

namespace gfx
{

TargetCamera::TargetCamera()
{
    auto display = core::g_Config.getDisplay();

    m_projection = math::perspective(
        70.0_rad,
        f32(display.width) / f32(display.height),
        0.01f,
        2000.f
    );
}

void TargetCamera::updateViewMatrix()
{
    m_view = math::lookAt(m_eye, m_center, math::vecY);
}

}
