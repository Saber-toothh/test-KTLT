# Về BTL của chúng tôi

## Tên: Mô phỏng QoS và Phân tích hiệu năng mạng

## Mục đích: Tìm hiểu và ứng dụng thư viện STL C++

## Mô tả tổng quan bài toán
### 1. Input: Dòng chảy dữ liệu
- Mô phỏng 2 loại gói tin chạy vào Router:
    + Gói tin Thường (Normal - Màu Xanh):
        * Ví dụ: Tải file, Lướt web, Email.
        * Đặc điểm: Kích thước lớn (1500 bytes), số lượng rất nhiều, không gấp.
    + Gói tin VIP (High Priority - Màu Đỏ):
        * Đại diện: Gọi Zalo, Video Call, Game.
        * Đặc điểm: Kích thước nhỏ (~200 bytes), số lượng ít hơn, nhưng cần đi ngay.
- Tình huống mô phỏng: Gói Thường vào dồn dập (làm tắc nghẽn hàng đợi), sau đó thỉnh thoảng bắn Gói VIP vào.
### 2. Core: Thuật toán xử dụng `std::priority_queue`
- Bước 1: Vào hàng (Push)
    + Gói tin mới đến. Router kiểm tra `header.priority`.
    + STL tự động so sánh. Gói VIP sẽ được đẩy lên phía trên, vượt qua tất cả các Gói Thường đang chờ (nhưng vẫn đứng sau các Gói VIP khác nếu có).
- Bước 2: Chờ đợi (Queueing Delay)
    + Router đang bận xử lý gói tin hiện tại (đang đẩy ra dây). **Tất cả** phải chờ!
- Bước 3: Ra hàng (Pop)
    + Ngay khi Router rảnh tay (xong gói hiện tại).
    + Lệnh `routerQueue.top()` sẽ luôn luôn lấy gói VIP ra để xử lý tiếp theo.
### 3. Output: Chứng minh thuật toán hiệu quả
- A. Độ trễ trung bình (Average Latency)
    + Gói Thường (Xanh): Độ trễ TĂNG LÊN (hoặc giữ nguyên cao). Ví dụ: 500ms. 
    + Gói VIP (Đỏ): Độ trễ GIẢM MẠNH. Ví dụ: Chỉ còn 5ms.
> Lợi ích: Cuộc gọi video mượt mà, trong khi việc tải file kém quan trọng hơn chậm đi 1 chút.
- B. Tỷ lệ mất gói (Packet Loss Rate)
    + Cơ chế QoS nâng cao: Khi hàng đợi đầy (ví dụ Max = 20), nếu gói VIP đến, Router sẽ vứt bỏ gói Thường ở cuối hàng để lấy chỗ cho gói VIP.
    + Gói Thường: Bị mất nhiều.
    + Gói VIP: Gần như không bao giờ bị mất (trừ khi quá tải cực độ).
> Lợi ích: Hình ảnh video không bị vỡ, giọng nói không bị mất chữ.
- C. Jitter (Độ rung trễ/Biến động trễ)
    + Nếu không có QoS: Gói VIP lúc thì chờ 10ms, lúc thì chờ 1000ms -> Jitter cao -> Giật lag
    + Có QoS: Gói VIP luôn được đi ngay khi có thể. Độ trễ luôn ổn định ở mức thấp (ví dụ dao động 5-10ms) -> Jitter thấp -> Mượt
