#pragma once

#include <list>
#include <stdarg.h>
#include "EventObject.h"
#include "Vecteur2.h"
#include "Control.h"

class Window : public EventObject{

private:
	SDL_Window* window;
	SDL_GLContext windowContext;

	bool activeWindow;
	Vecteur2ui size;

	std::map<const char*, Control*> controls;

public:
	
	Window(int w, int h, const char* name) {

		window = SDL_CreateWindow(name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_OPENGL | SDL_RENDERER_PRESENTVSYNC);
		windowContext = SDL_GL_CreateContext(window);
		activeWindow = true;
		size = Vecteur2ui(w, h);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

		glClearColor(255, 25, 255, 255);

		EventManager::getInstance().addObject(this);
	}

	~Window() {

		SDL_DestroyWindow(window);
		SDL_GL_DeleteContext(windowContext);

		EventManager::getInstance().removeObject(this);
	}

	void addControl(const char* name, Control* control) {

		if (controls.find(name) == controls.end())
			controls[name] = control;
	}

	void removeControl(const char* name) {

		if (controls.find(name) != controls.end())
			controls.erase(name);
	}

	Control* getControl(const char* name) {
		if (controls.find(name) != controls.end())
			return controls[name];

		return nullptr;
	}

	void clearControls() {

		auto it = controls.begin();

		for (int i = controls.size(); i > 0; --i) {
			delete (*it).second;
			controls.erase(it);
			it = controls.begin();
		}
	}

	// true open
	// false close
	bool windowState() {
		return activeWindow;
	}

	void closeWindow() {
		activeWindow = false;
	}

	void clearWindow() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void renderWindow() {
		SDL_GL_SwapWindow(window);
	}

	void setWindowSize(Vecteur2ui size) {

		this->size = size;
		SDL_SetWindowSize(window, size.x, size.y);
		glViewport(0, 0, size.x, size.y);
	}

	Vecteur2ui getSize() {

		return size;
	}

	bool reactToEvent(SDL_Event* event, bool newEvent) {

		if (newEvent) {
			switch (event->type) {
				case SDL_QUIT:
					activeWindow = false;
					return true;
					break;
			}
		}
		return false;
	}
};
