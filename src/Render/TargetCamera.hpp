#pragma once
#include "Camera.hpp"

namespace gfx
{

class TargetCamera : public Camera
{
public:
    TargetCamera();
    void updateViewMatrix() override final;
};

}
