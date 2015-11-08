#pragma once
#include "Sprite.h"
#include "TextTexture.h"
#include "Font.h"
#include "Control.h"

enum button_state { DEFAULT, OVERFLOWN, CLICKED, INACTIVE };

class Button : public Control{

private:

	Sprite* buttonTextures;
	button_state buttonState;
	Rectf buttonRect;

public:

	Button() {
		buttonTextures = nullptr;

		EventManager::getInstance().addObject(this);
	}

	Button(Sprite* textures) : Button() {

		buttonTextures = textures;
		this->position = textures->getPosition();
		buttonState = DEFAULT;
		buttonRect = Rectf(this->position.x, this->position.y, this->buttonTextures->getWidth(), this->buttonTextures->getHeight());

		RenderManager::getInstance().addObject(buttonTextures);
	}
	
	Button(Sprite* textures, const char* textOnButton, Font* font, SDL_Color color) : Button() {

		buttonTextures = textures;
		this->position = textures->getPosition();
		buttonState = DEFAULT;

		TextTexture* text = new TextTexture(textOnButton, color, font);
		SDL_Rect surfacePosition = {(int)((buttonTextures->getWidth() - text->getSurface()->w) / 2), (int)((buttonTextures->getHeight() - text->getSurface()->h) / 2), 0, 0};
		SDL_Surface* surface = buttonTextures->getTexture()->getSurface();

		for (int i = 0; i < 3; ++i) {
			SDL_BlitSurface(text->getSurface(), NULL, surface, &surfacePosition);
			surfacePosition.y += buttonTextures->getHeight();
		}
		buttonTextures->getTexture()->changeSurface(surface);
		buttonRect = Rectf(this->position.x, this->position.y, this->buttonTextures->getWidth(), this->buttonTextures->getHeight());

		RenderManager::getInstance().addObject(buttonTextures);
	}

	Button(Vecteur2f position, const char* buttonText, Font* font, SDL_Color defaultColor, SDL_Color ovreflownColor, SDL_Color clickedColor) : Button() {

		TextTexture* text = new TextTexture(buttonText, defaultColor, font);
		SDL_Surface* finalSurface = SDL_CreateRGBSurface(text->getSurface()->flags, text->getSurface()->w, text->getSurface()->h * 3, 32, 0, 0, 0, 0);

		SDL_Rect surfacePosition = { 0, 0, 0, 0 };

		SDL_BlitSurface(text->getSurface(), NULL, finalSurface, &surfacePosition);
		surfacePosition.y += text->getSurface()->h;
		delete text;

		text = new TextTexture(buttonText, ovreflownColor, font);
		SDL_BlitSurface(text->getSurface(), NULL, finalSurface, &surfacePosition);
		surfacePosition.y += text->getSurface()->h;
		delete text;

		text = new TextTexture(buttonText, clickedColor, font);
		SDL_BlitSurface(text->getSurface(), NULL, finalSurface, &surfacePosition);
		delete text;

		buttonTextures = new Sprite(position, Rectf(), new Texture(finalSurface), 1, 3);
		this->position = position;
		buttonState = DEFAULT;
		buttonRect = Rectf(this->position.x, this->position.y, this->buttonTextures->getWidth(), this->buttonTextures->getHeight());

		RenderManager::getInstance().addObject(buttonTextures);
	}

	~Button() {

		delete buttonTextures;

		EventManager::getInstance().removeObject(this);
		RenderManager::getInstance().removeObject(buttonTextures);
	}

	bool reactToEvent(SDL_Event* event, bool newEvent) {

		if (newEvent) {

			Rectf scaledButtonRect = getModifiedRect();

			switch (event->type) {

				case SDL_MOUSEMOTION:

					if (scaledButtonRect.contain(event->button.x, event->button.y) && buttonState != CLICKED) {
						if (buttonTextures->getWFrame() == 1)
							buttonTextures->changeFrame(0, OVERFLOWN);
						else
							buttonTextures->changeFrame(OVERFLOWN, 0);
						buttonState = OVERFLOWN;
					}
					else {
						if (!scaledButtonRect.contain(event->button.x, event->button.y)) {
							if (buttonTextures->getWFrame() == 1)
								buttonTextures->changeFrame(0, DEFAULT);
							else
								buttonTextures->changeFrame(DEFAULT, 0);
							buttonState = DEFAULT;
						}
					}

					break;

				case SDL_MOUSEBUTTONDOWN:

					if (scaledButtonRect.contain(event->button.x, event->button.y) &&
						event->button.button == SDL_BUTTON_LEFT) {
						if (buttonTextures->getWFrame() == 1)
							buttonTextures->changeFrame(0, CLICKED);
						else
							buttonTextures->changeFrame(CLICKED, 0);
						buttonState = CLICKED;
					}
					break;

				case SDL_MOUSEBUTTONUP:
					if (scaledButtonRect.contain(event->button.x, event->button.y) &&
						event->button.button == SDL_BUTTON_LEFT) {
						if (buttonTextures->getWFrame() == 1)
							buttonTextures->changeFrame(0, OVERFLOWN);
						else
							buttonTextures->changeFrame(OVERFLOWN, 0);
						buttonState = OVERFLOWN;
					}
					else if (event->button.button == SDL_BUTTON_LEFT) {
						if (buttonTextures->getWFrame() == 1)
							buttonTextures->changeFrame(0, DEFAULT);
						else
							buttonTextures->changeFrame(DEFAULT, 0);
						buttonState = DEFAULT;
					}
					break;
			}
			if (buttonState != DEFAULT)
				return checkReaction(event->type);
		}
		return false;
	}

	bool isOnButton(int x, int y) {

		return getModifiedRect().contain(x, y);
	}

	Rectf getRect() {

		return buttonRect;
	}

	Sprite* getTexture() {

		return buttonTextures;
	}

	button_state getButtonState() {

		return buttonState;
	}

	Rectf getModifiedRect() {

		return Rectf(buttonRect.x * buttonTextures->getScale().x, buttonRect.y * buttonTextures->getScale().y, buttonRect.w * buttonTextures->getScale().x, buttonRect.h * buttonTextures->getScale().y);
	}

	void setPosition(Vecteur2f pos) {

		buttonTextures->setPosition(pos);
		position = pos;
		buttonRect.x = pos.x;
		buttonRect.y = pos.y;
	}

	void setButtonState(button_state state) {

		if (buttonTextures->getWFrame() == 1)
			buttonTextures->changeFrame(0, state);
		else
			buttonTextures->changeFrame(state, 0);

		buttonState = state;
	}

};