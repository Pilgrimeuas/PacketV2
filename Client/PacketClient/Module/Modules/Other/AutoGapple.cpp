#include "AutoGapple.h"

AutoGapple::AutoGapple() : IModule(0, Category::PLAYER, "Auto eat gapples if you're low health") {
	registerIntSetting("health", &health, 10, 1, 20);
}

AutoGapple::~AutoGapple() {
}

const char* AutoGapple::getModuleName() {
	return ("AutoGapple");
}

void AutoGapple::onTick(C_GameMode* gm) {
	int healthy = g_Data.getLocalPlayer()->getHealth();
	int origSlot;
	auto inv = g_Data.getLocalPlayer()->getSupplies()->inventory;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	auto prevSlot = supplies->selectedHotbarSlot;
	origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	if (healthy < health) {
		for (int i = 0; i < 9; i++) {
			C_ItemStack* stack = inv->getItemStack(i);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 259) {
					if (prevSlot != i)
						supplies->selectedHotbarSlot = i;
					g_Data.getCGameMode()->useItem(*stack);
					return;
				}
			}
		}
	}

}