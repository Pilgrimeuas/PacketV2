#include "CPSAura.h"

CPSAura::CPSAura() : IModule('P', Category::COMBAT, "Attacks entities around you automatically") {
	registerBoolSetting("MultiAura", &isMulti, isMulti);
	registerBoolSetting("MobAura", &isMobAura, isMobAura);
	registerFloatSetting("range", &range, range, 2.f, 20.f);
	registerIntSetting("delay", &delay, delay, 0, 20);
	registerBoolSetting("hurttime", &hurttime, hurttime);
	registerBoolSetting("AutoWeapon", &autoweapon, autoweapon);
	registerBoolSetting("Rotations", &rotations, rotations);
	registerBoolSetting("Silent Rotations", &silent, silent);
}

CPSAura::~CPSAura() {
}

const char* CPSAura::getModuleName() {
	return ("CPSAura");
}

static std::vector<C_Entity*> targetList;
void FindEntity(C_Entity* currentEntity, bool isRegularEntity) {
	static auto CPSAuraMod = moduleMgr->getModule<CPSAura>();
	std::string TargetUtilName = currentEntity->getNameTag()->getText();

	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	//if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		//return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 66) // falling block
		return;

	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (CPSAuraMod->isMobAura) {
		if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
			return;
		if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) // Don't hit this pesky antibot on 2b2e.org
			return;
		if (currentEntity->getEntityTypeId() == 64) // item
			return;
		if (currentEntity->getEntityTypeId() == 80)  // Arrows
			return;
		if (currentEntity->getEntityTypeId() == 51) // NPC
			return;
	}
	else {
		if (!TargetUtil::isValidTarget(currentEntity))
			return;
	}

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < CPSAuraMod->range) {
		targetList.push_back(currentEntity);
	}
}

void CPSAura::findWeapon() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage) {
				damage = currentDamage;
				slot = n;
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}

void CPSAura::onTick(C_GameMode* gm) {
	targetListEmpty = targetList.empty();
	//Loop through all our players and retrieve their information
	targetList.clear();

	g_Data.forEachEntity(FindEntity);

	Odelay++;
	if (!targetList.empty() && Odelay >= delay) {

		if (autoweapon) findWeapon();

		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
			C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);  // make sure to update rotation if player is standing still
		}

		// Attack all entitys in targetList
		if (isMulti) {
			for (auto& i : targetList) {
				if (!(i->damageTime > 1 && hurttime)) {
					g_Data.getLocalPlayer()->swing();
					for (int n = 0; n < 15; n++) {
						gm->attack(i);
					}
				}
			}
		}
		else {
			if (!(targetList[0]->damageTime > 1 && hurttime)) {
				g_Data.getLocalPlayer()->swing();
				for (int s = 0; s < 15; s++) {
					gm->attack(targetList[0]);
				}
			}
		}
		if (rotations) {
			angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
		}
		Odelay = 0;
	}
}

void CPSAura::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr)
		setEnabled(false);
}

void CPSAura::onSendPacket(C_Packet* packet) {
	if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "ntest.easecation.net") == 0) {
		if (packet->isInstanceOf<LevelSoundEventPacket>() && !targetList.empty()) {
			LevelSoundEventPacket* soundEventPacket = reinterpret_cast<LevelSoundEventPacket*>(packet);
			if (soundEventPacket->sound == 43) //sound 42�ǿջ���ʱ����ֵ Ҳ�ᱻ�����CPS ���ǹ�����ʱ�򲻷��Ǹ���
				soundEventPacket->sound = 0;
		} //�ƹ�EaseCation������CPS���
	}
	if (strcmp(g_Data.getRakNetInstance()->serverIp.getText(), "ntest.easecation.net") == 0) {
		if (packet->isInstanceOf<LevelSoundEventPacket>() && !targetList.empty()) {
			LevelSoundEventPacket* soundEventPacket = reinterpret_cast<LevelSoundEventPacket*>(packet);
			if (soundEventPacket->sound == 42) //sound 42�ǿջ���ʱ����ֵ Ҳ�ᱻ�����CPS ���ǹ�����ʱ�򲻷��Ǹ���
				soundEventPacket->sound = 0;
		} //�ƹ�EaseCation������CPS���
	}
	if (packet->isInstanceOf<C_MovePlayerPacket>() && g_Data.getLocalPlayer() != nullptr && silent) {
		if (!targetList.empty()) {
			auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
			vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList[0]->getPos());
			movePacket->pitch = angle.x;
			movePacket->headYaw = angle.y;
			movePacket->yaw = angle.y;
		}
	}
}