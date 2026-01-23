# Về BTL của chúng tôi

## I. Tên: Mô phỏng QoS và Phân tích Hiệu năng mạng

## II. Mục đích: Tìm hiểu và ứng dụng thư viện STL C++

## III. Mô tả tổng quan bài toán

### 1. Input: Dòng chảy dữ liệu
- Mô phỏng 3 loại gói tin chạy vào Router:
    + Gói tin Thoại (Voice)
    + Gói tin Video
    + Gói tin dữ liệu thường (Data)

- Tình huống mô phỏng:
    Các gói sinh theo tỉ lệ mô phỏng, gây tắc nghẽn hàng đợi --> gói tin được gán priority HIGH luôn có độ trễ thấp, được đi trước **trong hàng đợi**

### 2. Core: Cơ chế Router
- 1: Vào hàng (Push)
    + Router nhận gói tin, kiểm tra độ ưu tiên.
    + Nếu hàng đợi trống, các gói tin sắp xếp theo độ ưu tiên, cao đứng trước thấp đứng sau.
    + Nếu hàng đợi đầy, các gói tin ưu tiên thấp bị drop, hoặc phải xếp sau các gói tin ưu tiên cao.
- 2: Xử lý
    + Router đang bận xử lý gói tin hiện tại. **Tất cả** phải chờ!
    + Xử lý xong, Router lấy gói tin có độ ưu tiên cao nhất trong hàng chở để xử  lý tiếp.
- 3: Hệ quả
    + Gói ưu tiên cao có độ trễ thấp, gói ưu tiên thấp phải chờ rất lâu hoặc bị drop.

### 3. Output: Báo cáo định lượng
- Kết quả được xuất ra file báo cáo (report.json) gồm:
    + Độ trễ trung bình của tất cả gói tin
    + Độ trễ trung bình từng loại
    + Gói tin có độ trễ lớn nhất
    + Trạng thái mạng

## IV. Các thư viện sử dụng

### 1. STL C++
- Container:
    + priority_queue: Yếu tố cốt lõi của Router. Tự động sắp xếp gói tin dựa trên độ ưu tiên và thời gian đến Router.
    + map: Ánh xạ cấu hình vào logic xử lý --> Thiêt kế linh hoạt (mới nâng cấp).
    + vector: Quản lý lịch sử gói tin.
- Algorithm: accumulate, count_if, max_element.

### 2. JSON for Modern C++
- Ứng dụng nlohmann/json để tách biệt Cấu hình (bao gồm nhãn ưu tiên) và Mã nguồn
--> Giúp tăng độ linh hoạt. Có thể test các kịch bản khác nhau.