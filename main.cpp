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

    // --- BƯỚC 1: LOAD CẤU HÌNH ---
    Config cfg = loadConfig("data/config.json");
    cout << "> Cau hinh: " << cfg.simulation_name 
         << " | Bandwidth: " << cfg.bandwidth_mbps << " Mbps" 
         << " | Time: " << cfg.total_time << "s" << endl;


    // --- BƯỚC 2: KHỞI TẠO ---
    Router coreRouter(cfg);       // Khởi tạo Router 
    double currentTime = 0.0;     // Đồng hồ mô phỏng
    double timeStep = 0.001;      // Độ mịn thời gian: 1ms 
    int packetIDCounter = 0;      // Bộ đếm ID gói tin
    

    // --- BƯỚC 3: VÒNG LẶP CHÍNH ---
    cout << "> Dang chay mo phong..." << endl;
    
    while (currentTime < cfg.total_time) {
        
        // A. Sinh gói tin 
    
         // Cứ mỗi 150ms (0.15s) thì sinh 1 gói
        
        if ((int)(currentTime * 1000) % 150 == 0) { // Cứ 150ms sinh 1 lần
            packetIDCounter++;
            Packet p = generatePacket(packetIDCounter, currentTime, cfg);
            coreRouter.receivePacket(p);
        }

        // B. Router xử lý
        coreRouter.process(currentTime);

        // C. Tăng thời gian
        currentTime += timeStep;
    }


    // --- BƯỚC 4: BÁO CÁO ---
    vector<Packet> history = coreRouter.getHistory();
    cout << "> Da xu ly xong " << history.size() << " goi tin." << endl;

    // Xuất báo cáo
    exportReport(history, "data/report.json");

    cout << "=== KET THUC ===" << endl;
    return 0;
}
