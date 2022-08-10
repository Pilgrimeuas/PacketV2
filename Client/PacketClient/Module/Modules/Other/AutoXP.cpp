#include "AutoXP.h"

AutoXP::AutoXP() : IModule(0, Category::PLAYER, "Auto eat gapples if you're low health") {
	registerIntSetting("minhealth", &health, 10, 1, 20);
}

AutoXP::~AutoXP() {
}

const char* AutoXP::getModuleName() {
	return ("修甲");
}
bool AutoXP::getxp() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 68) {
				C_MobEquipmentPacket a(id, *stack, n, n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				return true;
			}
		}
	}
	//C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	//g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;
}
void stopSpoofing4() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return;
}
void AutoXP::onTick(C_GameMode* gm) {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	getxp();
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 68) {
				g_Data.getCGameMode()->useItem(*stack);

			}
		}
	}
}
//int healthy = g_Data.getLocalPlayer()->getHealth();
//int origSlot;
//auto inv = g_Data.getLocalPlayer()->getSupplies()->inventory;
//C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
//auto prevSlot = supplies->selectedHotbarSlot;
//origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
/*		for (int i = 0; i < 9; i++) {
			C_ItemStack* stack = inv->getItemStack(i);
			if (stack->item != nullptr) {
				if (stack->getItem()->itemId == 259) {
					if (prevSlot != i)
						supplies->selectedHotbarSlot = i;
					g_Data.getCGameMode()->useItem(*stack);
					return;
			}
		}*/


