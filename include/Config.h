// --- File: Config.h ---
#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <iostream>
#include <fstream>
#include "json.hpp" // Đảm bảo đường dẫn đúng

using json = nlohmann::json;

// 1. Tạo struct con cho phần traffic_rate (vì nó lồng bên trong)
struct TrafficRate {
    double data;
    double video;
    double voice;
};

// 2. Tạo struct Config chính
struct Config {
    std::string simulation_name;
    double total_time;
    int bandwidth_mbps;
    int max_queue_size;
    TrafficRate traffic_rate; // Lồng struct con vào đây
};

// 3. KHAI BÁO MACRO (Phép thuật nằm ở đây)
// Lưu ý: Phải khai báo TrafficRate trước thì Config mới hiểu
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TrafficRate, data, video, voice)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, simulation_name, total_time, bandwidth_mbps, max_queue_size, traffic_rate)

// 4. Khai báo hàm load (chỉ tên hàm)
Config loadConfig(const std::string& filePath);

#endif
