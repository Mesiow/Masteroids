#pragma once
#include <Game/State/State.h>

enum class MenuLocation {
	Main = 0,
	Mp,
	MpSub
};

class MenuState : public State {
public:
	MenuState(Game* game);
	~MenuState();

	void handleEvents(sf::Event& ev, sf::RenderWindow& window)override;
	void handleInput(float dt)override;
	void update(float dt)override;
	void render(sf::RenderWindow& window)override;

private:
	void renderMenu();
	void handleMenuEvents(sf::Event &ev);

	void buildGui();
	void buildMpMenu();

private:
	MenuLocation _location;

	tgui::Gui _gui;
	tgui::Gui _mpmenu;
};