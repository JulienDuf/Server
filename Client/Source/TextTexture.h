#pragma once
#include "Font.h"
#include "Texture.h"

class TextTexture : public Texture {

private:

	SDL_Color color;
	Font* font;
	std::string text;

public:

	TextTexture() {
		font = nullptr;
	}

	TextTexture(const char* text, SDL_Color color, Font* font) {

		surface = TTF_RenderText_Blended(font->getFont(), text, color);
		this->color = color;
		this->font = font;
		this->text = std::string(text);

		RessourceManager::getInstance().putRessourceLoading(this);
	}

	SDL_Color getColor() {
		return color;
	}

	Font* getFont() {
		return font;
	}

	const char* getText() {
		return text.c_str();
	}

	void setText(const char* text) {

		this->text = std::string(text);
		SDL_FreeSurface(surface);
		surface = TTF_RenderText_Blended(font->getFont(), text, color);
		reload();
	}

	void setText(std::string string) {

		this->text = string;
	}
};