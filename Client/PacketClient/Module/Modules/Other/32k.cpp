#include "32k.h"

kkk::kkk() : IModule(0, Category::PLAYER, "666") {
}

kkk::~kkk() {

}

const char* kkk::getModuleName() {
	return ("32k");
}
void kkk::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	enchantMap["protection"] = 0;
	enchantMap["fire_protection"] = 1;
	enchantMap["feather_falling"] = 2;
	enchantMap["blast_protection"] = 3;
	enchantMap["projectile_protection"] = 4;
	enchantMap["thorns"] = 5;
	enchantMap["respiration"] = 6;
	enchantMap["depth_strider"] = 7;
	enchantMap["aqua_affinity"] = 8;
	enchantMap["frost_walker"] = 25;
	enchantMap["sharpness"] = 9;
	enchantMap["smite"] = 10;
	enchantMap["bane_of_arthropods"] = 11;
	enchantMap["knockback"] = 12;
	enchantMap["fire_aspect"] = 13;
	enchantMap["looting"] = 14;
	enchantMap["channeling"] = 32;
	enchantMap["impaling"] = 29;
	enchantMap["loyalty"] = 31;
	enchantMap["riptide"] = 30;
	enchantMap["silktouch"] = 16;
	enchantMap["fortune"] = 18;
	enchantMap["unbreaking"] = 17;
	enchantMap["efficiency"] = 15;
	enchantMap["mending"] = 26;
	enchantMap["power"] = 19;
	enchantMap["punch"] = 20;
	enchantMap["flame"] = 21;
	enchantMap["infinity"] = 22;
	enchantMap["multishot"] = 33;
	enchantMap["quick_charge"] = 35;
	enchantMap["piercing"] = 34;
	enchantMap["luck_of_sea"] = 23;
	enchantMap["lure"] = 24;
	enchantMap["soul_speed"] = 36;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	int selectedSlot = supplies->selectedHotbarSlot;
	C_ItemStack* item = inv->getItemStack(selectedSlot);
	int enchantId = 0;
	int enchantLevel = 32767;
	bool go2 = false;
	if (item == nullptr || item->count == 0 || item->item == NULL) {
		std::string textStr = "Enchant failed (No item in hand)";
		//auto notification = notificationMgr.add("Commands:", textStr);
		return;
	}
	if (item->isEnchanted())
	{
		return;
	}
	else { go = true; }
	if (go) {
		for (int i = 0; i < 38; i++)
			g_Data.getLocalPlayer()->enchantItem(item, i, enchantLevel);
		go2 = true;
	}
	if (go2) {
		g_Data.getLocalPlayer()->getTransactionManager()->addInventoryAction(*new C_InventoryAction(0, item, nullptr, 507, 99999));
		inv->addItemToFirstEmptySlot(item);
		std::string textStr = "Enchant successful!";
		return;
	}
}