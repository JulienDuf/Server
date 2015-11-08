#pragma once
#include "Control.h"
#include "Image.h"
#include "Text.h"

class Label : public Control {

private:

	Image* background;
	Text* text;

public:

	Label() {

		text = nullptr;
		background = nullptr;

		EventManager::getInstance().addObject(this);
	}

	Label(Text* text) : Label() {

		this->text = text;
		this->position = text->getPosition();
		this->background = nullptr;

		RenderManager::getInstance().addObject(text);
	}

	Label(Text* text, Image* background) : Label() {

		this->text = text;
		this->position = background->getPosition();
		this->background = background;
		RenderManager::getInstance().addObject(text);
		RenderManager::getInstance().addObject(background);

		Vecteur2f pos;

		pos.x = background->getPosition().x + (background->getTexture()->getSurface()->w - text->getTexture()->getSurface()->w) / 2;
		pos.y = background->getPosition().y + (background->getTexture()->getSurface()->h - text->getTexture()->getSurface()->h) / 2;
		text->setPosition(pos);

	}

	~Label() {

		EventManager::getInstance().removeObject(this);
		RenderManager::getInstance().removeObject(text);
		if (background != nullptr)
			RenderManager::getInstance().removeObject(background);

		delete background;
		delete text;
	}

	bool reactToEvent(SDL_Event* event, bool newEvent) {

		Rectf rectTmp;

		if (newEvent) {
			if (background != nullptr)
				rectTmp = Rectf(background->getPosition().x, background->getPosition().y,
								background->getTexture()->getSurface()->w, background->getTexture()->getSurface()->h);

			else
				rectTmp = Rectf(position.x, position.y, text->getTexture()->getSurface()->w,
								text->getTexture()->getSurface()->h);

			if (rectTmp.contain(event->button.x, event->button.y))
				return checkReaction(event->type);
			else if (event->type != SDL_MOUSEBUTTONDOWN && event->type != SDL_MOUSEBUTTONUP)
				return checkReaction(event->type);

		}
		return false;
	}

	void setText(Text* text) {

		RenderManager::getInstance().removeObject(this->text);
		delete this->text;
		this->text = text;
		RenderManager::getInstance().addObject(this->text);

		Vecteur2f pos;

		pos.x = background->getPosition().x + (background->getTexture()->getSurface()->w - text->getTexture()->getSurface()->w) / 2;
		pos.y = background->getPosition().y + (background->getTexture()->getSurface()->h - text->getTexture()->getSurface()->h) / 2;
		text->setPosition(pos);
	}

	void setText(const char* text) {

		this->text->getTexture()->setText(text);
	}

	void setBackground(Image* background) {

		RenderManager::getInstance().removeObject(this->background);
		delete this->background;
		this->background = background;
		RenderManager::getInstance().addObject(this->background);
	}

	Text* getText() {
		return text;
	}

	void stopShowing() {

		RenderManager::getInstance().removeObject(text);
		if (background != nullptr)
			RenderManager::getInstance().removeObject(background);
	}

	void show() {

		RenderManager::getInstance().addObject(text);
		if (background != nullptr)
			RenderManager::getInstance().addObject(background);
	}
};