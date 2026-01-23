#include "Config.h"
#include "Packet.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Chuyển String PRIORITY_LEVEL trong config.json -> int Score
int parsePriority (const std::string& level) {
    if (level == "HIGH") return 10;
    if (level == "MEDIUM") return 5;
    if (level == "LOW") return 1;

    cerr << "[Canh bao] Muc uu tien khong hop le: " << level << " -> Mac dinh: 0" << std::endl;
    return 0;
}

// Chuyển String TYPE trong config.json -> enum PacketType
int getTypeEnum(const std::string& typeName) {
    if (typeName == "voice") return VOICE;
    if (typeName == "video") return VIDEO;
    return DATA; // default
}

//Load config 
Config loadConfig(const std::string& filePath) {
    ifstream file(filePath);
    
    if (!file.is_open()) {
        cerr << "[Loi] Khong tim thay file config: " << filePath << std::endl;
        return Config(); 
    }

    json j;
    file >> j; // Đọc toàn bộ file vào biến json

    Config cfg = j.get<Config>(); // nhập tự động dùng MACRO JSON

    // duyệt qua config thô -> báo độ ưu tiên
    cout << "--- Cau hinh do uu tien ---" << std::endl;
    for (auto const& pair : cfg.priority_level) {
        string typeName = pair.first;
        string levelName = pair.second;

        int typeEnum = getTypeEnum(typeName);
        int score = parsePriority(levelName);
        
        cfg.type_to_score[typeEnum] = score;
        
        cout << "Loai: " << typeName << " (" << levelName << ") -> Diem uu tien: " << score << std::endl;
    }
    cout << "----------" << endl;

    cout << "[OK] Da load config: " << cfg.simulation_name << std::endl;
    
    return cfg;
}

