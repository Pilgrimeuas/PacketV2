#include "PacketLogger.h"
#include "../pch.h"

PacketLogger::PacketLogger() : IModule(0, Category::OTHER, "Packet Logging") {
}

const char* PacketLogger::getModuleName() {
	return ("发包记录");
}

void PacketLogger::onSendPacket(C_Packet* packet) {
	if (packet->isInstanceOf<C_PlayerActionPacket>()) {
		auto pk = reinterpret_cast<C_PlayerActionPacket*>(packet);
		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[PACKETLOG] %s action=%i", packet->getName()->getText(), pk->action);
		return;
	}
	g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[PACKETLOG] %s", packet->getName()->getText());
}
