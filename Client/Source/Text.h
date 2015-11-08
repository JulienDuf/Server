#pragma once
#include "TextTexture.h"
#include "RenderObject.h"

class Text : public RenderObject{

private:
	
	TextTexture* text;

public:

	Text(TextTexture* text, Vecteur2f position) {

		this->text = text;
		this->position = position;
	}

	void show(int w, int h) {

		glDisable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, w, h, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);

		glPushMatrix();
		glLoadIdentity();
		glScaled(scale.x, scale.y, 1);
		glEnable(GL_TEXTURE_2D);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, text->getID());

		glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glVertex2f(position.x, position.y);  //1

		glTexCoord2f(1, 0);
		glVertex2f(text->getSurface()->w + position.x, position.y); //2

		glTexCoord2f(1, 1);
		glVertex2f(text->getSurface()->w + position.x, text->getSurface()->h + position.y); //3

		glTexCoord2f(0, 1);
		glVertex2f(position.x, text->getSurface()->h + position.y); //4
		glEnd();

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}

	TextTexture* getTexture() {
		return text;
	}
};