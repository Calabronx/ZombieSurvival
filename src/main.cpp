#include <iostream>
#include "core/Application.h"


int main()
{
	std::cout << "arranca el programa";
	Application game;
	std::cout << "antes de ejectutar el programa";
	game.run();
	std::cout << "despues de ejectutar el programa";
}