#include "player.h"
#include <iostream>
#include <stdexcept>
#include <string>

#undef main

#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")


int main() {
	try {
		/*if (argc != 2) {
			throw std::logic_error{"Not enough arguments"};
		}*/

		Player play{"n.mp4"};
		play();

	}

	catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}
