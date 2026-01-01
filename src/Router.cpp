#include "Router.h"
#include <iostream>

Router::Router(const Config& config) : cfg(config), busyUntil(0.0) {}

void Router::receivePacket(Packet p) {
    // 1. Kiểm tra tràn hàng đợi (Queue Overflow)
    if (waitingQueue.size() >= cfg.max_queue_size) {
        // Gói tin bị DROP tại đây. 
        // Trong thực tế có thể ghi log "Packet Dropped", nhưng ở đây ta chỉ cần không push vào queue.
        return; 
    }

    // 2. Đưa vào hàng đợi
    // Packet.h đã có operator< nên priority_queue tự biết đưa gói VOICE lên đầu
    waitingQueue.push(p);
}

void Router::process(double currentTime) {
    // Nếu Router đang bận (thời gian hiện tại chưa vượt qua thời điểm rảnh dự kiến) -> KHÔNG LÀM GÌ CẢ
    if (currentTime < busyUntil) {
        return; 
    }

    // Nếu Router đang rảnh và có hàng chờ
    if (!waitingQueue.empty()) {
        // 1. Lấy gói ưu tiên nhất ra
        Packet p = waitingQueue.top();
        waitingQueue.pop();

        // 2. Gán thời điểm bắt đầu xử lý (chính là lúc Router vừa rảnh)
        p.startProcessTime = currentTime;

        // 3. Tính toán thời gian truyền (Serialization Delay)
        // Công thức: Thời gian (s) = Size (bits) / Bandwidth (bps)
        // Lưu ý đổi đơn vị: Size đang là Bytes (*8 -> bits), Bandwidth đang là Mbps (*10^6 -> bps)
        double transmissionTime = (p.size * 8.0) / (cfg.bandwidth_mbps * 1e6);

        // 4. Cập nhật thời điểm Router sẽ rảnh tiếp theo (Busy Until)
        busyUntil = currentTime + transmissionTime;

        // 5. Gán thời gian hoàn thành (Finish Time) cho gói tin
        p.finishTime = busyUntil;

        // 6. Lưu vào lịch sử (Đã xong nhiệm vụ)
        processedHistory.push_back(p);
    }
}

std::vector<Packet> Router::getHistory() const {
    return processedHistory;
}