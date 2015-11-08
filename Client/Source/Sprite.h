#pragma once
#include "Texture.h"
#include "Rect.h"
#include "Timer.h"
#include "RenderObject.h"

class Sprite : public RenderObject {

private:

	Texture* spriteTexture;
	Rectf sourceRect;
	Timer* spriteTimer;
	bool spriteIsActive;
	bool spriteIsLooped;

public:

	Sprite(Vecteur2f position, Rectf sourceRect, Texture* texture, int wFrames, int hFrames) {

		this->position = position;
		this->sourceRect = sourceRect;
		this->spriteTexture = texture;

		this->sourceRect.w = spriteTexture->getSurface()->w / wFrames;
		this->sourceRect.h = spriteTexture->getSurface()->h / hFrames;

		spriteTimer = nullptr;
	}

	Sprite(Vecteur2f position, Rectf sourceRect, Texture* texture, int wFrames, int hFrames, int timerTime) : Sprite(position, sourceRect, texture, wFrames, hFrames){

		spriteTimer = new Timer(timerTime);
	}

	~Sprite() {
		delete spriteTimer;
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
		glBindTexture(GL_TEXTURE_2D, spriteTexture->getID());

		Rectf rectTexture;
		rectTexture.x = sourceRect.x / spriteTexture->getSurface()->w;
		rectTexture.y = sourceRect.y / spriteTexture->getSurface()->h;
		rectTexture.w = sourceRect.w / spriteTexture->getSurface()->w;
		rectTexture.h = sourceRect.h / spriteTexture->getSurface()->h;

		glBegin(GL_QUADS);
		glTexCoord2f(rectTexture.x, rectTexture.y);
		glVertex2f(position.x, position.y);  //1

		glTexCoord2f(rectTexture.x + rectTexture.w, rectTexture.y);
		glVertex2f(sourceRect.w + position.x, position.y); //2

		glTexCoord2f(rectTexture.x + rectTexture.w, rectTexture.y + rectTexture.h);
		glVertex2f(sourceRect.w + position.x, sourceRect.h + position.y); //3

		glTexCoord2f(rectTexture.x, rectTexture.y + rectTexture.h);
		glVertex2f(position.x, sourceRect.h + position.y); //4
		glEnd();

		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

	}

	void checkFrame() {

		if (spriteTimer != nullptr) {
			if (spriteTimer->isFinished() && spriteIsActive) {

				int wFrame, hFrame; 
				
				if (sourceRect.x > 0)
					wFrame = spriteTexture->getSurface()->w / sourceRect.w;
				else
					wFrame = 0;
				if (sourceRect.y > 0)
					hFrame = spriteTexture->getSurface()->h / sourceRect.h;
				else
					hFrame = 0;

				if (sourceRect.x + sourceRect.w != spriteTexture->getSurface()->w)
					++wFrame;
				else if (sourceRect.y + sourceRect.h != spriteTexture->getSurface()->h)
					++hFrame;
				else {
					if (!spriteIsLooped)
						spriteIsActive = false;
					
					wFrame = hFrame = 0;						
				}
				changeFrame(wFrame, hFrame);
				spriteTimer->restart();
			}
		}
	}

	void changeFrame(int wFrame, int hFrame) {

		if (sourceRect.w * wFrame <= spriteTexture->getSurface()->w)
			sourceRect.x = sourceRect.w * wFrame;
		if(sourceRect.h * hFrame <= spriteTexture->getSurface()->h)
			sourceRect.y = sourceRect.h * hFrame;
		else
			sourceRect.y = 0;
	}

	float getWidth() {
		return sourceRect.w;
	}
	
	float getHeight() {
		return sourceRect.h;
	}

	Texture* getTexture() {
		return spriteTexture;
	}

	int getWFrame() {

		return spriteTexture->getSurface()->w / sourceRect.w;
	}

	int getHFrame() {

		return spriteTexture->getSurface()->h / sourceRect.h;
	}
};