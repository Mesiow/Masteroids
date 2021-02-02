#include "MenuState.h"
#include <Game/Game.h>
#include <Game/ResourceManager/ResourceManager.h>
#include "SPState.h"
#include "MPState.h"

MenuState::MenuState(Game* game)
	:State(game)
{
	//load necessary resources here
	ResourceManager::loadSound("shoot", "Res/playershoot.wav");
	ResourceManager::loadFont("mont", "Res/gui/Montserrat-Black.ttf");

	buildGui();
}

MenuState::~MenuState()
{

}

void MenuState::handleEvents(sf::Event& ev, sf::RenderWindow& window)
{
	_gui.handleEvent(ev);
}

void MenuState::handleInput(float dt)
{
}

void MenuState::update(float dt)
{
}

void MenuState::render(sf::RenderWindow& window)
{
	_gui.draw();
}

void MenuState::buildGui()
{
	_gui.setTarget(_game->getWindowHandle());
	_gui.loadWidgetsFromFile("Res/gui/AsteroidsMenu.txt");


	auto sp = _gui.get<tgui::Button>("SingleplayerBtn");
	sp->connect("pressed", [&]() { _game->pushState(new SPState(_game)); });

	auto mp = _gui.get<tgui::Button>("MultiplayerBtn");
	mp->connect("pressed", [&]() { _game->pushState(new MPState(_game)); });
}
