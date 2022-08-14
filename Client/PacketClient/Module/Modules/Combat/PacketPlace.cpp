#include "PacketPlace.h"
#include "../pch.h"

PacketPlace::PacketPlace() : IModule(0, Category::COMBAT, "Automatically clicks for you") {
	registerBoolSetting("Spoof", &weapons, weapons);
	registerBoolSetting("Hold", &hold, hold);
	registerIntSetting("MaxCPS", &maxCPS, maxCPS, 1, 20);
	registerIntSetting("MinCPS", &minCPS, minCPS, 1, 20);
}

const char* PacketPlace::getModuleName() {
	return ("金苹果放置");
}
bool PacketPlace::getobSpoof4() {
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
void stopsp114() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return;
}
void PacketPlace::onPlayerTick(C_Player* plr) {
	if (plr == nullptr) return;

	/*	auto selectedItem = plr->getSelectedItem();
		if (weapons && selectedItem->getAttackingDamageWithEnchants() < 1)
			return;*/

	if (hold && !g_Data.isWheelDown())
		return;

	if (maxCPS < minCPS) maxCPS = minCPS;
	CPS = randomFloat(maxCPS, minCPS);


}

void PacketPlace::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	if (g_Data.getLocalPlayer()->getSelectedItemId() != 259)
		return;
	/*	auto selectedItem = player->getSelectedItem();
		if (weapons && selectedItem->getAttackingDamageWithEnchants() < 1)
			return;*/

	if (hold && !g_Data.isWheelDown())
		return;

	if (g_Data.canUseMoveKeys() && !moduleMgr->getModule<ClickGUIMod>()->isEnabled()) {
		if (weapons)
			getobSpoof4();

		PointingStruct* pointing = player->pointingStruct;
		bool isDestroyed = false;

		g_Data.leftclickCount++;
		if (!moduleMgr->getModule<NoSwing>()->isEnabled()) player->swing();
		if (pointing->hasEntity())
			return;
		//PointingStruct* pstruct = g_Data.getLocalPlayer()->getPointingStruct();
		int id = g_Data.getLocalPlayer()->region->getBlock(pointing->block)->toLegacy()->blockId;
		if (id != 0)
		{
			//gm->startDestroyBlock(pointing->block, pointing->blockSide, isDestroyed);
			gm->buildBlock(&pointing->block, 0);
		}
		if (weapons)
			stopsp114();
		/*PointingStruct* pstruct = g_Data.getLocalPlayer()->getPointingStruct();
int id = g_Data.getLocalPlayer()->region->getBlock(pstruct->block)->toLegacy()->blockId;*/
	}

}