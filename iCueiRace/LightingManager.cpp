#include "LightingManager.h"
#include "CUESDK.h"

#include <iostream>
#include <atomic>
#include <thread>
#include <vector>
#include <unordered_set>
#include <cmath>
#include <Windows.h>
#include <unordered_map>

LightingManager::LightingManager() :
	THREAD_INTERRUPT(false), mChanged(false)
{

}

LightingManager::~LightingManager()
{
	for (std::pair<int, std::vector<CorsairLedColor*>*> each : (*mKeys)) {
		std::vector<CorsairLedColor*> *leds = each.second;
		for (CorsairLedColor *led : *leds) {
			if (led) delete led;
		}
		delete leds;
	}
	if (mKeys) delete mKeys;
}

void LightingManager::close()
{
	THREAD_INTERRUPT = true;
	while (mThread.joinable()) mThread.join();
}

EC LightingManager::init()
{ 
	LOGD("Entry");
	CorsairPerformProtocolHandshake();
	if (const auto error = CorsairGetLastError()) {
		LOGE("Handshake Failed");
		return EC::NotOK;
	}

	// std::unordered_map<int, std::vector<CorsairLedColor>>
	mKeys = new std::unordered_map<int, std::vector<CorsairLedColor*>*>();
	for (auto deviceIndex = 0; deviceIndex < CorsairGetDeviceCount(); deviceIndex++) {
		if (auto ledPositions = CorsairGetLedPositionsByDeviceIndex(deviceIndex)) {
			std::vector<CorsairLedColor*> *keys = new std::vector<CorsairLedColor*>();
			for (auto i = 0; i < ledPositions->numberOfLed; i++) {
				CorsairLedColor *s = (CorsairLedColor*) malloc(sizeof(CorsairLedColor));
				s->ledId = ledPositions->pLedPosition[i].ledId;
				s->r = 255;
				s->g = 255;
				s->b = 255;
				keys->push_back(s);
			}
			mKeys->emplace(deviceIndex, keys);
		}
	}

	mThread = std::thread(&LightingManager::threadMain, this);
	
	return EC::OK;
}


void LightingManager::redFlag()
{
	LOGD("red flag");
	for (std::pair<int, std::vector<CorsairLedColor*>*> each : *mKeys) {
		std::vector<CorsairLedColor*> *leds = each.second;
		for (CorsairLedColor *led : *leds) {
			led->r = 255;
			led->g = 0;
			led->b = 0;
		}
	}
	mChanged = true;
}

void LightingManager::yellowFlag()
{
	LOGD("Yellow flag");
	for (std::pair<int, std::vector<CorsairLedColor*>*> each : *mKeys) {
		std::vector<CorsairLedColor*>* leds = each.second;
		for (CorsairLedColor* led : *leds) {
			led->r = 255;
			led->g = 255;
			led->b = 0;
		}
	}
	mChanged = true;
}

void LightingManager::greenFlag()
{
	LOGD("Green flag");
	for (std::pair<int, std::vector<CorsairLedColor*>*> each : *mKeys) {
		std::vector<CorsairLedColor*>* leds = each.second;
		for (CorsairLedColor* led : *leds) {
			led->r = 0;
			led->g = 255;
			led->b = 0;
		}
	}
	mChanged = true;
}

void LightingManager::whiteFlag()
{
	LOGD("White flag");
	for (std::pair<int, std::vector<CorsairLedColor*>*> each : *mKeys) {
		std::vector<CorsairLedColor*>* leds = each.second;
		for (CorsairLedColor* led : *leds) {
			led->r = 255;
			led->g = 255;
			led->b = 255;
		}
	}
	mChanged = true;
}

void LightingManager::blueFlag()
{
	LOGD("Blue flag");
	for (std::pair<int, std::vector<CorsairLedColor*>*> each : *mKeys) {
		std::vector<CorsairLedColor*>* leds = each.second;
		for (CorsairLedColor* led : *leds) {
			led->r = 0;
			led->g = 0;
			led->b = 255;
		}
	}
	mChanged = true;
}

void LightingManager::blackFlag()
{
	LOGD("black flag");
	for (std::pair<int, std::vector<CorsairLedColor*>*> each : *mKeys) {
		std::vector<CorsairLedColor*>* leds = each.second;
		for (CorsairLedColor* led : *leds) {
			led->r = 100;
			led->g = 100;
			led->b = 100;
		}
	}
	mChanged = true;
}

void LightingManager::DEBUG_LEDS()
{
	for (std::pair<int, std::vector<CorsairLedColor*>*> each : *mKeys) {
		std::vector<CorsairLedColor*>* leds = each.second;
		for (CorsairLedColor* led : *leds) {
			led->r = 0;
			led->g = 0;
			led->b = 0;
		}
	}
}

void LightingManager::threadMain()
{
	if (mKeys->empty()) {
		LOGE("Colors empty!!!");
		return;
	}

	while (!THREAD_INTERRUPT) {
		if (mChanged) {
			for (auto each : *mKeys) {
				auto idx = each.first;
				auto leds = each.second;
				for (auto led : *leds) {
					CorsairSetLedsColorsBufferByDeviceIndex(idx, 1, led);
				}
			}
			if (!CorsairSetLedsColorsFlushBuffer()) {
				LOGE("Could not set colors to buffer");
			}
			mChanged = false;
		}		
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}
