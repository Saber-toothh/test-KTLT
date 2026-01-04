# --- CẤU HÌNH ---

# Trình biên dịch
CXX = g++

# Cờ biên dịch (Compiler Flags)
# -std=c++17 : Dùng chuẩn C++17 (tốt cho nlohmann/json)
# -I./include: Chỉ cho trình biên dịch biết chỗ tìm file header (.h)
# -Wall -Wextra: Bật chế độ cảnh báo chi tiết (như bạn yêu cầu)
CXXFLAGS = -std=c++17 -Wall -Wextra -I./include

# Tên file chạy đầu ra (Executable)
TARGET = run_sim

# Danh sách các file nguồn (.cpp)
# Lưu ý: main.cpp nằm ở ngoài, các module nằm trong src/
SRCS = main.cpp \
       src/LoadConfig.cpp \
       src/PacketGenerator.cpp \
       src/Router.cpp \
       src/Analytics.cpp

# Tự động tạo danh sách file object (.o) tương ứng
OBJS = $(SRCS:.cpp=.o)

# --- CÁC QUY TẮC (RULES) ---

# 1. Quy tắc mặc định (khi gõ 'make') -> Build ra file chạy
all: $(TARGET)

# 2. Quy tắc liên kết (Linking): Nối các file .o thành file chạy
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "--------------------------------------"
	@echo "Build THANH CONG! Chay ./$(TARGET) de bat dau."
	@echo "--------------------------------------"

# 3. Quy tắc biên dịch (Compiling): Biến .cpp thành .o
# $< : File nguồn (.cpp)
# $@ : File đích (.o)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 4. Quy tắc dọn dẹp (Xóa các file .o và file chạy cũ)
clean:
	rm -f $(OBJS) $(TARGET)
	rm -f src/*.o

# 5. Quy tắc chạy nhanh (Build xong chạy luôn)
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run