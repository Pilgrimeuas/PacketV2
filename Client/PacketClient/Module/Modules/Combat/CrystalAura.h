#pragma once
#include "../Module.h"
class CrystalAura : public IModule {
private:

	bool tryCrystalAura(vec3_t CrystalAura);

public:
	CrystalAura();
	~CrystalAura();

	int range = 7;
	bool onClick = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
