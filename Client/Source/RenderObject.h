#pragma once
#include "Vecteur2.h"

class RenderObject {

protected:

	Vecteur2f position;
	Vecteur2f scale;

public:

	RenderObject() {

		scale = Vecteur2f(1, 1);
	}

	virtual void show(int w, int h) = 0;


	Vecteur2f getPosition() {
		return position;
	}

	virtual void setPosition(Vecteur2f pos) {

		position = pos;
	}

	Vecteur2f getScale() {
		return scale;
	}

	void setScale(Vecteur2f scale) {

		this->scale = scale;
	}
};