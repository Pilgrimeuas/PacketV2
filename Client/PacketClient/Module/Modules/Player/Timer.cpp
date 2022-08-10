#include "Timer.h"
#include "../pch.h"

Timer::Timer() : IModule(0, Category::PLAYER, "Modifies the games speed") {
	registerIntSetting("TPS", &timer, timer, 1, 500);
}

const char* Timer::getModuleName() {
	return ("变速齿轮");
}

void Timer::onTick(C_GameMode* gm) {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
}

void Timer::onDisable() {
	g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
}