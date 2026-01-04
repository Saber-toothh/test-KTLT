#ifndef CONFIG_H
#define CONFIG_H

#include <string>
#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

struct TrafficRate {
    double data;
    double video;
    double voice;
};

// Cấu trúc Config chính
struct Config {
    std::string simulation_name;
    double total_time;
    double bandwidth_mbps;
    unsigned int max_queue_size;
    TrafficRate traffic_rate; // Lồng struct con vào đây
};

// KHAI BÁO MACRO
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TrafficRate, data, video, voice)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Config, simulation_name, total_time, bandwidth_mbps, max_queue_size, traffic_rate)

// Khai báo hàm loadConfig
Config loadConfig(const std::string& filePath);

#endif
