#include "Xray.h"
#include "../pch.h"

Xray::Xray() : IModule(0, Category::VISUAL, "Allows you to see through walls") {
}

const char* Xray::getModuleName() {
	return ("X光透视");
}

void Xray::onEnable() {
	if (smoothLightningSetting != nullptr) {
		if (!gotSmoothInfo) {
			gotSmoothInfo = true;
			wasSmooth = *smoothLightningSetting;
		}
		*smoothLightningSetting = 0;
	}
}

void Xray::onTick(C_GameMode* gm) {
	onEnable(); // yes lol
}

void Xray::onDisable() {
	if (smoothLightningSetting != nullptr && gotSmoothInfo) {
		*smoothLightningSetting = wasSmooth;
	}
	gotSmoothInfo = false;
}