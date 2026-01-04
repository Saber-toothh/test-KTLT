#include "Router.h"
#include <iostream>

Router::Router(const Config& config) : cfg(config), busyUntil(0.0) {}

void Router::receivePacket(Packet p) {
    // 1. Kiểm tra tràn hàng đợi 
    if (waitingQueue.size() >= cfg.max_queue_size) {
        // Gói tin bị DROP tại đây. -- > Không Push
        return; 
    }

    // 2. Đưa vào hàng đợi
    waitingQueue.push(p);
}

void Router::process(double currentTime) {
    // Nếu Router đang bận -> KHÔNG LÀM GÌ
    if (currentTime < busyUntil) {
        return; 
    }

    // Nếu Router đang rảnh và hàng chờ không đầy
    if (!waitingQueue.empty()) {
        // 1. Lấy gói ưu tiên nhất ra xử lý
        Packet p = waitingQueue.top();
        waitingQueue.pop();

        // 2. Gán thời điểm bắt đầu xử lý 
        p.startProcessTime = currentTime;

        // 3. Tính thời gian truyền (Serialization Delay)
        // Công thức: Thời gian (s) = Size (bits) / Bandwidth (bps)
        double transmissionTime = (p.size * 8.0) / (cfg.bandwidth_mbps * 1e6);

        // 4. Cập nhật thời điểm Router sẽ rảnh tiếp theo
        busyUntil = currentTime + transmissionTime;

        // 5. Gán thời gian hoàn thành cho gói tin
        p.finishTime = busyUntil;

        // 6. Lưu vào lịch sử 
        processedHistory.push_back(p);
    }
}

std::vector<Packet> Router::getHistory() const {
    return processedHistory;
}
