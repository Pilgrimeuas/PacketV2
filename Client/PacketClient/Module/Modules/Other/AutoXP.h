#pragma once
#include "../Module.h"

class AutoXP : public IModule {
private:
	int health;
	bool hasclick = false;

public:
	C_MoveInputHandler* inputHandler = nullptr;
	AutoXP();
	~AutoXP();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	bool getxp();
	virtual void onTick(C_GameMode* gm) override;
};
