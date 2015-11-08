#pragma once
#include "Timer.h"
#include "Image.h"

class ScrollBar : public Control {

private:

	Button* topArrow;
	Button* bottomArrow;
	Button* scrollBar;
	Image* background;
	int notDrawElements;
	int stepSize;
	bool firstClick;
	bool barClicked;
	Timer* timer;
	int numberStepsDone;

public:

	ScrollBar(Button* topArrow, Button* bottomArrow, Button* scrollBar, Image* background) {

		this->topArrow = topArrow;
		this->bottomArrow = bottomArrow;
		this->scrollBar = scrollBar;
		this->background = background;

		numberStepsDone = 0;
		stepSize = 5;
		notDrawElements = 0;
		position = background->getPosition();

		timer = new Timer(0.0);
		firstClick = true;
		barClicked = false;

		int distance = bottomArrow->getPosition().y - (topArrow->getPosition().y + topArrow->getRect().h);
		scrollBar->getTexture()->setScale(Vecteur2f(1, distance / scrollBar->getRect().h));
		scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, (scrollBar->getPosition().y / scrollBar->getTexture()->getScale().y)));

		EventManager::getInstance().addObject(this);
	}

	~ScrollBar() {

		RenderManager::getInstance().removeObject(background);
		delete topArrow;
		delete bottomArrow;
		delete scrollBar;
		delete background;
	}

	bool reactToEvent(SDL_Event* event, bool newEvent) {

		if (notDrawElements > 0) {

			if (timer->isFinished()) {

				if (firstClick) {

					firstClick = false;
					timer->setTime(0.5);
				}

				else {

					timer->setTime(0.05);
				}

				if (topArrow->isOnButton(event->button.x, event->button.y) && topArrow->getButtonState() == CLICKED) {

					if (scrollBar->getModifiedRect().y == (topArrow->getPosition().y + topArrow->getRect().h))
						numberStepsDone = 0;

					else
						numberStepsDone = -1;

					if (scrollBar->getModifiedRect().y - stepSize < (topArrow->getPosition().y + topArrow->getRect().h))
						scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, (topArrow->getPosition().y + topArrow->getRect().h) / scrollBar->getTexture()->getScale().y));

					else
						scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, scrollBar->getPosition().y - stepSize));

				}

				else if (bottomArrow->isOnButton(event->button.x, event->button.y) && bottomArrow->getButtonState() == CLICKED) {

					if ((scrollBar->getModifiedRect().y + scrollBar->getModifiedRect().h) == bottomArrow->getTexture()->getPosition().y)
						numberStepsDone = 0;

					else
						numberStepsDone = 1;

					if (scrollBar->getModifiedRect().y + scrollBar->getModifiedRect().h + stepSize > bottomArrow->getTexture()->getPosition().y)
						scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, (bottomArrow->getPosition().y - scrollBar->getModifiedRect().h) / scrollBar->getTexture()->getScale().y));

					else
						scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, scrollBar->getPosition().y + stepSize));

				}

				else {

					firstClick = true;
					timer->setTime(0.0);
					numberStepsDone = 0;
				}

				timer->restart();
			}

			if (event->type == SDL_MOUSEWHEEL && newEvent) {

				int yrel = event->wheel.y;

				if (yrel > 0) {

					if (scrollBar->getModifiedRect().y == (topArrow->getPosition().y + topArrow->getRect().h))
						numberStepsDone = 0;

					else
						numberStepsDone = -1;

					if (scrollBar->getModifiedRect().y - stepSize < (topArrow->getPosition().y + topArrow->getRect().h))
						scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, (topArrow->getPosition().y + topArrow->getRect().h) / scrollBar->getTexture()->getScale().y));

					else
						scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, (scrollBar->getPosition().y - stepSize)));

				}

				else if (yrel < 0) {

					if ((scrollBar->getModifiedRect().y + scrollBar->getModifiedRect().h) == bottomArrow->getTexture()->getPosition().y)
						numberStepsDone = 0;

					else
						numberStepsDone = 1;

					if (scrollBar->getModifiedRect().y + scrollBar->getModifiedRect().h + stepSize > bottomArrow->getTexture()->getPosition().y)
						scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, (bottomArrow->getPosition().y - scrollBar->getModifiedRect().h) / scrollBar->getTexture()->getScale().y));

					else
						scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, scrollBar->getPosition().y + stepSize));

				}

				else
					numberStepsDone = 0;

			}

			if (scrollBar->isOnButton(event->button.x, event->button.y) && scrollBar->getButtonState() == CLICKED) {

				barClicked = true;
			}

			else if (event->type == SDL_MOUSEBUTTONUP && barClicked)
				barClicked = false;

			if (barClicked && newEvent) {

				if (scrollBar->getButtonState() != CLICKED)
					scrollBar->setButtonState(CLICKED);

				int yrel = event->motion.yrel;

				if (yrel < 0) {

					if (scrollBar->getModifiedRect().y == (topArrow->getPosition().y + topArrow->getRect().h))
						numberStepsDone = 0;

					else
                        numberStepsDone = yrel / stepSize;


					if (scrollBar->getModifiedRect().y + yrel < (topArrow->getPosition().y + topArrow->getRect().h))
						scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, (topArrow->getPosition().y + topArrow->getRect().h) / scrollBar->getTexture()->getScale().y));

					else
						scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, (scrollBar->getPosition().y + (yrel / scrollBar->getTexture()->getScale().y))));
				}

				else if (yrel > 0) {

					if ((scrollBar->getModifiedRect().y + scrollBar->getModifiedRect().h) == bottomArrow->getTexture()->getPosition().y)
						numberStepsDone = 0;

					else
						numberStepsDone = yrel / stepSize;

					if (scrollBar->getModifiedRect().y + scrollBar->getModifiedRect().h + yrel > bottomArrow->getTexture()->getPosition().y)
						scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, (bottomArrow->getPosition().y - scrollBar->getModifiedRect().h) / scrollBar->getTexture()->getScale().y));

					else
						scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, scrollBar->getPosition().y + (yrel / scrollBar->getTexture()->getScale().y)));
				}

				else
					numberStepsDone = 0;
			}
		}
		return false;
	}

	void setNotDrawsNumber(int element) {

		notDrawElements = element;

        int distance = bottomArrow->getPosition().y - (topArrow->getPosition().y + topArrow->getRect().h);

        distance -= (notDrawElements * stepSize);
        scrollBar->getTexture()->setScale(Vecteur2f(1, distance / scrollBar->getRect().h));
        scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, (((topArrow->getPosition().y + topArrow->getRect().h) + (notDrawElements * stepSize)) / scrollBar->getTexture()->getScale().y)));
	}

	void show() {

		RenderManager::getInstance().addObject(background);
		RenderManager::getInstance().addObject(topArrow->getTexture());
		RenderManager::getInstance().addObject(bottomArrow->getTexture());
		RenderManager::getInstance().addObject(scrollBar->getTexture());
	}

	void stopShowing() {

		RenderManager::getInstance().removeObject(background);
		RenderManager::getInstance().removeObject(topArrow->getTexture());
		RenderManager::getInstance().removeObject(bottomArrow->getTexture());
		RenderManager::getInstance().removeObject(scrollBar->getTexture());
	}

	Vecteur2f getSize() {

		return Vecteur2f(background->getModifiedRect().w, background->getModifiedRect().h);
	}

	int getNumberStepsDone() {

		return numberStepsDone;
	}

	void setMaxY() {

		scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, (bottomArrow->getPosition().y - scrollBar->getModifiedRect().h) / scrollBar->getTexture()->getScale().y));
	}

	void setMinY() {

		scrollBar->setPosition(Vecteur2f(scrollBar->getPosition().x, (topArrow->getPosition().y + topArrow->getRect().h) / scrollBar->getTexture()->getScale().y));
	}
};