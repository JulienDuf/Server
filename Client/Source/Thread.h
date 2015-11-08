#pragma once

class Thread {

private:

	SDL_Thread* thread;
	Uint32 ID;
	const char* name;

public:

	Thread(int function(void*), const char* name, void* parameter) {

		this->name = name;
		thread = SDL_CreateThread(function, name, parameter);
		ID = SDL_GetThreadID(thread);
	}

	Uint32 getID() {
		return ID;
	}

	const char* getName() {
		return name;
	}

	bool isActiveThread() {

		return ID == SDL_ThreadID();
	}
};