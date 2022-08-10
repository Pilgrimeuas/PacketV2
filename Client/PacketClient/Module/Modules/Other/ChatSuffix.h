#pragma once
#include "../Module.h"
class ChatSuffix : public IModule {
public:
	ChatSuffix();
	~ChatSuffix();
	std::string clientName = "神里凌迟";
	std::string& getClientName() { return clientName; };
	virtual const char* getModuleName() override;
	virtual void onSendPacket(C_Packet* packet) override;
};