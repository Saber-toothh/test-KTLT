#ifndef PACKET_H
#define PACKET_H

#include "json.hpp" // Thư viện để chuyển đổi Packet <-> JSON

using json = nlohmann::json;

enum PacketType { DATA , VIDEO, VOICE };

// Struct: Định nghĩa cấu trúc dữ liệu
struct Packet {
    // A. DỮ LIỆU (DATA)
    int id;                  // Mã định danh
    PacketType type;         // Loại (để xét ưu tiên)
    double size;             // Kích thước (để tính thời gian truyền)
    double arrivalTime;      // Lúc đến Router
    double startProcessTime; // Lúc bắt đầu được xử lý
    double finishTime;       // Lúc xử lý xong

    // B. HÀM TIỆN ÍCH (HELPER METHOD)
    // Tính toán nhanh độ trễ
    double getLatency() const { return finishTime - arrivalTime; }
    
    // C. ĐỊNH NGHĨA TOÁN TỬ SO SÁNH 
    bool operator<(const Packet& other) const {
        // 1. Nếu cùng loại:
        if (type == other.type) {
            return arrivalTime > other.arrivalTime; 
        } else {// 2. Nếu khác loại:
        // Gói có type nhỏ (0) sẽ thua gói có type lớn (1)
        return type < other.type; }
    }
};

// --- D. MACRO JSON ---
// Giúp thư viện tự động tạo code để biến Struct thành JSON và ngược lại
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Packet, id, type, size, arrivalTime, startProcessTime, finishTime)


#endif
