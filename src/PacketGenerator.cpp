#include "PacketGenerator.h"
#include "Config.h"
#include <iostream>
#include <random>
#include <map>

// Bộ sinh số ngẫu nhiên tạo nguồn cho các hàm - std::mt19937 rng (...)
static std::mt19937 rng (std::random_device{}());

// Hàm sinh số nguyên ngẫu nhiên trong [min, max] --> Kích cỡ gói tin
int randomInt (int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);  // sinh 1 số ngẫu nhiên trong [min, max].
}

// Hàm sinh gói tin dựa trên cấu hình lưu lượng (tỉ lệ):
Packet generatePacket (int id, double currentTime, const Config& cfg) {
    Packet p;
    p.id = id;
    p.arrivalTime = currentTime;
    p.startProcessTime = 0.0;
    p.finishTime = 0.0;
    
	// Hàm sinh số ngẫu nhiên theo tỉ lệ:
    std::discrete_distribution<int> typeDist({
        cfg.traffic_rate.data,   // Index 0 -> DATA
        cfg.traffic_rate.video,  // Index 1 -> VIDEO
        cfg.traffic_rate.voice   // Index 2 -> VOICE
    });

    // Sinh loại
    int typeIndex = typeDist(rng);
    p.type = static_cast<PacketType>(typeIndex);

    //Gán điểm ưu tiên
    try {
        p.priorityScore = cfg.type_to_score.at((int)p.type);
    } catch (...) {
        p.priorityScore = 0; // Fallback nếu quên config
    }

    //Sinh size gói
    /*switch (p.type) {
        case VOICE: 
            p.size = randomInt(64, 128);   // Gói thoại nhỏ
            break;
        case VIDEO: 
            p.size = randomInt(700, 1200); // Gói video lớn
            break;
        case DATA: 
            p.size = randomInt(500, 1000);  // Gói web trung bình
            break;
        default:
            p.size = 500;
            break;
    }*/
   p.size = 500;

    return p;
}



