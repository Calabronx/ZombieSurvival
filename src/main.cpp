#include "core/Application.h"

#include <stdexcept>
#include <iostream>


int main()
{
	try
	{
		Application game;
		game.run();

	}
	catch (std::exception& e)
	{
		std::cout << "\nEXCEPTION: " << e.what() << std::endl;
	}
}