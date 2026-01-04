#include "PacketGenerator.h"
#include "Config.h"
#include <iostream>
#include <random>
#include <map>

// Bộ sinh số ngẫu nhiên tạo nguồn cho các hàm - std::mt19937 rng (...)
static std::mt19937 rng (std::random_device{}());
	/*
	static : cho phép sử dụng hàm trong cả file.
	mt19937 : thuật toán Mersenne Twister (máy quay số).
	random_device{}() : bộ khởi tạo giá trị ban đầu cho bộ sinh số.
	*/

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

    int typeIndex = typeDist(rng);
    p.type = static_cast<PacketType>(typeIndex);

    switch (p.type) {
        case VOICE: 
            p.size = randomInt(64, 200);   // Gói thoại nhỏ
            break;
        case VIDEO: 
            p.size = randomInt(700, 1500); // Gói video lớn
            break;
        case DATA: 
            p.size = randomInt(500, 1000);  // Gói web trung bình
            break;
        default:
            p.size = 500;
            break;
    }

    return p;
}



