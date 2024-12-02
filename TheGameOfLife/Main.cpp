#include "Macro.h"
#include "Game.h"
#include <iomanip>
#include <string>


using namespace Macro;

int main()
{
	Config();
	System::MaximizeConsole();


	// max 50 x 50 avec la grille sinon 100 x 100
	Grid _grid(100,100);
	_grid.Display(false);

	return EXIT_SUCCESS;
}