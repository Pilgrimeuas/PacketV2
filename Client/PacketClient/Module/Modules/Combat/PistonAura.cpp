#include "PistonAura.h"

#include "../../../../SDK/Tag.h"

PistonAura::PistonAura() : IModule(VK_NUMPAD0, Category::COMBAT, "LOL PistonAura By Dimples#1337") {
	this->registerIntSetting("range", &this->range, this->range, 3, 10);
	this->registerIntSetting("RedstoneDelay", &this->pdelay, this->pdelay, 0, 30);
	this->registerIntSetting("TrapDelay", &this->trapdelay, this->trapdelay, 0, 30);
	this->registerIntSetting("PistonDelay", &this->rdelay, this->rdelay, 0, 30);
	this->registerIntSetting("CrystalPlace delay", &this->cdelay, this->cdelay, 0, 30);
	this->registerIntSetting("CrystalPlace range", &this->cbrange, this->cbrange, 0, 10);
	this->registerIntSetting("CrystalBreak delay", &this->breakdelay, this->breakdelay, 0, 30);
	this->registerIntSetting("CrystalBreak Range", &this->crange, this->crange, 0, 10);
	this->registerBoolSetting("High Level", &this->airplace, this->airplace);
	this->registerBoolSetting("Bypass", &this->bypass, this->bypass);
	registerEnumSetting("LookAt", &lookat, 0);
	lookat.addEntry("w", 0);
	lookat.addEntry("e", 1);
	lookat.addEntry("s", 2);
	lookat.addEntry("n", 3);
	this->registerBoolSetting("North", &this->n2, this->n2);
	this->registerBoolSetting("South", &this->s2, this->s2);
	this->registerBoolSetting("East", &this->e2, this->e2);
	this->registerBoolSetting("West", &this->w2, this->w2);

}
PistonAura::~PistonAura() {
}
const char* PistonAura::getModuleName() {
	return ("活塞光环");
}

static std::vector<C_Entity*> targetList;

void PistonAura::onEnable() {
	targetList.clear();
}

bool findEntByC(C_Entity* curEnt, bool isRegularEntity) {
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
	if (dist <= moduleMgr->getModule<PistonAura>()->range) {
		targetList.push_back(curEnt);
		return true;
	}
	return false;
}

bool placeBloKKK(vec3_t blkPlacement) {
	blkPlacement = blkPlacement.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blkPlacement);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
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
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i);
			return true;
		}
	}
	return false;
}
bool space2 = true;
vec3_t _pos114514;
bool CanPlaceCCC(vec3_ti* pos) {
	space2 = true;
	_pos114514 = pos->toVector3();
	g_Data.forEachEntity([](C_Entity* ent, bool b) {
		if (!space2)
			return;
		if (ent->aabb.intersects(AABB(_pos114514, _pos114514.add(1.f))))
			space2 = false;
		});
	return space2;
}
void PistonAura::CPlaceSB(C_GameMode* gm, vec3_t* pos) {
	vec3_ti bestPos;
	bool ValidPos = false;
	for (int x = (int)pos->x - crange; x < pos->x + crange; x++) {
		for (int z = (int)pos->z - crange; z < pos->z + crange; z++) {
			for (int y = (int)pos->y - crange; y < pos->y + crange; y++) {
				vec3_ti blockPos = vec3_ti(x, y, z);
				vec3_ti upperBlockPos = vec3_ti(x, y + 1, z);
				C_Block* block = gm->player->region->getBlock(blockPos);
				C_Block* upperBlock = gm->player->region->getBlock(upperBlockPos);
				if (block != nullptr) {
					auto blockId = block->toLegacy()->blockId;
					auto upperBlockId = upperBlock->toLegacy()->blockId;
					if ((blockId == 49 || blockId == 7) && upperBlockId == 0 && CanPlaceCCC(&blockPos)) {  //Check for awailable block;

						if (!ValidPos) {
							ValidPos = true;
							bestPos = blockPos;
						}
					}
				}
			}
		}
	}
	if (ValidPos) {
		bool idk = true;
		gm->buildBlock(&bestPos, 0);
	}
}
bool SilentSwap2(int Itemid) {
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
bool PistonAura::tryPistonAura(vec3_t PistonAura) {
	PistonAura = PistonAura.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(PistonAura));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(PistonAura);
		// Find neighbour
		static std::vector<vec3_ti*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
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
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i);

			return true;
		}
	}

	return false;
}

/*void chargeAnchor(vec3_t gsplace) {
	gsplace = gsplace.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(gsplace));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(gsplace);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = g_Data.getLocalPlayer()->region->getBlock(gsplace)->toLegacy()->blockId == 527;  //only target respawn anchors
			if (Y) {
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i);  // place on face with respawn anchor
			return;
		}
	}
	return;
}*/

void getAnchor() {//get RedStone id 152
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 33 || g_Data.getLocalPlayer()->getSelectedItemId() == 29)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 33 || stack->getItem()->itemId == 29) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

void getGS() {//get Piston 29  //77 is button //143 is button
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 152 || g_Data.getLocalPlayer()->getSelectedItemId() == 76 || g_Data.getLocalPlayer()->getSelectedItemId() == 77 || g_Data.getLocalPlayer()->getSelectedItemId() == 143 || g_Data.getLocalPlayer()->getSelectedItemId() == 395 || g_Data.getLocalPlayer()->getSelectedItemId() == 396 || g_Data.getLocalPlayer()->getSelectedItemId() == 397 || g_Data.getLocalPlayer()->getSelectedItemId() == 398 || g_Data.getLocalPlayer()->getSelectedItemId() == 399)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 152 || stack->getItem()->itemId == 76 || stack->getItem()->itemId == 77 || stack->getItem()->itemId == 143 || stack->getItem()->itemId == 395 || stack->getItem()->itemId == 396 || stack->getItem()->itemId == 397 || stack->getItem()->itemId == 398 || stack->getItem()->itemId == 399) {  // select redstone or more
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
void getCr() {//get Piston 29
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 637)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 637) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
void getAnti() {//get Piston 29
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
void getche() {//get Piston 29 //54 = chest
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
void PistonAura::DestroyC(C_Entity* ent, int crange) {
	if (g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) < crange && bypass) {
		g_Data.getCGameMode()->attack(ent);
		g_Data.getLocalPlayer()->swingArm();
	}
	else if (bypass) {
		for (auto& i : targetList)
			if (ent->getPos()->dist(*i->getPos()) < crange) {
				g_Data.getCGameMode()->attack(ent);
				g_Data.getLocalPlayer()->swingArm();
				return;
			}
	}
}

void PistonAura::onTick(C_GameMode* gm) {
	C_LocalPlayer* player = g_Data.getLocalPlayer();
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	targetList.clear();
	g_Data.forEachEntity(findEntByC);
	int place = 0;
	/* X 360 X   315-45= s
  X X X     45-135 = e
  X X X     135-225 = n   1 =
			225-315 = w*/
	int look = (int)(player->yaw + 180);
	//		if (moduleMgr->getModule<PistonAura>()->lookat.selected != 0)
	//moduleMgr->getModule<PistonAura>()->lookat.selected = 0;
	if (look >= 45 && look <= 135) {
		if (!moduleMgr->getModule<PistonAura>()->w2)
			moduleMgr->getModule<PistonAura>()->w2 = true;//w
		moduleMgr->getModule<PistonAura>()->s2 = false;//e
		moduleMgr->getModule<PistonAura>()->e2 = false;//e
		moduleMgr->getModule<PistonAura>()->n2 = false;//e
	}
	if (look >= 135 && look <= 225) {
		if (!moduleMgr->getModule<PistonAura>()->s2)///s
			moduleMgr->getModule<PistonAura>()->s2 = true;//e
		moduleMgr->getModule<PistonAura>()->e2 = false;//e
		moduleMgr->getModule<PistonAura>()->w2 = false;//e
		moduleMgr->getModule<PistonAura>()->n2 = false;//e
	}
	if (look >= 225 && look <= 315) {
		if (!moduleMgr->getModule<PistonAura>()->e2)//e
			moduleMgr->getModule<PistonAura>()->e2 = true;//e
		moduleMgr->getModule<PistonAura>()->s2 = false;//e
		moduleMgr->getModule<PistonAura>()->w2 = false;//e
		moduleMgr->getModule<PistonAura>()->n2 = false;//e
	}
	if ((look >= 0 && look <= 45) || (look >= 315 && look <= 360)) {
		if (!moduleMgr->getModule<PistonAura>()->n2)//n
			moduleMgr->getModule<PistonAura>()->e2 = false;//e
		moduleMgr->getModule<PistonAura>()->s2 = false;//e
		moduleMgr->getModule<PistonAura>()->w2 = false;//e
		moduleMgr->getModule<PistonAura>()->n2 = true;//e
	}

	if (!targetList.empty()) {
		if (!hasPlacedAnchor) {  // NOT placed anchor
			if (!takenAnchor) {
				getAnchor();//getpiston
				takenAnchor = true;
				return;
			}
			vec3_t enemyLoc = (targetList[0]->eyePos0).floor();
			//x+����
			// x-�Ƕ�
			// z-�Ǳ�
			// z+����
			//ground level
			//restone
			w = enemyLoc.add(2, 1, 0);
			e = enemyLoc.add(-2, 1, 0);
			s = enemyLoc.add(0, 1, 2);
			n = enemyLoc.add(0, 1, -2);
			//restone2
			/* X 360 X   315-45= s
			*  X X X     45-135 = e
			*  X X X     135-225 = n
			*            225-315 = w
			*/
			rw = enemyLoc.add(3, 0, 0);
			re = enemyLoc.add(-3, 0, 0);
			rs = enemyLoc.add(0, 0, 3);
			rn = enemyLoc.add(0, 0, -3);
			//top level

			neckBreaker = enemyLoc.add(0, 1, 0);
			//middle level  ~ last priority because it is the least effective
			// 
			//piston
			w1 = enemyLoc.add(2, 0, 0);
			e1 = enemyLoc.add(-2, 0, 0);
			s1 = enemyLoc.add(0, 0, 2);
			n1 = enemyLoc.add(0, 0, -2);
			//crystal
			cw = enemyLoc.add(1, 0, 0);
			ce = enemyLoc.add(-1, 0, 0);
			cs = enemyLoc.add(0, 0, 1);
			cn = enemyLoc.add(0, 0, -1);
			std::vector<vec2_t> stacking{};
			/*				auto rncheck = g_Data.getLocalPlayer()->region->getBlock(cw)->toLegacy()->blockId;
				if (rncheck == 49 || rncheck == 7)
					airplace = true;
				else
					airplace = false;*/

					//�����ж�

			if (w2) {
				C_Block* block = gm->player->region->getBlock(cw);
				auto blockId = block->toLegacy()->blockId;
				if (blockId == 49 || blockId == 7) {
					airplace = true;
				}
				else
				{
					airplace = false;
				}
				if (airplace)
					gm->buildBlock(&vec3_ti(w), 0);
				else
					gm->buildBlock(&vec3_ti(w1), 0);
				Option = 1;
			}


			if (e2) {
				C_Block* block = gm->player->region->getBlock(ce);
				auto blockId = block->toLegacy()->blockId;
				if (blockId == 49 || blockId == 7) {
					airplace = true;
				}
				else
				{
					airplace = false;
				}
				if (airplace)
					gm->buildBlock(&vec3_ti(e), 0);
				else
					gm->buildBlock(&vec3_ti(e1), 0);
				Option = 2;
			}

			if (s2) {
				C_Block* block = gm->player->region->getBlock(cs);
				auto blockId = block->toLegacy()->blockId;
				if (blockId == 49 || blockId == 7) {
					airplace = true;
				}
				else
				{
					airplace = false;
				}
				if (airplace)
					gm->buildBlock(&vec3_ti(s), 0);
				else
					gm->buildBlock(&vec3_ti(s1), 0);
				Option = 3;
			}

			if (n2) {
				C_Block* block = gm->player->region->getBlock(cn);
				auto blockId = block->toLegacy()->blockId;
				if (blockId == 49 || blockId == 7) {
					airplace = true;
				}
				else
				{
					airplace = false;
				}
				if (airplace)
					gm->buildBlock(&vec3_ti(n), 0);
				else
					gm->buildBlock(&vec3_ti(n1), 0);
				Option = 4;
			}
			hasPlacedAnchor = true;

		}
		/*if (g_Data.getLocalPlayer()->region->getBlock(w)->toLegacy()->blockId == 0) {
			if (airplace)
				placeBloKKK(w);
			else
				gm->buildBlock(&vec3_ti(w), 0, true);
			Option = 1;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(bottom2)->toLegacy()->blockId == 0) {
			if (airplace)
				placeBloKKK(bottom2);
			else
				gm->buildBlock(&vec3_ti(bottom2), 0, true);
			Option = 2;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(bottom3)->toLegacy()->blockId == 0) {
			if (airplace)
				placeBloKKK(bottom3);
			else
				gm->buildBlock(&vec3_ti(bottom3), 0, true);
			Option = 3;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(bottom4)->toLegacy()->blockId == 0) {
			if (airplace)
				placeBloKKK(bottom4);
			else
				gm->buildBlock(&vec3_ti(bottom4), 0, true);
			Option = 4;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(neckBreaker)->toLegacy()->blockId == 0) {
			if (airplace)
				placeBloKKK(neckBreaker);
			else
				gm->buildBlock(&vec3_ti(neckBreaker), 0, true);
			Option = 5;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(mid1)->toLegacy()->blockId == 0) {
			if (airplace)
				placeBloKKK(mid1);
			else
				gm->buildBlock(&vec3_ti(mid1), 0, true);
			Option = 6;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(mid2)->toLegacy()->blockId == 0) {
			if (airplace)
				placeBloKKK(mid2);
			else
				gm->buildBlock(&vec3_ti(mid2), 0, true);
			Option = 7;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(mid3)->toLegacy()->blockId == 0) {
			if (airplace)
				placeBloKKK(mid3);
			else
				gm->buildBlock(&vec3_ti(mid3), 0, true);
			Option = 8;
		}
		else if (g_Data.getLocalPlayer()->region->getBlock(mid4)->toLegacy()->blockId == 0) {
			if (airplace)
				placeBloKKK(mid4);
			else
				gm->buildBlock(&vec3_ti(mid4), 0, true);
			Option = 9;  // god, that was hell writing that shit
		}*/
		//clientMessageF("Placed redstone!");
	}

	if (tickTimer >= rdelay && !DhasPlacedAnchor) {//rdelay = pistondelay
		tickTimer = 0;
		DhasPlacedAnchor = true;
	}
	else if (tickTimer < rdelay && !DhasPlacedAnchor) {
		tickTimer++;
		return;
	}
	if (!hastrap) {
		if (!takentrap) {//redstone
			SilentSwap2(49);
			takentrap = true;
			return;
		}
		C_Block* block2 = gm->player->region->getBlock(neckBreaker.add(0, 1, 0));
		auto bid = block2->toLegacy()->blockId;
		switch (Option) {
			if (bid == 0) {
		case 1:


			if (airplace)
				gm->buildBlock(&vec3_ti(neckBreaker.add(0, 1, 0)), 0);
			else
				gm->buildBlock(&vec3_ti(neckBreaker.add(0, 1, 0)), 0);

			break;
		case 2:
			if (airplace)
				gm->buildBlock(&vec3_ti(neckBreaker.add(0, 1, 0)), 0);
			else
				gm->buildBlock(&vec3_ti(neckBreaker.add(0, 1, 0)), 0);
			break;
		case 3:
			if (airplace)
				gm->buildBlock(&vec3_ti(neckBreaker.add(0, 1, 0)), 0);
			else
				gm->buildBlock(&vec3_ti(neckBreaker.add(0, 1, 0)), 0);
			break;
		case 4:
			if (airplace)
				gm->buildBlock(&vec3_ti(neckBreaker.add(0, 1, 0)), 0);
			else
				gm->buildBlock(&vec3_ti(neckBreaker.add(0, 1, 0)), 0);
			break;
			}
		}
		hastrap = true;
	}
	if (tickTimer >= trapdelay && !Dtrap) {
		tickTimer = 0;
		Dtrap = true;
	}
	else if (tickTimer < pdelay && !Dtrap) {
		tickTimer++;
		return;
	}
	//new start



	if (!hasDetonated) {
		if (!takenRS) {//redstone
			getGS();
			takenRS = true;
			return;
		}
		switch (Option) {
		case 1:


			if (airplace)
				gm->buildBlock(&vec3_ti(rw.add(0, 1, 0)), 0);
			else
				gm->buildBlock(&vec3_ti(rw), 0);
			break;
		case 2:
			if (airplace)
				gm->buildBlock(&vec3_ti(re.add(0, 1, 0)), 0);
			else
				gm->buildBlock(&vec3_ti(re), 0);
			break;
		case 3:
			if (airplace)
				gm->buildBlock(&vec3_ti(rs.add(0, 1, 0)), 0);
			else
				gm->buildBlock(&vec3_ti(rs), 0);
			break;
		case 4:
			if (airplace)
				gm->buildBlock(&vec3_ti(rn.add(0, 1, 0)), 0);
			else
				gm->buildBlock(&vec3_ti(rn), 0);
			break;
		}
		hasDetonated = true;
	}
	if (tickTimer >= pdelay && !DhasDetonated) {
		tickTimer = 0;
		DhasDetonated = true;
	}
	else if (tickTimer < pdelay && !DhasDetonated) {
		tickTimer++;
		return;
	}
	if (!hasCharged) {
		if (!takenGS) {
			getCr();//getCr
			takenGS = true;
			return;
		}
		//			cw = enemyLoc.add(1, 0, 0);
		//ce = enemyLoc.add(-1, 0, 0);
		//cs = enemyLoc.add(0, 0, 1);
		//cn = enemyLoc.add(0, 0, -1);
		//
		//
		//
		switch (Option) {
		case 1:
			if (airplace)
				tryPistonAura(cw.add(0, 1, 0));
			else
				tryPistonAura(cw);
			//if (bypass)tryPistonAura(w);//s
			//tryPistonAura(cn);
			//tryPistonAura(cs);
			break;
		case 2:
			if (airplace)
				tryPistonAura(ce.add(0, 1, 0));
			else
				tryPistonAura(ce);
			//if (bypass)tryPistonAura(e);//n
			//tryPistonAura(cs);
			//tryPistonAura(cn);
			break;
		case 3:
			if (airplace)
				tryPistonAura(cs.add(0, 1, 0));
			else
				tryPistonAura(cs);
			//if (bypass)tryPistonAura(s);//e
			//tryPistonAura(cw);
			//tryPistonAura(ce);
			break;
		case 4:
			if (airplace)
				tryPistonAura(cn.add(0, 1, 0));
			else
				tryPistonAura(cn);
			//if (bypass)tryPistonAura(n);//w
			//tryPistonAura(cw);
			//tryPistonAura(ce);
			break;
		}

		hasCharged = true;
		//clientMessageF("pistion!");
	}

	if (tickTimer >= cdelay && !DhasCharged) {
		tickTimer = 0;
		DhasCharged = true;
	}
	else if (tickTimer < cdelay && !DhasCharged) {
		tickTimer++;
		return;
	}
	//end
	if (!hasbb) {
		switch (Option) {
		case 1:
			if (airplace)
				gm->buildBlock(&vec3_ti(rw.add(0, 1, 0)), 0);
			else
				gm->buildBlock(&vec3_ti(rw), 0);
			break;
		case 2:
			if (airplace)
				gm->buildBlock(&vec3_ti(re.add(0, 1, 0)), 0);
			else
				gm->buildBlock(&vec3_ti(re), 0);
			break;
		case 3:
			if (airplace)
				gm->buildBlock(&vec3_ti(rs.add(0, 1, 0)), 0);
			else
				gm->buildBlock(&vec3_ti(rs), 0);
			break;
		case 4:
			if (airplace)
				gm->buildBlock(&vec3_ti(rn.add(0, 1, 0)), 0);
			else
				gm->buildBlock(&vec3_ti(rn), 0);
			break;
		}
		hasbb = true;
	}
	if (tickTimer >= pdelay && !Dhasbb) {
		tickTimer = 0;
		Dhasbb = true;
	}
	else if (tickTimer < pdelay && !Dhasbb) {
		tickTimer++;
		return;
	}
	if (!hasbreak) {
		if (!anti) {
			getAnti();
			anti = true;
			return;
		}
		g_Data.forEachEntity([](C_Entity* ent, bool b) {
			if (ent->getEntityTypeId() != 71)
				return;
			int range;
			if (moduleMgr->getModule<PistonAura>()->bypass)
				range = moduleMgr->getModule<PistonAura>()->crange;
			else
				range = moduleMgr->getModule<PistonAura>()->range;
			moduleMgr->getModule<PistonAura>()->DestroyC(ent, range);
			});
		hasbreak = true;
	}

	//clientMessageF("Detonated!");

	if (tickTimer >= breakdelay) {  // reset variables; prepare for another loop
		hasPlacedAnchor = false;
		hasCharged = false;
		hasbreak = false;
		hasDetonated = false;
		hasbb = false;
		hastrap = false;
		//
		DhasPlacedAnchor = false;
		DhasCharged = false;
		DhasDetonated = false;
		Dhasbb = false;
		Dtrap = false;
		tickTimer = 0;
		//
		takenAnchor = false;
		takenRS = false;
		takenGS = false;
		takentrap = false;
		anti = false;
		return;
	}
	else {
		tickTimer++;
		return;
	}

}



void PistonAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	//DrawUtils::setColor(.5411765f, .1058824f, 1.f, 1.f); // 138,27,255
	//DrawUtils::drawBox(currentBlock.toVector3(), currentBlock.toVector3().add(1), .4f, false);
	//
	if (w2) {
		if (airplace) {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(rw.add(0, 1, 0)).toVector3(), vec3_ti(rw.add(0, 1, 0)).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(cw.add(0, 1, 0)).toVector3(), vec3_ti(cw.add(0, 1, 0)).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(w1.add(0, 1, 0)).toVector3(), vec3_ti(rw.add(0, 1, 0)).toVector3().add(1), .4f, true);
		}
		else {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(rw).toVector3(), vec3_ti(rw).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(cw).toVector3(), vec3_ti(cw).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(w1).toVector3(), vec3_ti(w1).toVector3().add(1), .4f, true);
		}
	}
	else if (e2) {
		if (airplace) {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(re.add(0, 1, 0)).toVector3(), vec3_ti(re.add(0, 1, 0)).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(ce.add(0, 1, 0)).toVector3(), vec3_ti(ce.add(0, 1, 0)).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(e1.add(0, 1, 0)).toVector3(), vec3_ti(e1.add(0, 1, 0)).toVector3().add(1), .4f, true);
		}
		else {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(re).toVector3(), vec3_ti(re).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(ce).toVector3(), vec3_ti(ce).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(e1).toVector3(), vec3_ti(e1).toVector3().add(1), .4f, true);
		}
	}
	else if (s2) {
		if (airplace) {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(rs.add(0, 1, 0)).toVector3(), vec3_ti(rs.add(0, 1, 0)).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(cs.add(0, 1, 0)).toVector3(), vec3_ti(cs.add(0, 1, 0)).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(s1.add(0, 1, 0)).toVector3(), vec3_ti(s1.add(0, 1, 0)).toVector3().add(1), .4f, true);
		}
		else {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(rs).toVector3(), vec3_ti(rs).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(cs).toVector3(), vec3_ti(cs).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(s1).toVector3(), vec3_ti(s1).toVector3().add(1), .4f, true);
		}
	}
	else if (n2) {
		if (airplace) {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(rn.add(0, 1, 0)).toVector3(), vec3_ti(rn.add(0, 1, 0)).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(cn.add(0, 1, 0)).toVector3(), vec3_ti(cn.add(0, 1, 0)).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(n1.add(0, 1, 0)).toVector3(), vec3_ti(n1.add(0, 1, 0)).toVector3().add(1), .4f, true);
		}
		else {
			DrawUtils::setColor(.5411765f, .1058824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(rn).toVector3(), vec3_ti(rn).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.2477765f, .5123824f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(cn).toVector3(), vec3_ti(cn).toVector3().add(1), .4f, true);

			DrawUtils::setColor(.7466765f, .3052124f, 1.f, 1.f); // 138,27,255
			DrawUtils::drawBox(vec3_ti(n1).toVector3(), vec3_ti(n1).toVector3().add(1), .4f, true);
		}
	}
}

void PistonAura::onDisable() {
}