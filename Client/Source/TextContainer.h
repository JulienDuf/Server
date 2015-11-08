#pragma once
#include "Control.h"
#include "TextContainerObject.h"

class TextContainer : public Control {

private:

	Image* container;
	Rectf containerRect;
	std::vector<TextContainerObject*> texts;
	std::list<TextContainerObject*> textsDraw;
	ScrollBar* scrollBar;
	Vecteur2f nextTextPosition;
	Font* font;
	unsigned int step;
	unsigned int notDrawElements;
    float scrollStep;

    bool upObject() {

        TextContainerObject *tmp = (*(--texts.end()));

        if (tmp->getPosition().y + tmp->getSize().y - scrollStep == containerRect.y + containerRect.h)
            return true;

        float stepY = scrollStep;

        if (tmp->getPosition().y + tmp->getSize().y - scrollStep < containerRect.y + containerRect.h)
            stepY = (tmp->getPosition().y + tmp->getSize().y) - (containerRect.y + containerRect.h) + step;

        for (auto object : texts) {

            object->setPosition(Vecteur2f(object->getPosition().x, object->getPosition().y - stepY));

            if (object->getPosition().y > containerRect.h + containerRect.h) {

                auto it = std::find(textsDraw.begin(), textsDraw.end(), object);

                if (it != textsDraw.end()) {
                    textsDraw.erase(it);
                    object->stopShowing();
                }
            }

            else if (object->getPosition().y + object->getSize().y > 0) {

                auto it = std::find(textsDraw.begin(), textsDraw.end(), object);

                if (it == textsDraw.end()) {
                    textsDraw.push_back(object);
                    object->show();
                }
            }
        }

        return stepY != scrollStep;
    }

    bool downObject() {

        TextContainerObject *tmp = (*texts.begin());

        float stepY = scrollStep;

        if (tmp->getPosition().y + scrollStep == containerRect.y)
            return true;

        if (tmp->getPosition().y + scrollStep > containerRect.y)
            stepY = containerRect.y - tmp->getPosition().y;

        for (auto object : texts) {

            object->setPosition(Vecteur2f(object->getPosition().x, object->getPosition().y + stepY));

            if (object->getPosition().y > containerRect.h + containerRect.h) {

                auto it = std::find(textsDraw.begin(), textsDraw.end(), object);

                if (it != textsDraw.end()) {
                    textsDraw.erase(it);
                    object->stopShowing();
                }
            }

            else if (object->getPosition().y + object->getSize().y > 0) {

                auto it = std::find(textsDraw.begin(), textsDraw.end(), object);

                if (it == textsDraw.end()) {
                    textsDraw.push_back(object);
                    object->show();
                }
            }
        }

        return stepY != scrollStep;
    }

public:

	TextContainer(Image* image, Font* font, ScrollBar* scrollBar, unsigned int step, Rectf containerRect) {

		this->container = image;
		this->font = font;
		this->position = image->getPosition();
		this->scrollBar = scrollBar;
        this->containerRect = containerRect;
		nextTextPosition.x = containerRect.x;
		nextTextPosition.y = containerRect.y;
		this->step = step;
		notDrawElements = 0;
        scrollStep = 40;

		if (scrollBar != nullptr)
			scrollBar->stopShowing();

		RenderManager::getInstance().addObject(container);

		if (scrollBar != nullptr)
			scrollBar->show();

        EventManager::getInstance().addObject(this);
	}

	bool reactToEvent(SDL_Event* event, bool newEvent) {

        if (scrollBar != nullptr && newEvent) {

            int steps = scrollBar->getNumberStepsDone();

            if (steps < 0) {

                downObject();
            }

            else if (steps > 0) {

                upObject();
            }
        }
		return checkReaction(event->type);
	}

	void addText(std::string text, Texture* background, Text* title, position_type positionType) {

        Vecteur2f pos = nextTextPosition;

        if (positionType == SHOW_RIGHT)
            pos.x = containerRect.x + containerRect.w - scrollBar->getSize().x - 20;

        else if (positionType == SHOW_MIDDLE)
            pos.x += (containerRect.w / 2) - pos.x;

        texts.push_back(new TextContainerObject(text, pos, new Image(background, Vecteur2f()), title, font,
                                                container->getModifiedRect().w / 2, positionType));

        if (nextTextPosition.y + texts.back()->getSize().y < containerRect.y + containerRect.h) {

            nextTextPosition.y += (texts.back()->getSize().y + step);

        }

        else {

            TextContainerObject *tmp = texts.back();
            texts.pop_back();

            while (!upObject());

            texts.push_back(tmp);

            notDrawElements = 0;

            Vecteur2f pos = Vecteur2f(tmp->getPosition().x,
                                      containerRect.y + containerRect.h - texts.back()->getSize().y - step);
            float diffY = nextTextPosition.y - pos.y;

            for (auto object : texts) {

                object->setPosition(Vecteur2f(object->getPosition().x, object->getPosition().y - diffY));

                if (object->getPosition().y + object->getSize().y < 0) {

                    notDrawElements += (int)((object->getSize().y / 40) + 1);

                    auto it = std::find(textsDraw.begin(), textsDraw.end(), object);

                    if (it != textsDraw.end()) {
                        textsDraw.erase(it);
                        object->stopShowing();
                    }
                }
            }

            scrollBar->setMaxY();
            scrollBar->setNotDrawsNumber(notDrawElements);

        }

        textsDraw.push_back(texts.back());
        texts.back()->show();
    }

	void removeText(std::string text) {

		int pos = 0;

		for (TextContainerObject* item: texts) {

			if (!strcmp(item->getText()->getTexture()->getText(), text.c_str())) {

				auto it = std::find(textsDraw.begin(), textsDraw.end(), item);

				int tmp = pos - 1;

				if (texts.size() > 1) {
					for (int i = pos + 1; i < texts.size(); tmp = i, ++i) {

						if (tmp >= 0)
							texts[i]->setPosition(Vecteur2f(texts[i]->getPosition().x,
															texts[tmp]->getPosition().y + texts[tmp]->getSize().y +
															step));

						else
							texts[i]->setPosition(Vecteur2f(texts[i]->getPosition().x, texts[0]->getPosition().y));
					}

					if (it != textsDraw.end()) {

						texts.erase(std::find(texts.begin(), texts.end(), item));
						(*it)->stopShowing();
					}
					nextTextPosition.y = (texts.back()->getPosition().y + texts.back()->getSize().y + step);
				}

				else {

					if (it != textsDraw.end()) {

						texts.erase(std::find(texts.begin(), texts.end(), item));
						(*it)->stopShowing();
					}

					nextTextPosition = position;
					nextTextPosition.x = containerRect.x;
					nextTextPosition.y = containerRect.y;
				}

			}
			++pos;
		}
	}
};