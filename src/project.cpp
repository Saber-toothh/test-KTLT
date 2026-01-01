#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;
enum PacketType { DATA , VIDEO , VOICE };

// --- PHẦN 1: CẤU TRÚC DỮ LIỆU ---

struct Config {
    int max_queue_size;    
    double bandwidth_bps;
    double totalTime;  
};

struct Packet {
    int id;
    PacketType type;          // 0: Thường, 1: VIP ,2: SIU VIP
    int size_bytes;        
    double arrivalTime;
    double finishedTime;   

    // Khởi tạo dấu so sánh
    bool operator<(const Packet& other) const {
        return type < other.type;
    }
};

// --- PHẦN 2: LOGIC ROUTER ---

class Router {
private:
    priority_queue<Packet> buffer;
    Config config;
    double busy_until;
    vector<Packet> processedpacket;

public:
    Router(Config cfg) {
        config = cfg;
        busy_until = 0.0;
    }

    // Nhận gói tin vào
    bool push(Packet p, double current_time) {
        if (buffer.size() >= config.max_queue_size) {
            cout << "Time " << current_time << ": [DROP] Router day! Huy goi " << p.id << endl;
            return false;
        }

        p.arrivalTime = current_time;
        buffer.push(p);
        
        cout << "Time " << current_time << ": [PUSH] Nhan goi " << p.id 
             << " (Prio=" << p.type << ")" << endl;
        return true;
    }

    // Xử lý và gửi đi
    void process(double current_time) {
        // Nếu Router đang bận hoặc không có hàng thì thôi
        if (current_time < busy_until || buffer.empty() || busy_until > config.totalTime) {
            return;
        }

        Packet p = buffer.top();
        buffer.pop();
        processedpacket.push_back(p);

        // Tính toán độ trễ (Giây) = Kích thước (Bits) / Tốc độ (Bps)
        double delay = (p.size_bytes * 8.0) / config.bandwidth_bps;
        
        // Cập nhật thời gian bận
        busy_until = current_time + delay;
        p.finishedTime = busy_until;

        cout << "Time " << current_time << ": [SENT] Xu ly xong goi " << p.id 
             << " (Mat " << delay << "s)" << endl;
    }

    // Hàm phụ trợ cho Main
    bool isBusy(double current_time) {
        return current_time < busy_until;
    }

    int getCount() {
        return buffer.size();
    }
};

// --- PHẦN 3: CHƯƠNG TRÌNH CHẠY ---

int main() {
    // Cấu hình: Max 5 gói, Tốc độ 1000 bit/s
    Config cfg = { 5, 1000.0 ,100.0};
    Router myRouter(cfg);

    cout << "=== MO PHONG ROUTER (SIMPLE) ===" << endl;

    double time = 0.0;

    // Kịch bản: 2 Gói thường đến trước
    myRouter.push({101, DATA, 200}, time);
    myRouter.push({102, DATA, 500}, time);

    // Xử lý gói đầu tiên
    myRouter.process(time);

    // Thời gian trôi đi 0.5s, gói VIP đến
    time = 0.5;
    cout << "\n--- VIP xuat hien ---" << endl;
    myRouter.push({999, VOICE, 100}, time); // VIP (Prio = 1)

    // Vòng lặp giả lập thời gian trôi để xử lý nốt các gói còn lại
    cout << "\n--- Bat dau xu ly tu dong ---" << endl;
    while (myRouter.getCount() > 0 || myRouter.isBusy(time)) {
        time += 0.5;
        myRouter.process(time);
    }

    return 0;
}
