#include <Game/Game.h>
#include "States/MenuState.h"
#include "Net/Constants.h"

int main() {
	Game game;
	game.create("Asteroids", SCREEN_WIDTH, SCREEN_HEIGHT);
	game.setWindowIcon("Res/thrust.png");
	game.pushState(new MenuState(&game));
	game.run();

	return 0;
}