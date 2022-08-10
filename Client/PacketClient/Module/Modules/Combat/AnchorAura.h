#pragma once
#pragma once
#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include "../Module.h"

class AnchorAura : public IModule {
private:
	int anchordelay = 1;
	int gsdelay = 3;
	int breakdelay = 2;
	bool airplace = true;
	bool spoof = false;

	bool hasDetonated = false;
	bool hasPlacedAnchor = false;
	bool hasCharged = false;
	bool DhasPlacedAnchor = false;
	bool DhasCharged = false;
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
	AnchorAura();
	~AnchorAura();

	int range = 10;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	void charge2(C_GameMode* gm, vec3_t* pos);
	virtual void onTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
	virtual void onEnable() override;
	bool anchorspoof();
	bool gsspoof();
	virtual void onDisable() override;
};