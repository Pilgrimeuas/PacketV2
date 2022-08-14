#include "SelfTrap.h"

#include "../../../../Utils/Logger.h"
#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"

SelfTrap::SelfTrap() : IModule(0x0, Category::COMBAT, "Trap Your Self") {
	registerBoolSetting("PitchUp", &this->ATRots, this->ATRots);
	registerBoolSetting("Switch2Obby", &this->switch2obby, this->switch2obby);
	registerBoolSetting("Echest", &this->ec, this->ec);
	registerBoolSetting("Only Head", &this->sb, this->sb);
	registerBoolSetting("PuaseOnGap", &this->airplace, this->airplace);
	registerBoolSetting("Hold", &this->hold, this->hold);
}
SelfTrap::~SelfTrap() {
}

const char* SelfTrap::getModuleName() {
	return ("围住我自己");
}
bool SelfTrap::isHoldMode() {
	return hold;
}
void getobbbb() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 49) {  // select obsid
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
void getec() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 130) {  // select obsid
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
bool SelfTrap::getobSpoof() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 49) {
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
bool SelfTrap::getespoof() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 130) {
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
void stopsp1() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return;
}
void SelfTrap::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 607)
		return;
	if (airplace)
		if (g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot)->getItem()->isFood() && g_Data.isRightClickDown()) return;
	auto selectedItem = g_Data.getLocalPlayer()->getSelectedItem();
	if ((selectedItem == nullptr || selectedItem->count == 0 || selectedItem->item == nullptr))  // Block in hand?
		return;
	vec3_t EZ;
	EZ = g_Data.getLocalPlayer()->eyePos0;  // Block below the player
	upper = EZ.add(-1, 1, 0);
	b1 = EZ.add(0, -1, 0);
	b2 = EZ.add(-2, -1, 0);
	b3 = EZ.add(-1, -1, 1);
	b4 = EZ.add(-1, -1, -1);
	if (switch2obby)
		getobSpoof();
	else if (ec)
		getespoof();
	if (sb) {
		if (g_Data.getLocalPlayer()->region->getBlock(upper)->toLegacy()->blockId == 0)
			gm->buildBlock(&vec3_ti(upper), 0);
		gm->buildBlock(&vec3_ti(b1), 0);
		gm->buildBlock(&vec3_ti(b2), 0);
		gm->buildBlock(&vec3_ti(b3), 0);
		gm->buildBlock(&vec3_ti(b4), 0);
		stopsp1();
	}
	else
		gm->buildBlock(&vec3_ti(b1), 0);
	gm->buildBlock(&vec3_ti(b2), 0);
	gm->buildBlock(&vec3_ti(b3), 0);
	gm->buildBlock(&vec3_ti(b4), 0);
	stopsp1();
}