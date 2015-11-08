#pragma once

class EventObject {

public:

	virtual bool reactToEvent(SDL_Event* event, bool newEvent) = 0;
};