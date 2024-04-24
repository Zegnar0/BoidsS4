#include "InputManager.hpp"

InputManager::InputManager(p6::Context& ctx, FreeflyCamera& camera)
    : _ctx(ctx), _camera(camera)
{
}

void InputManager::update()
{
    if (_ctx.key_is_pressed(GLFW_KEY_D))
    {
        _camera.moveLeft(-0.1f);
    }
    if (_ctx.key_is_pressed(GLFW_KEY_A))
    {
        _camera.moveLeft(0.1f);
    }
    if (_ctx.key_is_pressed(GLFW_KEY_W))
    {
        _camera.moveFront(0.1f);
    }
    if (_ctx.key_is_pressed(GLFW_KEY_S))
    {
        _camera.moveFront(-0.1f);
    }

    // Handle mouse movement
    _ctx.mouse_moved = [this](p6::MouseMove event) {
        _camera.rotateLeft(-event.position.x);
        _camera.rotateUp(event.position.y);
    };
}
