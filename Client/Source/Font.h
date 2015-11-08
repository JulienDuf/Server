#pragma once
#include "Ressource.h"

class Font : public Ressource{

private:

	TTF_Font* font;
	int size;

public:

	Font() {
		font = nullptr;
	}

	Font(const char* path, int size) {

		font = TTF_OpenFont(path, size);
		this->size = size;
	}

	~Font() {
		TTF_CloseFont(font);
	}

	TTF_Font* getFont() {
		return font;
	}

	int getSize(){
		return this->size;
	}

	bool load() {
		return true;
	}

};