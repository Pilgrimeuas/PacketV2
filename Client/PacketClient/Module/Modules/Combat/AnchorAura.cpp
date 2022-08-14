#include "AnchorAura.h"

#include "../../../../SDK/Tag.h"

AnchorAura::AnchorAura() : IModule(VK_NUMPAD0, Category::COMBAT, "Improved By Dimpless") {
	this->registerIntSetting("range", &this->range, this->range, 3, 10);
	this->registerIntSetting("anchorDelay", &this->anchordelay, this->anchordelay, 0, 10);
	this->registerIntSetting("gsDelay", &this->gsdelay, this->gsdelay, 0, 10);
	this->registerIntSetting("Break delay", &this->breakdelay, this->breakdelay, 0, 10);
	this->registerBoolSetting("AppleCan", &this->airplace, this->airplace);
	this->registerBoolSetting("Spoof", &this->spoof, this->spoof);
}
AnchorAura::~AnchorAura() {
}
const char* AnchorAura::getModuleName() {
	return ("重生锚炸头");
}

static std::vector<C_Entity*> targetList;

void AnchorAura::onEnable() {
	targetList.clear();
}

bool Findentbyc(C_Entity* curEnt, bool isRegularEntity) {
	std::string Enzo = "Enzo6172";
	std::string hell = "hellorogic";
	std::string TargetUtilName = curEnt->getNameTag()->getText();
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;  // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;  // falling block
	if (curEnt->getEntityTypeId() == 64) return false;  // item
	if (curEnt->getEntityTypeId() == 69) return false;  // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (!TargetUtil::isValidTarget(curEnt)) return false;

	float dist = (*curEnt->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist <= moduleMgr->getModule<AnchorAura>()->range) {
		targetList.push_back(curEnt);
		return true;
	}
	return false;
}
bool AnchorAura::anchorspoof() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == -272) {
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
bool AnchorAura::gsspoof() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId == 89) {
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
void stopSp() {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	C_MobEquipmentPacket a(id, *g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return;
}
bool placeBlok2(vec3_t blkPlacement) {
	blkPlacement = blkPlacement.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blkPlacement);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		bool mutil = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}
		C_Block* blocck = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blok));
		g_Data.getCGameMode()->buildBlock(&blok, i);
		/*for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}*/

	}
	return false;
}

/*void charge2(vec3_t gsplace) {
	gsplace = gsplace.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(gsplace));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(gsplace);
		int i = 0;
			g_Data.getCGameMode()->buildBlock(&blok, i);  // place on face with respawn anchor
			return;
	}
	return;
}*/
void AnchorAura::charge2(C_GameMode* gm, vec3_t* pos) {
	int i = 0;
	bool sb = false;
	if (!sb) {
#pragma warning(push)
#pragma warning(disable : 4244)
		vec3_ti blockPos = vec3_ti(pos->x, pos->y, pos->z);
		vec3_ti upperBlockPos = vec3_ti(pos->x, pos->y - 1, pos->z);
#pragma warning(pop)
		C_Block* block = gm->player->region->getBlock(blockPos);
		C_Block* upperBlock = gm->player->region->getBlock(upperBlockPos);
		gm->buildBlock(&blockPos, g_Data.getLocalPlayer()->pointingStruct->blockSide);
		return;
	}
	vec3_ti bestPos;
	bool ValidPos = false;
	for (int x = (int)pos->x - 0.5; x < pos->x + 0.5; x++) {
		for (int z = (int)pos->z - 0.5; z < pos->z + 0.5; z++) {
			for (int y = (int)pos->y - 0.5; y < pos->y + 0.5; y++) {
				vec3_ti blockPos = vec3_ti(x - 0.5, y, z - 0.5);
				vec3_ti upperBlockPos = vec3_ti(x - 0.5, y + 1, z - 0.5);
				C_Block* block = gm->player->region->getBlock(blockPos);
				C_Block* upperBlock = gm->player->region->getBlock(upperBlockPos);
				if (block != nullptr) {
					auto blockId = block->toLegacy()->blockId;
					if (blockId != 0) {  // Check for awailable block
						if (!ValidPos) {
							ValidPos = true;
							bestPos = blockPos;
						}
						else if (blockPos.toVector3().dist(*pos) < bestPos.toVector3().dist(*pos)) {
							bestPos = blockPos;
						}
					}
				}
			}
		}
	}
	C_Block* blocck = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(bestPos));
	vec3_ti blok(bestPos);
	if (ValidPos) {
		g_Data.getCGameMode()->buildBlock(&blok, i);
	}
	else {
		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
		}

		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		g_Data.getCGameMode()->buildBlock(&blok, i);
		return;
	}
}
bool SilentSwap3(int Itemid) {
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if ((*stack->item)->itemId != 0) {
				if (stack->getItem()->itemId == Itemid) {
					C_MobEquipmentPacket a(id, *stack, n, n);
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
					return true;
				}
			}
		}
	}
	return false;
}
void getAnchor2() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == -272)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == -272) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}


void getGS2() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 89)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 89) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}


void AnchorAura::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	targetList.clear();
	g_Data.forEachEntity(Findentbyc);

	int place = 0;

	if (!targetList.empty()) {
		for (auto& i : targetList) {
			vec3_t enemyLoc = (i->eyePos0).floor();
			bottom2 = enemyLoc.add(1, 0, 0);
			mid1 = enemyLoc.add(0, 1, 0);
			bottom3 = enemyLoc.add(-1, 0, 0);
			bottom4 = enemyLoc.add(0, 0, 1);
			bottom1 = enemyLoc.add(0, 0, -1);
			//bottom1 = enemyLoc.add(0, 1, 0);
			if (!hasPlacedAnchor) {
				// NOT placed anchor
				if (!takenAnchor) {
					if (spoof)
						//SilentSwap3(-272);
						anchorspoof();
					else
						getAnchor2();
					takenAnchor = true;
					return;
				}


				if (g_Data.getLocalPlayer()->region->getBlock(mid1)->toLegacy()->blockId == 0 || g_Data.getLocalPlayer()->region->getBlock(bottom1)->toLegacy()->blockId == 0) {
					for (auto& i : targetList)
						if (spoof) {
							gm->buildBlock(&vec3_ti(mid1), 0);
							gm->buildBlock(&vec3_ti(bottom1), 0);
							gm->buildBlock(&vec3_ti(bottom2), 0);
							gm->buildBlock(&vec3_ti(bottom3), 0);
							gm->buildBlock(&vec3_ti(bottom4), 0);
						}
						else {
							gm->buildBlock(&vec3_ti(mid1), 0);
						}
					Option = 1;
					if (spoof) {
						stopSp();
					}
				}
				//stopSp();
				hasPlacedAnchor = true;
				//clientMessageF("Placed anchor!");
			}

			if (tickTimer >= anchordelay && !DhasPlacedAnchor) {
				tickTimer = 0;
				DhasPlacedAnchor = true;
			}
			else if (tickTimer < anchordelay && !DhasPlacedAnchor) {
				tickTimer++;
				return;
			}

			if (!hasCharged) {
				if (!takenGS) {
					if (spoof)
						//getGS2();
						//SilentSwap3(89);
						gsspoof();
					else
						getGS2();
					takenGS = true;
					return;
				}

				switch (Option) {
				case 1:

					bool sb = g_Data.getLocalPlayer()->region->getBlock(mid1)->toLegacy()->blockId != 0;
					bool sb2 = g_Data.getLocalPlayer()->region->getBlock(bottom1)->toLegacy()->blockId != 0;
					if (sb || sb2) {
						if (spoof) {
							gm->buildBlock(&vec3_ti(mid1), 0);
							gm->buildBlock(&vec3_ti(bottom1), 0);
							gm->buildBlock(&vec3_ti(bottom2), 0);
							gm->buildBlock(&vec3_ti(bottom3), 0);
							gm->buildBlock(&vec3_ti(bottom4), 0);
						}
						else {
							gm->buildBlock(&vec3_ti(mid1), 0);
						}
						stopSp();
					}
					break;
				}
				if (spoof) {
					stopSp();
				}

				hasCharged = true;
				takenAnchor = false;
				//clientMessageF("Charging!");
			}

			if (tickTimer >= gsdelay && !DhasCharged) {
				tickTimer = 0;
				DhasCharged = true;
			}
			else if (tickTimer < gsdelay && !DhasCharged) {
				tickTimer++;
				return;
			}

			if (!takenAnchor) {
				if (spoof)
					//SilentSwap3(-272);
					anchorspoof();
				else
					getAnchor2();
				//getAnchor2();
				takenAnchor = true;
				return;
			}

			if (!hasDetonated) {
				switch (Option) {
				case 1:
					if (spoof) {
						gm->buildBlock(&vec3_ti(mid1), 0);
						gm->buildBlock(&vec3_ti(bottom1), 0);
						gm->buildBlock(&vec3_ti(bottom2), 0);
						gm->buildBlock(&vec3_ti(bottom3), 0);
						gm->buildBlock(&vec3_ti(bottom4), 0);
					}
					else {
						gm->buildBlock(&vec3_ti(mid1), 0);
					}
					//stopSp();
					//gm->buildBlock(&vec3_ti(bottom1), 0);
					break;
				}if (spoof) {
					stopSp();
				}

				hasDetonated = true;
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
				//stopSp();
				return;
			}
			else {
				tickTimer++;
				return;
			}
		}
	}
}
void AnchorAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
}

void AnchorAura::onDisable() {
}