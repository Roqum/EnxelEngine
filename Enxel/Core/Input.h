#pragma once

#include "glm/glm.hpp"
#include <SDL3/SDL.h>

namespace Enxel
{
	enum KeyCode {
		
        A = SDL_SCANCODE_A,
        B = SDL_SCANCODE_B,
        C = SDL_SCANCODE_C,
        D = SDL_SCANCODE_D,
        E = SDL_SCANCODE_E,
        F = SDL_SCANCODE_F,
        G = SDL_SCANCODE_G,
        H = SDL_SCANCODE_H,
        I = SDL_SCANCODE_I,
        J = SDL_SCANCODE_J,
        K = SDL_SCANCODE_K,
        L = SDL_SCANCODE_L,
        M = SDL_SCANCODE_M,
        N = SDL_SCANCODE_N,
        O = SDL_SCANCODE_O,
        P = SDL_SCANCODE_P,
        Q = SDL_SCANCODE_Q,
        R = SDL_SCANCODE_R,
        S = SDL_SCANCODE_S,
        T = SDL_SCANCODE_T,
        U = SDL_SCANCODE_U,
        V = SDL_SCANCODE_V,
        W = SDL_SCANCODE_W,
        X = SDL_SCANCODE_X,
        Y = SDL_SCANCODE_Y,
        Z = SDL_SCANCODE_Z,

        Key_1 = SDL_SCANCODE_1,
        Key_2 = SDL_SCANCODE_2,
        Key_3 = SDL_SCANCODE_3,
        Key_4 = SDL_SCANCODE_4,
        Key_5 = SDL_SCANCODE_5,
        Key_6 = SDL_SCANCODE_6,
        Key_7 = SDL_SCANCODE_7,
        Key_8 = SDL_SCANCODE_8,
        Key_9 = SDL_SCANCODE_9,
        Key_0 = SDL_SCANCODE_0,

        RETURN    = SDL_SCANCODE_RETURN,
        ESCAPE    = SDL_SCANCODE_ESCAPE,
        BACKSPACE = SDL_SCANCODE_BACKSPACE,
        TAB       = SDL_SCANCODE_TAB,
        SPACE     = SDL_SCANCODE_SPACE
	};

    enum MouseButton : Uint32 {
		Left = SDL_BUTTON_LMASK,
		Middle = SDL_BUTTON_MMASK,
		Right = SDL_BUTTON_RMASK,
        X1 = SDL_BUTTON_X1MASK,
        X2 = SDL_BUTTON_X2MASK
    };

    class Input
	{
	public:
		static bool IsKeyDown(KeyCode keycode);
		static bool IsMouseButtonDown(MouseButton button);

		static glm::vec2 GetMousePosition();

		//static void SetCursorMode(CursorMode mode);
	};
}