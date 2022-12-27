#include "ChatSuffix.h"
ChatSuffix::ChatSuffix() : IModule(0, Category::OTHER, "Adds the client suffix") {
}

ChatSuffix::~ChatSuffix() {
}


const char* ChatSuffix::getModuleName() {
	return "ChatSuffix";
}
void ChatSuffix::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_TextPacket>()) {
		C_TextPacket* funy = reinterpret_cast<C_TextPacket*>(packet);
		std::string Sentence;
		std::string end;
		std::string fuck;
		fuck = " | ";
		end = "> ";
		Sentence = end + funy->message.getText() + fuck + clientName;
		funy->message.resetWithoutDelete();
		funy->message = Sentence;
	}
}