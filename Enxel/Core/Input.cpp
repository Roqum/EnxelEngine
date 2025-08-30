#include "Input.h"

namespace Enxel
{

    bool Input::IsKeyDown(KeyCode keycode)
    {
        const bool* state = SDL_GetKeyboardState(nullptr);
        return state[static_cast<SDL_Scancode>(keycode)] != 0;
    }

    bool Input::IsMouseButtonDown(MouseButton button)
    {
        return (SDL_GetMouseState(nullptr, nullptr) & static_cast<Uint32>(button)) != 0;
    }

    glm::vec2 Input::GetMousePosition()
    {
        float x, y;
        SDL_GetMouseState(&x, &y);
        return glm::vec2(x, y);
    }
}