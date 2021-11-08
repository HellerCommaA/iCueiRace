#pragma once

#include "iCueiRace.h"
#include "EC.h"

#include "CUESDK.h"

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <unordered_map>
#include <cmath>

class LightingManager
{
public:
	LightingManager();
	~LightingManager();

	void redFlag();
	void yellowFlag();
	void greenFlag();
	void whiteFlag();
	void blueFlag();
	void blackFlag();

	EC init();
	void close();
	void DEBUG_LEDS();

private:
	std::atomic<bool> THREAD_INTERRUPT;
	std::atomic<bool> mChanged;
	void threadMain();
	std::thread mThread;
	std::unordered_map<int, std::vector<CorsairLedColor*> *> *mKeys;
};

