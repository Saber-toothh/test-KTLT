#include "Analytics.h"
#include "Packet.h"
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric> 
#include <algorithm>
#include <iomanip>

using namespace std;
using json = nlohmann::json;

void exportReport(const vector<Packet>& history, const string& filename) {
    // Kiểm tra dữ liệu rỗng
    if (history.empty()) {
        cerr << "[Canh bao Module 4] Khong co du lieu de bao cao!" << endl;
        return;
    }

    cout << "--- MODULE 4: Dang phan tich " << history.size() << " goi tin... ---" << endl;

    // --- A. TÍNH TOÁN BẰNG STL ALGORITHMS ---

    // 1. Đếm số lượng (count_if)
    int voiceCount = count_if(history.begin(), history.end(), [](const Packet& p){
        return p.type == VOICE;
    });
    int videoCount = count_if(history.begin(), history.end(), [](const Packet& p){
        return p.type == VIDEO;
    });
    int dataCount = history.size() - voiceCount - videoCount;

    // 2. Tính trung bình độ trễ (accumulate)
    double totalLatency, VoiceLatency, VideoLatency, DataLatency;
    double avgLatency, avgVoiceLatency, avgVideoLatency, avgDataLatency;
    
    totalLatency = accumulate(history.begin(), history.end(), 0.0, 
        [](double sum, const Packet& p) {
            return sum + p.getLatency();
        });
    VoiceLatency = accumulate(history.begin(), history.end(), 0.0,
        [](double sum, const Packet& p) {
            if (p.type == VOICE) {
                return sum + p.getLatency();
            } else { return sum; }
        });

    VideoLatency = accumulate(history.begin(), history.end(), 0.0,
        [](double sum, const Packet& p) {
            if (p.type == VIDEO) {
                return sum + p.getLatency();
            } else { return sum; }
        });
    
    DataLatency = accumulate(history.begin(), history.end(), 0.0,
        [](double sum, const Packet& p) {
            if (p.type == DATA) {
                return sum + p.getLatency();
            } else { return sum; }
        });
    
    avgLatency = 0.0;
    if (!history.empty()) {
        avgLatency = totalLatency / history.size();
    }

    avgVoiceLatency = VoiceLatency / voiceCount;
    avgVideoLatency = VideoLatency / videoCount;
    avgDataLatency = DataLatency / dataCount;

    // 3. Tìm gói tin tệ nhất (max_element)
    auto maxIt = max_element(history.begin(), history.end(), 
        [](const Packet& a, const Packet& b) {
            return a.getLatency() < b.getLatency();
        });

    // --- B. TẠO JSON REPORT ---
    json jReport;
    
    // Phần tổng quan
    jReport["summary"] = {
        {"total_packets", history.size()},
        {"breakdown", {
            {"voice", voiceCount},
            {"video", videoCount},
            {"data", dataCount}
        }},
        {"average_latency", avgLatency},
        {"average_voice_latency", avgVoiceLatency},
        {"average_video_latency", avgVideoLatency},
        {"average_data_latency", avgDataLatency},
        {"status", (avgLatency < 1.0) ? "GOOD" : "CONGESTED"} // Ví dụ đánh giá
    };

    // Phần chi tiết gói tin lag nhất
    if (maxIt != history.end()) {
        jReport["worst_packet"] = *maxIt; 
    }

    // Phần danh sách toàn bộ (để vẽ biểu đồ)
    jReport["details"] = history; 

    // --- C. XUẤT RA FILE ---
    ofstream file(filename);
    if (file.is_open()) {
        // setw(4) để format thụt đầu dòng đẹp (Pretty print)
        file << setw(4) << jReport << endl;
        cout << "[Thanh cong] Da xuat bao cao ra file: " << filename << endl;
    } else {
        cerr << "[Loi] Khong the ghi file " << filename << endl;
    }
}