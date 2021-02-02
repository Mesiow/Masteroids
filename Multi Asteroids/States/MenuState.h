#pragma once
#include <Game/State/State.h>


class MenuState : public State {
public:
	MenuState(Game* game);
	~MenuState();

	void handleEvents(sf::Event& ev, sf::RenderWindow& window)override;
	void handleInput(float dt)override;
	void update(float dt)override;
	void render(sf::RenderWindow& window)override;

private:
	void buildGui();

private:
	tgui::Gui _gui;
};