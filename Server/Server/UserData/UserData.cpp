#include "../UserData/UserData.h"

#include "../IOCPServer/SocketInfo.h"

unique_ptr<ItemManager> UserData::itemManager = make_unique<ItemManager>();
