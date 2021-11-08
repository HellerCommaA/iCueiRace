
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <Windows.h>

#include "iCueiRace.h"
#include "LightingManager.h"


int main() {

	EC ec;

	LightingManager *mLightManager = new LightingManager();
	ec = mLightManager->init();
	if (ec == EC::NotOK) {
		printf("Unable to init LightingManager\n");
		return -1;
	}

	LOGD("Init OK");
	
	mLightManager->whiteFlag();

	char ch = getchar();
	while (ch != 'q') {
		switch (ch) {
		case 'g': mLightManager->greenFlag();
			std::cout << "Green" << std::endl;
			break;
		case 'b': mLightManager->blueFlag();
			std::cout << "Blue" << std::endl;
			break;
		case 'r': mLightManager->redFlag();
			std::cout << "red" << std::endl;
			break;
		case 'y': mLightManager->yellowFlag();
			std::cout << "yellow" << std::endl;
			break;
		case 'w': mLightManager->whiteFlag();
			std::cout << "white" << std::endl;
			break;
		case 'l': mLightManager->blackFlag();
			std::cout << "black" << std::endl;
		default:
			break;
		}
		ch = getchar();
	}

	mLightManager->close();

	delete mLightManager;

	return 0;
}
