#ifndef ROUTER_H
#define ROUTER_H

#include <queue>
#include <vector>
#include "Packet.h"
#include "Config.h"

class Router {
private:
    std::priority_queue<Packet> waitingQueue;
    
    // Lưu lịch sử các gói đã xử lý xong để Phân tích
    std::vector<Packet> processedHistory;

    Config cfg; 
    
    // Biến trạng thái: Router đang bận xử lý gói hiện tại đến ?
    double busyUntil; 

public:
    // Constructor nhận Config
    Router(const Config& config);

    // Nhận gói tin từ PacketGenerator
    void receivePacket(Packet p);

    // Xử lý gói tin theo thời gian thực
    void process(double currentTime);

    // Lấy dữ liệu lịch sử
    std::vector<Packet> getHistory() const;
};

#endif
