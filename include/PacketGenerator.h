#ifndef PACKET_GENERATOR_H
#define PACKET_GENERATOR_H

#include "Packet.h"
#include "Config.h"

// Hàm sinh MỘT gói tin dựa trên cấu hình lưu lượng
Packet generatePacket(int id, double currentTime, const Config& cfg);

#endif			              		 				
