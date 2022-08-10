#include "BurrowWTF.h"

#include "../../../../SDK/Tag.h"

BurrowWTF::BurrowWTF() : IModule(VK_NUMPAD0, Category::COMBAT, "BurrowWTF BY DIMPLES") {
	this->registerIntSetting("range", &this->range, this->range, 3, 10);
	this->registerIntSetting("TPDelay", &this->pdelay, this->pdelay, 0, 20);
	this->registerIntSetting("ObsdianDelay", &this->bdelay, this->bdelay, 0, 10);
	this->registerIntSetting("Nothing", &this->breakdelay, this->breakdelay, 0, 10);
	this->registerBoolSetting("Aurora", &this->airplace, this->airplace);
	this->registerBoolSetting("hold", &this->hold, this->hold);
}
BurrowWTF::~BurrowWTF() {
}
const char* BurrowWTF::getModuleName() {
	return ("防止炸头");
}
void getOb() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 49)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 49) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
static std::vector<C_Entity*> targetList;

void BurrowWTF::onEnable() {
	targetList.clear();
}
bool BurrowWTF::isHoldMode() {
	return hold;
}

void getBo() {
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
void BurrowWTF::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	int place = 0;
	vec3_t EZZ;
	EZZ = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
	bottom1 = EZZ.add(0, 3, 0);
	bottom2 = EZZ.add(-1, 0, 0);
	bottom3 = EZZ.add(-1, -1, 0);
	bottom4 = EZZ.add(-1, -1, 0);
	vec3_t blockBelow;
	blockBelow = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
	bottom2.y = EZZ.y;
	int delay = 0;
	pdelay++;
	if (pdelay >= delay) {
		gm->player->setPos(bottom1);
		pdelay = 0;
	}
	if (airplace) {
		getOb();
		bdelay++;
		if (bdelay >= delay) {
			gm->buildBlock(&vec3_ti(bottom2), 0);
			bdelay = 0;
		}
	}
}


void BurrowWTF::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
}
void BurrowWTF::onPlayerTick(C_Player* plr) {
	if (airplace) {
		plr->pitch = -90;
	}
}

void BurrowWTF::onDisable() {
}