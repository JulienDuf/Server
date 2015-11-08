#pragma once
#include "Control.h"
#include "Timer.h"
#include "Engine.h"

class TextBox : public Control {

private:

	Image* textBox;
	Text* textInBox;
	Image* bar;
	std::string text;
	bool active;
	bool noLetter;
	bool barShowed;
	Timer* timer;

	void setText() {

		int w, h;
		std::string drawText = text;
		Engine::getTextSize(text.c_str(), textInBox->getTexture()->getFont(), w, h);

		if (w < (textBox->getRect().w - 20))
			textInBox->getTexture()->setText(text.c_str());
		
		else {

			do {
				drawText.erase(0, 1);
				Engine::getTextSize(drawText.c_str(), textInBox->getTexture()->getFont(), w, h);
			} while (w > (textBox->getRect().w - 20));
			textInBox->getTexture()->setText(drawText.c_str());
		}
		
		textInBox->setPosition(Vecteur2f((textBox->getPosition().x + 10), textBox->getRect().y + ((textBox->getRect().h - textInBox->getTexture()->getSize().y) / 2)));
		bar->setPosition(Vecteur2f(textInBox->getPosition().x + textInBox->getTexture()->getSurface()->w + 2, bar->getPosition().y));

		if (noLetter)
			bar->setPosition(Vecteur2f(position.x + 10, position.y + 4));
	}

public:

	TextBox(Image* box, Font* font) {

		this->textBox = box;
		text = std::string(" ");
		noLetter = true;
		textInBox = new Text(new TextTexture(" ", { 0, 0, 0, 0 }, font), Vecteur2f((box->getPosition().x + 5), (box->getRect().y + box->getRect().h) / 2));
		active = false;
		barShowed = false;

		position = textBox->getPosition();

		bar = new Image(Engine::isTextureExist("textbox_bar", "Ressources/textbox_bar.png"), Vecteur2f(position.x + 10, position.y + 4));

		timer = new Timer(0.6);
		timer->restart();

		RenderManager::getInstance().addObject(textBox);
		RenderManager::getInstance().addObject(textInBox);
		EventManager::getInstance().addObject(this);
	}

	~TextBox() {

		RenderManager::getInstance().removeObject(textBox);
		RenderManager::getInstance().removeObject(textInBox);
		EventManager::getInstance().removeObject(this);

		RenderManager::getInstance().removeObject(bar);

		delete textBox;
		delete textInBox;
	}

	bool reactToEvent(SDL_Event* event, bool newEvent) {

		if (timer->isFinished() && active) {

			if (barShowed)
				RenderManager::getInstance().removeObject(bar);

			else
				RenderManager::getInstance().addObject(bar);

			barShowed = !barShowed;
			timer->restart();
		}

		if (newEvent) {
			char *textTmp = nullptr;

			std::string etAccent;
			etAccent.push_back(-61);
			etAccent.push_back(-87);

			std::string etAccentM;
			etAccentM.push_back(-61);
			etAccentM.push_back(-119);

			std::string estAccent;
			estAccent.push_back(-61);
			estAccent.push_back(-88);

			std::string estAccentM;
			estAccentM.push_back(-61);
			estAccentM.push_back(-120);

			std::string aAccent;
			aAccent.push_back(-61);
			aAccent.push_back(-96);

			std::string aAccentM;
			aAccentM.push_back(-61);
			aAccentM.push_back(-128);

			std::string c;
			c.push_back(-61);
			c.push_back(-89);

			std::string cM;
			cM.push_back(-61);
			cM.push_back(-121);

			std::string estChapeau;
			estChapeau.push_back(-61);
			estChapeau.push_back(-86);

			std::string estChapeauM;
			estChapeauM.push_back(-61);
			estChapeauM.push_back(-118);

			SDL_StartTextInput();

			int lastSize = text.size();

			switch (event->type) {

				case SDL_TEXTINPUT:

					if (active) {

						if (!strcmp(event->text.text, etAccent.c_str()))
							textTmp = "é";
						else if (!strcmp(event->text.text, etAccentM.c_str()))
							textTmp = "É";
						else if (!strcmp(event->text.text, estAccent.c_str()))
							textTmp = "è";
						else if (!strcmp(event->text.text, estAccentM.c_str()))
							textTmp = "È";
						else if (!strcmp(event->text.text, aAccent.c_str()))
							textTmp = "à";
						else if (!strcmp(event->text.text, aAccentM.c_str()))
							textTmp = "À";
						else if (!strcmp(event->text.text, c.c_str()))
							textTmp = "ç";
						else if (!strcmp(event->text.text, cM.c_str()))
							textTmp = "Ç";
						else if (!strcmp(event->text.text, estChapeau.c_str()))
							textTmp = "ê";
						else if (!strcmp(event->text.text, estChapeauM.c_str()))
							textTmp = "Ê";
						else
							textTmp = event->text.text;
						if (noLetter) {
							text.clear();
							noLetter = false;
						}
						text.append(textTmp);
						setText();
					}
					break;

				case SDL_MOUSEBUTTONUP:

					if (textBox->getRect().contain(event->button.x, event->button.y))
						active = true;
					else
						active = false;

					break;

				case SDL_KEYDOWN:

					if (active) {

						switch (event->key.keysym.scancode) {

							case SDL_SCANCODE_TAB:

								active = false;
								break;
							case SDL_SCANCODE_BACKSPACE:

								if (text.size() > 0)
									text.pop_back();
								if (text.size() == 0) {
									text.push_back(' ');
									noLetter = true;
								}

								setText();
								break;

						}
					}
					break;
				case SDL_KEYUP:

					if (active) {

						switch (event->key.keysym.scancode) {

							case SDL_SCANCODE_TAB:

								active = false;
								break;
						}
					}
					break;
			}

			if (lastSize != text.size()) {
				if (!barShowed)
					RenderManager::getInstance().addObject(bar);

				timer->restart();
			}

			if (active)
				return checkReaction(event->type);

			else {
				if (barShowed)
					RenderManager::getInstance().removeObject(bar);
			}
		}

		return false;
	}

	std::string& getText() {
		return text;
	}
	
	void setText(std::string text) {
		this->text = text;
		if (text[0] == ' ')
			noLetter = true;
		setText();
	}

	void stopShowing() {

		RenderManager::getInstance().removeObject(textBox);
		RenderManager::getInstance().removeObject(textInBox);

		if (barShowed)
			RenderManager::getInstance().removeObject(bar);
	}

	void show() {

		RenderManager::getInstance().addObject(textBox);
		RenderManager::getInstance().addObject(textInBox);

		if (barShowed)
			RenderManager::getInstance().addObject(bar);

	}
}; 