#include "Config.h"
#include <iostream>
#include <fstream>
#include <string>

Config loadConfig(const std::string& filePath) {
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        std::cerr << "[Loi Module 1] Khong tim thay file config: " << filePath << std::endl;
        return Config(); 
    }

    json j;
    file >> j; // Đọc toàn bộ file vào biến json

    Config cfg = j.get<Config>(); 

    std::cout << "[Module 1] Da load config: " << cfg.simulation_name << std::endl;
    
    return cfg;
}
