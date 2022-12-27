#include "Dupe.h"

#include "../../../../SDK/Tag.h"

Dupe::Dupe() : IModule(VK_NUMPAD0, Category::COMBAT, "DUPE BY DIMPLES") {
	this->registerIntSetting("range", &this->range, this->range, 3, 10);
	this->registerIntSetting("pixace", &this->pdelay, this->pdelay, 0, 10);
	this->registerIntSetting("box", &this->bdelay, this->bdelay, 0, 10);
	this->registerIntSetting("Nothing", &this->breakdelay, this->breakdelay, 0, 10);
	this->registerBoolSetting("SB", &this->airplace, this->airplace);
}
Dupe::~Dupe() {
}
const char* Dupe::getModuleName() {
	return ("Dupe");
}
void getP() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 318 || g_Data.getLocalPlayer()->getSelectedItemId() == 606)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 318 || stack->getItem()->itemId == 606) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
static std::vector<C_Entity*> targetList;

void Dupe::onEnable() {
	targetList.clear();
}

void getB() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 205 || g_Data.getLocalPlayer()->getSelectedItemId() == 218)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 205 || stack->getItem()->itemId == 218) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
void Dupe::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	int place = 0;
	vec3_t EZ;
	EZ = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
	bottom1 = EZ.add(-1, 1, -1);

	if (!hasPlacedAnchor) {
		// NOT placed anchor
		if (!takenAnchor) {
			getB();
			takenAnchor = true;
			return;
		}


		if (g_Data.getLocalPlayer()->region->getBlock(bottom1)->toLegacy()->blockId == 0) {
			gm->buildBlock(&vec3_ti(bottom1), 0);
			Option = 1;
		}
		hasPlacedAnchor = true;
		//clientMessageF("Placed anchor!");
	}

	if (tickTimer >= bdelay && !DhasPlacedAnchor) {
		tickTimer = 0;
		DhasPlacedAnchor = true;
	}
	else if (tickTimer < bdelay && !DhasPlacedAnchor) {
		tickTimer++;
		return;
	}

	if (!hasCharged) {
		if (!takenGS) {
			getP();
			takenGS = true;
			return;
		}

		switch (Option) {
		case 1:

			bool sb = g_Data.getLocalPlayer()->region->getBlock(bottom1)->toLegacy()->blockId != 0;
			if (sb) {
				bool isDestroyed = false;
				gm->startDestroyBlock(bottom1, 1, isDestroyed);
				gm->stopDestroyBlock(bottom1);
				gm->destroyBlock(&vec3_ti(bottom1), 0);
			}
			break;
		}

		hasCharged = true;
		takenAnchor = false;
		//clientMessageF("Charging!");
	}

	if (tickTimer >= pdelay && !DhasCharged) {
		tickTimer = 0;
		DhasCharged = true;
	}
	else if (tickTimer < pdelay && !DhasCharged) {
		tickTimer++;
		return;
	}

	if (!takenAnchor) {
		getB();
		takenAnchor = true;
		return;
	}

	//clientMessageF("Detonated!");

	if (tickTimer >= breakdelay) {  // reset variables; prepare for another loop
		hasPlacedAnchor = false;
		hasCharged = false;
		hasDetonated = false;
		DhasPlacedAnchor = false;
		DhasCharged = false;
		tickTimer = 0;
		takenAnchor = false;
		takenGS = false;
		return;
	}
	else {
		tickTimer++;
		return;
	}
}

void Dupe::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	DrawUtils::setColor(.1f, .75f, 1.f, 1.f);
	DrawUtils::drawBox(bottom1.sub(0, 1, 0), bottom1.add(1, 0, 1), .4f, false);
}

void Dupe::onDisable() {
}