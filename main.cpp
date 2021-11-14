#include "NewGarphics.h"

int main()
{
	NewGarphics game;
	
	if (!game.ConstructConsole(360, 200, 2, 2, L"Light's"))
		game.Loop();

	return 0;
}