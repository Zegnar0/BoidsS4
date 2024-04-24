#pragma once

#include "p6/p6.h"
#include "src-common/glimac/Camera/FreeflyCamera.hpp"

class InputManager {
public:
    InputManager(p6::Context& ctx, FreeflyCamera& camera);

    void update();

private:
    p6::Context&   _ctx;
    FreeflyCamera& _camera;
};
