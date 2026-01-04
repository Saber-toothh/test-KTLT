#include <iostream>
#include <vector>

// Gọi tất cả các Module vào
#include "Config.h"           // Module 1
#include "PacketGenerator.h"  // Module 2
#include "Router.h"           // Module 3
#include "Analytics.h"        // Module 4 

using namespace std;

int main() {
    cout << "=== MO PHONG QoS ROUTER (Batch Simulation) ===" << endl;

    // --- BƯỚC 1: LOAD CẤU HÌNH (Module 1) ---
    Config cfg = loadConfig("data/config.json");
    cout << "> Cau hinh: " << cfg.simulation_name 
         << " | Bandwidth: " << cfg.bandwidth_mbps << " Mbps" 
         << " | Time: " << cfg.total_time << "s" << endl;


    // --- BƯỚC 2: KHỞI TẠO ---
    Router coreRouter(cfg);       // Khởi tạo Router (Module 3)
    double currentTime = 0.0;     // Đồng hồ mô phỏng
    double timeStep = 0.001;      // Độ mịn thời gian: 1ms 
    int packetIDCounter = 0;      // Bộ đếm ID gói tin
    

    // --- BƯỚC 3: VÒNG LẶP CHÍNH (Simulation Loop) ---
    cout << "> Dang chay mo phong..." << endl;
    
    while (currentTime < cfg.total_time) {
        
        // A. Module 2: Sinh gói tin (Traffic Generation)
    
        // Quy ước cứ mỗi 10ms (0.01s) thì sinh 1 gói
        
        if ((int)(currentTime * 1000) % 10 == 0) { // Cứ 10ms sinh 1 lần
            packetIDCounter++;
            Packet p = generatePacket(packetIDCounter, currentTime, cfg);
            coreRouter.receivePacket(p);
        }

        // B. Module 3: Router xử lý
        coreRouter.process(currentTime);

        // C. Tăng thời gian
        currentTime += timeStep;
    }


    // --- BƯỚC 4: BÁO CÁO (Module 4) ---
    vector<Packet> history = coreRouter.getHistory();
    cout << "> Da xu ly xong " << history.size() << " goi tin." << endl;

    // Gọi hàm xuất báo cáo
    exportReport(history, "data/report.json");

    cout << "=== KET THUC ===" << endl;
    return 0;
}
