#pragma once
#pragma once
#include "../Module.h"

class kkk : public IModule {
private:
	int health;
	std::map<std::string, int> enchantMap;


public:

	C_MoveInputHandler* inputHandler = nullptr;
	kkk();
	~kkk();
	bool go = false;

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};