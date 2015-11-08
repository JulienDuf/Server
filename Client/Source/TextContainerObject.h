#pragma once

#include "Text.h"
#include "Image.h"
#include "RenderManager.h"

enum position_type {SHOW_LEFT, SHOW_RIGHT, SHOW_MIDDLE};

class TextContainerObject {

private:

    Text* title;
    Text* text;
    Image* background;
    Vecteur2f size;
    Font* font;
    position_type positionType;

    bool isTextOk(const char* text, int maxW) {

        int w, h;
        Engine::getTextSize(text, font, w, h);

        if (w > maxW)
            return false;

        return true;
    }

    void addText(std::string text, Vecteur2f position, int maxW, position_type type) {

        std::string textShow = std::string(text);
        int checkWord;
        bool ok = false;
        bool noSpace = false;
        std::vector<SDL_Surface*> surfaceTexts;
        TextTexture* texture = new TextTexture();
        texture->setText(text);
        SDL_Surface* surface;
        SDL_Rect surfacePosition = { 0, 0, 0, 0 };

        if (isTextOk(textShow.c_str(), maxW)) {
            surfaceTexts.push_back(TTF_RenderText_Blended(font->getFont(), textShow.c_str(), { 0, 0, 0, 255 }));
            ok = true;
        }
        while (!ok) {
            checkWord = textShow.size() - 1;
            while (!isTextOk(textShow.c_str(), maxW)) {

                --checkWord;
                while ((textShow[checkWord] != ' ') && (checkWord != 0)) {
                    --checkWord;
                    if (checkWord == 0) {
                        noSpace = true;
                    }
                }
                if (noSpace) {
                    checkWord = textShow.size() - 1;
                    while (!isTextOk(textShow.c_str(), maxW)) {
                        --checkWord;
                        textShow.erase(checkWord, textShow.size());
                    }
                }
                textShow.erase(checkWord, textShow.size());
            }
            surfaceTexts.push_back(TTF_RenderText_Blended(font->getFont(), textShow.c_str(), { 0, 0, 0, 255 }));
            textShow = text;
            textShow.erase(0, checkWord + 1);
            text = textShow;
            ok = (textShow.size() == 0);
        }

        int w, h;
        h = w = 0;
        if (surfaceTexts.size() > 1) {
            for (int i = 0; i < surfaceTexts.size(); ++i) {

                if (w < surfaceTexts[i]->w)
                    w = surfaceTexts[i]->w;
                h += (surfaceTexts[i]->h + 2);
            }

            surface = SDL_CreateRGBSurface(0, w, h - 2, 32, 0, 0, 0, 0);

            for (int i = 0; i < surfaceTexts.size(); ++i) {

                SDL_BlitSurface(surfaceTexts[i], NULL, surface, &surfacePosition);
                surfacePosition.y += (surfaceTexts[i]->h + 2);
                SDL_FreeSurface(surfaceTexts[i]);

            }
            texture->changeSurface(surface);
            surfaceTexts.clear();
        }
        else {
            texture->changeSurface(surfaceTexts[0]);
        }
        this->text = new Text(texture, Vecteur2f(position.x + 7, position.y + 7 + title->getTexture()->getSurface()->h));

        int w1 = this->text->getTexture()->getSurface()->w;
        w1 += 14;
        int h1 = this->text->getTexture()->getSurface()->h;
        h1 += 14;

        if (w1 < title->getTexture()->getSurface()->w)
            w1 = title->getTexture()->getSurface()->w;

        title->setPosition(Vecteur2f(position.x, position.y));

        float scaleX = (float)w1 / (float)background->getTexture()->getSurface()->w;
        float scaleY = (float)h1 / (float)background->getTexture()->getSurface()->h;

        background->setScale(Vecteur2f(scaleX, scaleY));
        size = Vecteur2f(background->getModifiedRect().w, background->getModifiedRect().h);

        if (type == SHOW_RIGHT) {

            this->text->setPosition(Vecteur2f(this->text->getPosition().x - size.x, this->text->getPosition().y));
            title->setPosition(Vecteur2f(title->getPosition().x - size.x, title->getPosition().y));
            position.x -= size.x;
        }

        else if (type == SHOW_MIDDLE) {

            this->text->setPosition(Vecteur2f(this->text->getPosition().x - (size.x / 2), this->text->getPosition().y));
            title->setPosition(Vecteur2f(title->getPosition().x - (size.x / 2), title->getPosition().y));
            position.x -= (size.x / 2);
        }

        float posX = (float)position.x / scaleX;
        float posY = (float)(position.y + title->getTexture()->getSurface()->h) / scaleY;

        background->setPosition(Vecteur2f(posX, posY));
    }

public:

    TextContainerObject(std::string text, Vecteur2f position, Image* background, Text* title, Font* font, int maxW, position_type type) {

        this->font = font;
        this->background = background;
        this->title = title;
        this->positionType = type;
        addText(text, position, maxW, positionType);

    }

    void show() {

        RenderManager::getInstance().addObject(text, true);
        RenderManager::getInstance().addObject(title, true);
        RenderManager::getInstance().addObject(background, true);
    }

    void stopShowing() {

        RenderManager::getInstance().removeObject(title);
        RenderManager::getInstance().removeObject(text);
        RenderManager::getInstance().removeObject(background);
    }

    Vecteur2f getSize() {

        return size;
    }

    Vecteur2f getPosition() {

        return title->getPosition();
    }

    Text* getText() {

        return text;
    }

    void setPosition(Vecteur2f pos) {

        float difX = pos.x - title->getPosition().x;
        float difY = pos.y - title->getPosition().y;

        background->setPosition(Vecteur2f((background->getModifiedRect().x + difX) / background->getScale().x, (background->getModifiedRect().y + difY) / background->getScale().y));
        text->setPosition(Vecteur2f(text->getPosition().x + difX, text->getPosition().y + difY));
        title->setPosition(Vecteur2f(title->getPosition().x + difX, title->getPosition().y + difY));

    }
};
