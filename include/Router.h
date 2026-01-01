#ifndef ROUTER_H
#define ROUTER_H

#include <queue>
#include <vector>
#include "Packet.h"
#include "Config.h"

class Router {
private:
    // Hàng đợi ưu tiên (Tự động sắp xếp VOICE lên đầu, DATA xuống cuối nhờ operator< trong Packet)
    std::priority_queue<Packet> waitingQueue;
    
    // Lưu lịch sử các gói đã xử lý xong để Module 4 báo cáo
    std::vector<Packet> processedHistory;

    Config cfg; 
    
    // Biến trạng thái: Router đang bận đến thời điểm nào?
    double busyUntil; 

public:
    // Constructor nhận Config
    Router(const Config& config);

    // 1. Nhận gói tin từ Generator (Ingress)
    void receivePacket(Packet p);

    // 2. Xử lý gói tin theo thời gian thực (Core Logic)
    void process(double currentTime);

    // 3. Lấy dữ liệu lịch sử
    std::vector<Packet> getHistory() const;
};

#endif