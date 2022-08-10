#pragma once
#include "../Module.h"
class AutoTrap : public IModule {
private:
	bool tryAutoTrap(vec3_t AutoTrap);
	bool ATRots = true;
	bool fullbody = false;
	bool niggerTurnCAOn = false;
	bool switch2obby = true;
	bool airplace = false;
	//bool spoof = false;

	/* GROUND   FEET     HEAD
	   0 0 N	0 0 N    N N N
	   0 0 0    0 0 0    N 0 N
	   0 0 0	0 0 0    0 N N
	*/

	std::vector<vec3_t> placements;

public:
	bool mustGoUp = false;

	AutoTrap();
	~AutoTrap();

	int range = 7;
	bool onClick = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	bool getobby2();
	//bool getCrystalSpoof();
	virtual void onTick(C_GameMode* gm) override;
};
