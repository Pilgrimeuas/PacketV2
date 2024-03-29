#include <string>

#include "../../../../Utils/Utils.h"
#include "Verify.h"
//Added this her ebec no point in fill rebuild each time we add a name
std::string allowedNames[] = { "EveryoneWhySoEZ", "pilgrimeaus", "Thatrogic", "hellorogic", "DimplesIII", "GhOsT37664", "ItsCascade20997", "GuiTi2022", "", "Enzo6172", "FitFrancE2021", "NPCblocko", "USSR USSR 177", "USSRUSSR177", "USSR_USSR_177","yurubb", "xuan715","JayGatsby2", "Zmzz4861","kuaisxiuhe", "ddejjx","BaisMo", "m1n1 w0rld" ,"Wikiliema" };
Verification::Verification() : IModule(0, Category::OTHER, "Checks if you are pro and based") {
}
Verification::~Verification() {
}

const char* Verification::getModuleName() {
	return ("Verification");
}

void Verification::onTick(C_GameMode* gm) {

	if (g_Data.getLocalPlayer() == nullptr) return;

	if (test == false) {
		clientMessageF("VerificationID中.....");


		std::string SBName = g_Data.getLocalPlayer()->getNameTag()->getText();

		for (auto checkOne : allowedNames) {
			std::size_t strHas = SBName.find(checkOne);

			if (SBName == "" /*|| SBName == "sblolhhh"*/) {
				return;  // user is offline; let them test in a flatworld XD
			}
			else if (strHas != std::string::npos) {
				passedTest = true;
				test = true;
				break;
			}
			else
				passedTest = false;
			test = true;
		}

		if (passedTest) {
			clientMessageF("欢迎使用PacketClient");
			g_Data.getLocalPlayer()->pointingStruct->playSound("raid.horn", *g_Data.getLocalPlayer()->getPos(), 1.f, 1.f);
		}
		else if (passedTest == false) {
			clientMessageF("白名单未通过，取消注入!");
			g_Data.getLocalPlayer()->pointingStruct->playSound("block.bell.hit", *g_Data.getLocalPlayer()->getPos(), 1.f, 1.f);
			GameData::terminate();
		}
	}
}

void Verification::onDisable() {
	setEnabled(true);
}
