#pragma once
#pragma once
#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class BurrowWTF : public IModule {
private:
	int bdelay = 1;
	int pdelay = 3;
	int breakdelay = 2;
	bool airplace = true;
	bool hold = false;

	bool hastp = false;
	bool hasbuild = false;
	bool hasCharged = false;
	bool Dhastp = false;
	bool Dhasbuild = false;
	int tickTimer = 0;
	bool takenAnchor = false;
	bool takenGS = false;
	int Option = 99;

	//ground level  ~~~ funni initializations
	vec3_t bottom1;
	vec3_t bottom2;
	vec3_t bottom3;
	vec3_t bottom4;
	//top level
	vec3_t neckBreaker;
	//middle level  ~ last priority because it is the least effective
	vec3_t mid1;
	vec3_t mid2;
	vec3_t mid3;
	vec3_t mid4;

public:
	BurrowWTF();
	~BurrowWTF();

	int range = 10;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPlayerTick(C_Player* plr) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
	bool isHoldMode();
	virtual void onDisable() override;
};