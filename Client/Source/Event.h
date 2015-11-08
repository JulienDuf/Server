#pragma once
#include <vector>

enum mouse_state {PRESSED = 1, RELEASED = 2};
enum mouse_button { LEFT_CLICK = 1, MIDDLE_CLICK = 2, RIGHT_CLICK = 3 };

enum key_event  {KEY_A = 4, KEY_B = 5, KEY_C = 6, KEY_D = 7, KEY_E = 8, KEY_F = 9, KEY_G = 10, KEY_H = 11, KEY_I = 12, KEY_J = 13, KEY_K = 14, KEY_L = 15, KEY_M = 16, KEY_N = 17, KEY_O = 18, KEY_P = 19, KEY_Q = 20, KEY_R = 21, KEY_S = 22, KEY_T = 23, KEY_U = 24, KEY_V = 25, KEY_W = 26, KEY_X = 27, KEY_Y = 28, KEY_Z = 29, KEY_1 = 30, KEY_2 = 31, KEY_3 = 32, KEY_4 = 33, KEY_5 = 34, KEY_6 = 35, KEY_7 = 36, KEY_8 = 37, KEY_9 = 38, KEY_0 = 39, KEY_ENTER = 40, KEY_ESCAPE = 41, KEY_BACKSPACE = 42, KEY_TAB = 43, KEY_SPACE = 44, KEY_MINUS = 45, KEY_EQUALS = 46, KEY_LEFTBRACKET = 47, KEY_RIGHTBRACKET = 48, KEY_BACKSLASH = 49, KEY_COMMA = 54, KEY_PERIOD = 55, KEY_SLASH = 56, KEY_CAPSLOCK = 57, KEY_F1 = 58, KEY_F2 = 59, KEY_F3 = 60, KEY_F4 = 61, KEY_F5 = 62, KEY_F6 = 63, KEY_F7 = 64, KEY_F8 = 65, KEY_F9 = 66, KEY_F10 = 67, KEY_F11 = 68, KEY_F12 = 69, KEY_PRINTSCREEN = 70, KEY_SCROLLLOCK = 71, KEY_PAUSE = 72, KEY_INSERT = 73, KEY_HOME = 74, KEY_PAGEUP = 75, KEY_DELETE = 76, KEY_END = 77, KEY_PAGEDOWN = 78, KEY_RIGHT = 79, KEY_LEFT = 80, KEY_DOWN = 81, KEY_UP = 82, KEY_NUMLOCKCLEAR = 83, KEY_NUMBERPAD_DIVIDE = 84, KEY_NUMBERPAD_MULTIPLY = 85, KEY_NUMBERPAD_MINUS = 86, KEY_NUMBERPAD_PLUS = 87, KEY_NUMBERPAD_ENTER = 88, KEY_NUMBERPAD_1 = 89, KEY_NUMBERPAD_2 = 90, KEY_NUMBERPAD_3 = 91, KEY_NUMBERPAD_4 = 92, KEY_NUMBERPAD_5 = 93, KEY_NUMBERPAD_6 = 94, KEY_NUMBERPAD_7 = 95, KEY_NUMBERPAD_8 = 96, KEY_NUMBERPAD_9 = 97, KEY_NUMBERPAD_0 = 98, KEY_NUMBERPAD_PERIOD = 99};

enum controller_button { BUTTON_A, BUTTON_B, BUTTON_X, BUTTON_Y };

struct Keybord {

	std::vector<Uint32> keysPressed;
	Uint32 lastKeyPressed;
	Uint32 lastKeyUp;
};

struct Mouse {

	Uint32 button;
	Uint32 state;

	int lastX = 0;
	int lastY = 0;
	int xrel = 0;
	int yrel = 0;
};

struct Controller {

};

struct Event {

	static Keybord keybord;
	static Mouse mouse;

	static void update(SDL_Event* event) {

		switch (event->type) {

		case SDL_KEYDOWN:
			keybord.lastKeyPressed = event->key.keysym.scancode;
			if (std::find(keybord.keysPressed.begin(), keybord.keysPressed.end(), keybord.lastKeyPressed) == keybord.keysPressed.end())
				keybord.keysPressed.push_back(keybord.lastKeyPressed);
			break;
		case SDL_KEYUP:
			keybord.lastKeyUp = event->key.keysym.scancode;
			if (keybord.keysPressed.size() > 0)
				keybord.keysPressed.erase(std::find(keybord.keysPressed.begin(), keybord.keysPressed.end(), keybord.lastKeyUp));
			break;
		case SDL_MOUSEBUTTONDOWN:
			mouse.state = PRESSED;
			mouse.button = event->button.button;
			break;
		case SDL_MOUSEBUTTONUP:
			mouse.state = RELEASED;
			mouse.button = event->button.button;
			break;
		}

		if (mouse.lastX == 0) {

			mouse.lastX = event->button.x;
			mouse.lastY = event->button.y;
		}

		else {

			mouse.xrel = mouse.lastX - event->button.x;
			mouse.yrel = mouse.lastY - event->button.y;

			mouse.lastX = event->button.x;
			mouse.lastY = event->button.y;
		}
	}
};

Keybord Event::keybord;
Mouse Event::mouse;
