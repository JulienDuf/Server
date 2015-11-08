#pragma once
#include "Vecteur2.h"
#include "Ressource.h"
#include <string>

class Texture : public Ressource{
protected:
	SDL_Surface* surface;
	GLuint ID;
public:

	Texture() {
		surface = nullptr;
		ID = -1;
	}

	Texture(const char* path) {
		ID = -1;
		load(path);
	}

	Texture(SDL_Surface* surface) {
		this->surface = surface;
		ID = -1;
		RessourceManager::getInstance().putRessourceLoading(this);
	}

	~Texture() {
		if (surface != nullptr){
			if (surface->format != nullptr)
				SDL_FreeSurface(surface);
			surface = nullptr;
			glDeleteTextures(1, &ID);
		}
	}

	void load(const char* path){
		surface = IMG_Load(path);
		RessourceManager::getInstance().putRessourceLoading(this);
	}

	bool load() {

		glEnable(GL_TEXTURE_2D);

		if (ID == -1)
			glGenTextures(1, &ID);

		if (surface != nullptr){
			glBindTexture(GL_TEXTURE_2D, ID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

#if defined(__APPLE__) && defined(__MACH__)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);
#elif defined _WIN32 || defined(__linux__)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
#endif
		}

		return (surface != nullptr);
	}

	void reload() {
		glDeleteTextures(1, &ID);
		load();
	}

	void changeSurface(SDL_Surface* surface) {
		this->surface = surface;
		RessourceManager::getInstance().putRessourceLoading(this);
	}

	GLuint getID(){
		return ID;
	}

	Vecteur2d getSize() {
		return Vecteur2d(surface->w, surface->h);
	}

	SDL_Surface* getSurface() {
		return surface;
	}

};