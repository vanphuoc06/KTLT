# Finance Analysis Project (BTL02)

Dự án này là một chương trình C++ được thiết kế để xử lý dữ liệu tài chính (danh mục đầu tư), thực hiện các phép phân tích kỹ thuật và áp dụng các thuật toán giải quyết các bài toán về dãy số như tìm lợi nhuận tối đa, chuỗi tăng dài nhất, hay cửa sổ trượt (Sliding Window).

## Cấu trúc thư mục

- `src/`: Mã nguồn C++.
  - `main.cpp`: Tệp khởi chạy chính của chương trình.
  - `struct.h`: Định nghĩa các cấu trúc dữ liệu cơ bản như `Session`, `PortfolioStats`, `Stock` và `Portfolio`.
  - `io.h`, `io.cpp`: Đọc tệp đầu vào (cấu hình `config.txt`, dữ liệu danh mục `portfolio.csv`) và ghi kết quả tính toán ra các tệp phân tích (`Analysis.csv`, `Signals.csv`, `BestPeriod.csv`).
  - `processing.h`, `processing.cpp`: Các hàm xử lý cốt lõi, bao gồm: tính toán thống kê cơ bản, điền dữ liệu thiếu, tính trung bình động (MA), mảng cộng dồn (Prefix Sum), tối ưu lợi nhuận (Kadane), và Sliding Window cho cửa sổ ngắn nhất đạt mức lợi nhuận mục tiêu.
- `Input/`: Thư mục chứa dữ liệu đầu vào.
  - `config.txt`: Chứa các cấu hình tham số (ví dụ: `k`, `S`).
  - `portfolio.csv` (hoặc `portfolio3.csv`): Dữ liệu giao dịch theo từng phiên của các mã cổ phiếu.
- `Output/`: Thư mục chứa kết quả phân tích sau khi chạy chương trình.
  - `Analysis.csv`: Kết quả phân tích thống kê cơ bản.
  - `Signals.csv`: Tín hiệu giao dịch MA.
  - `BestPeriod.csv`: Kỳ đầu tư tối ưu nhất cho danh mục đầu tư.

## Chức năng chính

1. **Xử lý dữ liệu cơ bản**: Đọc file CSV, xử lý lại dữ liệu thiếu của các chuỗi quá khứ (`processing_missing_data`).
2. **Khảo sát Thống kê**: Tính giá Trị lớn nhất, nhỏ nhất, trung bình và độ lệch chuẩn của các mã cổ phiếu (`calculate_Statistics`).
3. **Trung bình động (Moving Average - MA)**: Tính đường MA dựa vào tham số $k$ đầu vào (`calculate_MA`).
4. **Phân tích chiến lược đầu tư**:
   - Khảo sát các chuỗi tăng liên tiếp dài nhất (Longest Increasing Subarray).
   - Tối đa hóa lợi nhuận đầu tư dựa trên biểu đồ giá.
   - Sử dụng Sliding Window để tìm khoảng cách ngắn nhất đạt lợi nhuận cho trước ($S$).
   - Sử dụng Prefix Sum cho các truy vấn tính tổng biến động nhanh trên khoảng lớn.

## Hướng dẫn cài đặt và chạy (Build Instructions)

Yêu cầu máy tính của bạn đã cài đặt trình biên dịch `g++` (ví dụ như MinGW trên Windows, hoặc qua GCC trên Linux/macOS). Tắt Terminal và mở một cửa sổ làm việc mới tại thư mục gốc của dự án.

1. **Biên dịch mã nguồn** bằng lệnh sau trên Terminal/Command Prompt:
   ```bash
   g++ src\main.cpp src\io.cpp src\processing.cpp -o app.exe
   ```

2. **Khởi chạy ứng dụng**:
   ```bash
   .\app.exe
   ```

Khi chương trình chạy thành công, Terminal sẽ xuất ra chuỗi `"Chuong trinh da chay xong"` và bạn có thể xem các báo cáo kết quả trong thư mục `Output/`.
