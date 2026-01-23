#ifndef PACKET_H
#define PACKET_H

#include "json.hpp" // Chuyển đổi Packet <-> JSON

using json = nlohmann::json;

enum PacketType { DATA , VIDEO, VOICE };

// Định nghĩa cấu trúc dữ liệu = struct
struct Packet {
    // A. DỮ LIỆU
    int id;                  // Mã định danh
    PacketType type;         // Loại (để xét ưu tiên)
    double size;             // Kích thước (để tính thời gian truyền)
    double arrivalTime;      // Lúc đến Router
    double startProcessTime; // Lúc bắt đầu được xử lý
    double finishTime;       // Lúc xử lý xong
    int priorityScore;

    // B. HELPER METHOD: Tính toán độ trễ
    double getLatency() const { return finishTime - arrivalTime; }
    
    // C. ĐỊNH NGHĨA TOÁN TỬ SO SÁNH 
    bool operator<(const Packet& other) const { //std::less default in p_queue
        // 1. Nếu cùng điểm ưu tiên
        if (priorityScore == other.priorityScore) {
            return arrivalTime > other.arrivalTime; 
        } else {// 2. Nếu khác loại: càng lớn càng ưu tiên
        return priorityScore < other.priorityScore; }
    }
};

//  D. MACRO JSON 
// Giúp thư viện tự động tạo code để chuyển đổi giữa Struct và JSON
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Packet, id, type, size, arrivalTime, startProcessTime, finishTime, priorityScore)


#endif


