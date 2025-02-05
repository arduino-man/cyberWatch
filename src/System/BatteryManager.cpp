#include "config.h"

#include "BatteryManager.h"

#include <LilyGoWatch.h>
#include <WiFi.h>

BatteryManager* BatteryManager::inst;

BatteryManager *BatteryManager::getInstance() {

	if (BatteryManager::inst == nullptr) {
		BatteryManager::inst = new BatteryManager();
	}
	return BatteryManager::inst;
}

uint8_t BatteryManager::getCapacity() {
	int capacity = (int)(TTGOClass::getWatch()->power->getBattVoltage()	- BATTERY_LOW_CAPACITY) * 100 / (BATTERY_FULL_CAPACITY - BATTERY_LOW_CAPACITY);
	capacity = min(100, capacity);	//prevent to return capacity over 100% (calculation inaccurancy)
	capacity = max(0, capacity); //prevent to return capacity less then 0 (calculation inaccurancy)
	return (uint8_t)capacity;
}

bool BatteryManager::isCharging() {
	return TTGOClass::getWatch()->power->isChargeing();
}

bool BatteryManager::handleCabelPlugInIRQ() {
	return (
		TTGOClass::getWatch()->power->isVbusPlugInIRQ()
		|| TTGOClass::getWatch()->power->isAcinPlugInIRQ()
	);
}

bool BatteryManager::handleCabelPlugRemoveIRQ() {
	return (
		TTGOClass::getWatch()->power->isVbusRemoveIRQ()
		|| TTGOClass::getWatch()->power->isAcinRemoveIRQ()
	);
}

void BatteryManager::energyConsumptionSavingsSettings() {
	WiFi.disconnect(true);	// Disconnect from the network
	WiFi.mode(WIFI_OFF);			// Switch WiFi off //not need to wifi on -> power saving
	setCpuFrequencyMhz(CPU_FREQUENCY);
}


BatteryManager::BatteryManager() {
}
