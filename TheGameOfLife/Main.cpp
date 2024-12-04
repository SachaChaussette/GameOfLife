#include "Macro.h"
#include "Game.h"


using namespace Macro;

int main()
{
	Config();

	Game _game = Game(100,100); 
	_game.Gameloop();

	return EXIT_SUCCESS;
}