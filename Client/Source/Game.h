#pragma once
#include "Button.h"
#include "ScrollBar.h"
#include "Label.h"
#include "TextBox.h"
#include "TextContainer.h"

void enter_textBox(Control* sender) {

	TextContainer* container;
	TextBox* textBox = dynamic_cast<TextBox*>(sender);
	std::string message;

	if (Event::keybord.lastKeyUp == KEY_ENTER) {

		ClientInfo *info = new ClientInfo();
		info->name = Engine::client->getClientName();
		info->message = new std::string(textBox->getText());
		info->ID = Engine::client->getClientID();

		Engine::client->sendToServer(info);
		delete info;

		textBox->setText(std::string(" "));
	}
}

void clientReaction(ServerInfo* info) {

	std::string message;

	if ((*info->message != QUIT_SIGNAL) && (*info->message != SHUTDOWN_SIGNAL) && (info->clientName != "New client") && (info->message_type == NEW_MESSAGE)) {

		TextBox* textBox = dynamic_cast<TextBox*>(Engine::window->getControl("textBox"));

		message = *info->message;
		TextContainer *container = dynamic_cast<TextContainer *>(Engine::window->getControl("textContainer"));
		Text* name = new Text(Engine::isTextExist(info->clientName, info->clientName.c_str(), {0, 0, 0, 255}, Engine::isFontExist("Arial20", "Ressources/arial.ttf", 20)), Vecteur2f());

		if (info->clientID != Engine::client->getClientID())
			container->addText(message, Engine::isTextureExist("textcontainer_object1", "Ressources/textcontainer_object1.png"), name, SHOW_LEFT);

		else
			container->addText(message, Engine::isTextureExist("textcontainer_object2", "Ressources/textcontainer_object2.png"), name, SHOW_RIGHT);

		textBox->stopShowing();
		textBox->show();
	}

	else if (((info->message_type == NEW_CLIENT) && (info->clientID != Engine::client->getClientID())) || (info->message_type == CLIENTS_CONNECTED)) {

		Label* label = dynamic_cast<Label*>(Engine::window->getControl("label"));

		message = info->clientName;
		TextContainer *container = dynamic_cast<TextContainer *>(Engine::window->getControl("peoplesConnected"));
		container->addText(message, Engine::isTextureExist("white_blank", "Ressources/blank.png"), new Text(Engine::isTextExist("nothing", " ", {0, 0, 0, 255}, Engine::isFontExist("Arial20", "Ressources/arial.ttf", 20)), Vecteur2f()), SHOW_MIDDLE);

		label->stopShowing();
		label->show();

	}

	else if (info->message_type == CLIENT_DISCONNECTED) {

		TextContainer *container = dynamic_cast<TextContainer *>(Engine::window->getControl("peoplesConnected"));
		container->removeText(info->clientName);
	}
}

void enter_textBox_Name(Control* sender) {

	if (Event::keybord.lastKeyUp == KEY_ENTER) {

		TextBox* name = dynamic_cast<TextBox*>(sender);
		std::string clientName = name->getText();

		Engine::window->clearControls();

		Button* b1 = new Button(new Sprite(Vecteur2f(1142, 6), Rectf(), new Texture("Ressources/top_arrow.png"), 1, 3));
		Button* b2 = new Button(new Sprite(Vecteur2f(1142, 522), Rectf(), new Texture("Ressources/bottom_arrow.png"), 1, 3));
		Button* b3 = new Button(new Sprite(Vecteur2f(1142, b1->getTexture()->getHeight() + 6), Rectf(), new Texture("Ressources/scrollBar.png"), 1, 3));

		ScrollBar* scrollBar = new ScrollBar(b1, b2, b3, new Image(new Texture("Ressources/background_scrollbar.png"), Vecteur2f(1142, 6)));

		TextContainer* peoplesConnected = new TextContainer(new Image(new Texture("Ressources/connect_people_container.png"), Vecteur2f(0, 0)), Engine::isFontExist("arial25", "Ressources/arial.ttf", 25), nullptr, 0, Rectf(0, 40, 258, 500));
		TextContainer* textContainer = new TextContainer(new Image(new Texture("Ressources/text_container.png"), Vecteur2f(254, 0)), Engine::isFontExist("arial25", "Ressources/arial.ttf", 25), scrollBar, 30, Rectf(281, 27, 890, 438));

		TextBox* textBox = new TextBox(new Image(new Texture("Ressources/textBox.png"), Vecteur2f(500, 510)), Engine::isFontExist("arial20", "Ressources/arial.ttf", 20));
		textBox->addReaction(SDL_KEYUP, enter_textBox);

		Label* label = new Label(new Text(Engine::isTextExist("People online", "People online", {0,0,0,255}, Engine::isFontExist("Arial25", "Ressources/arial.ttf", 25)), Vecteur2f(50, 10)));

		Engine::window->addControl("peoplesConnected", peoplesConnected);
		Engine::window->addControl("textContainer", textContainer);
		Engine::window->addControl("textBox", textBox);
		Engine::window->addControl("label", label);

		Engine::createClient("104.236.69.2", clientReaction, clientName);
	}
}

class Game {

public:

	static void start() {

		Engine::createWindow(1173, 550);

		Label* label = new Label(new Text(Engine::isTextExist("Please enter your name and press Enter", "Please enter your name and press Enter", {0,0,0,255}, Engine::isFontExist("Arial30", "Ressources/arial.ttf", 30)), Vecteur2f(300, 200)));
		TextBox* textBox = new TextBox(new Image(new Texture("Ressources/textBox.png"), Vecteur2f(380, 250)), Engine::isFontExist("arial20", "Ressources/arial.ttf", 20));
		textBox->addReaction(SDL_KEYUP, enter_textBox_Name);

		Engine::window->addControl("label", label);
		Engine::window->addControl("textbox", textBox);

		while (Engine::window->windowState()) {

			Engine::renderAndEvent();
		}
	}
};
