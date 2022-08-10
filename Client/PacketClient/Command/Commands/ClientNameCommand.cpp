#include "ClientNameCommand.h"

#include "../../Module/ModuleManager.h"
#include "pch.h"

ClientNameCommand::ClientNameCommand() : IMCCommand("chatname", "更改聊天后缀", "<set/reset> <name>") {
	registerAlias("ct");
	registerAlias("chat");
}

bool ClientNameCommand::execute(std::vector<std::string>* args) {
	assertTrue(g_Data.getLocalPlayer() != nullptr);
	std::string option = args->at(1);
	std::transform(option.begin(), option.end(), option.begin(), ::tolower);
	auto interfaceMod = moduleMgr->getModule<Interface>();
	auto SBMOD = moduleMgr->getModule<ChatSuffix>();

	if (args->at(1) == "set" && args->size() > 2) {
		std::ostringstream os;
		for (int i = 2; i < args->size(); i++) {
			if (i > 1)
				os << " ";
			os << args->at(i);
		}
		std::string text = os.str().substr(1);
		std::string setStr = "更改为 " + args->at(2) + "!";
		//auto notification = g_Data.addNotification("Success", setStr); notification->duration = 10;
		//interfaceMod->getClientName() = text;
		SBMOD->getClientName() = text;
		return true;
	}
	else if (args->at(1) == "reset") {
		//interfaceMod->getClientName() = "Packet Client";
		SBMOD->getClientName() = "神里凌迟";
		//auto notification = g_Data.addNotification("Success:", "Reset the client's name!"); notification->duration = 10;
		return true;
	}
	return false;
}