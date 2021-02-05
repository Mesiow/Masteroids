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

	_location = MenuLocation::Main;
	buildGui();
	buildMpMenu();
}

MenuState::~MenuState()
{

}

void MenuState::handleEvents(sf::Event& ev, sf::RenderWindow& window)
{
	handleMenuEvents(ev);
}

void MenuState::handleInput(float dt)
{
}

void MenuState::update(float dt)
{
}

void MenuState::render(sf::RenderWindow& window)
{
	renderMenu();
}

void MenuState::renderMenu()
{
	switch (_location) {
		case MenuLocation::Main: _gui.draw(); break;
		case MenuLocation::Mp: _mpmenu.draw(); break;
	}
}

void MenuState::handleMenuEvents(sf::Event& ev)
{
	switch (_location) {
		case MenuLocation::Main: _gui.handleEvent(ev); break;
		case MenuLocation::Mp: _mpmenu.handleEvent(ev); break;
	}
}

void MenuState::buildGui()
{
	_gui.setTarget(_game->getWindowHandle());
	_gui.loadWidgetsFromFile("Res/gui/AsteroidsMenu.txt");


	auto sp = _gui.get<tgui::Button>("SingleplayerBtn");
	sp->connect("pressed", [&]() { _game->pushState(new SPState(_game)); });

	auto mp = _gui.get<tgui::Button>("MultiplayerBtn");
	//mp->connect("pressed", [&]() { _game->pushState(new MPState(_game)); });
	mp->connect("pressed", [&]() { _location = MenuLocation::Mp; });
}

void MenuState::buildMpMenu()
{
	_mpmenu.setTarget(_game->getWindowHandle());
	_mpmenu.loadWidgetsFromFile("Res/gui/mp_options.txt");

	auto back = _mpmenu.get<tgui::Button>("BackBtn");
	back->connect("pressed", [&]() { _location = MenuLocation::Main; });
}
