# TÀI LIỆU THIẾT KẾ BÀI TẬP LỚN

**Môn học:** Kỹ thuật lập trình (2025-2026)
**Đề tài:** BTL02 - Hệ thống phân tích dữ liệu danh mục đầu tư chứng khoán

---

## 1. Phân tích bài toán (System Analysis)

### Mô tả bài toán

Xây dựng một chương trình C++ để phân tích dữ liệu giao dịch chứng khoán từ file CSV. Chương trình cần tính toán các chỉ số tài chính cơ bản, phát hiện xu hướng giá và xác định kỳ đầu tư tối ưu cho nhà đầu tư.

### Dữ liệu đầu vào

| File | Định dạng | Mô tả |
|---|---|---|
| `config.txt` | Text | Hai tham số trên hai dòng: `k` (chu kỳ MA) và `S` (ngưỡng mục tiêu) |
| `*.csv` | CSV | Mỗi dòng gồm 4 cột theo thứ tự: `close`, `ticker`, `volume`, `date`. Dòng đầu là header. Giá trị thiếu được đánh dấu bằng `NA`. |

### Dữ liệu đầu ra

| File | Mô tả |
|---|---|
| `Analysis.csv` | Dữ liệu sau khi chuẩn hóa Min-Max: `Ticker, Date, Close (đã scale), Volume, MA` |
| `signals.txt` | Chuỗi ngày tăng giá liên tiếp dài nhất + cửa sổ ngắn nhất đạt tổng giá trị `>= S` |
| `best_period.txt` | Ngày mua, ngày bán và lợi nhuận tối đa tính được bằng thuật toán Kadane |

### Quy trình xử lý (Pipeline)

```
Đọc config.txt & CSV
        ↓
Tiền xử lý dữ liệu thiếu (NA → nội suy phiên liền kề)
        ↓
Tính Moving Average k ngày (dùng Prefix Sum)
        ↓
Phân tích tín hiệu: LIS & Sliding Window  ──→  Xuất signals.txt
        ↓
Tìm lợi nhuận tối đa (Kadane)             ──→  Xuất best_period.txt
        ↓
Chuẩn hóa Min-Max + Tính thống kê         ──→  Xuất Analysis.csv
```

> **Lưu ý thiết kế:** Hàm `calculate_Statistics()` thực hiện Min-Max Normalization **sau cùng** vì nó ghi đè trực tiếp lên trường `close`. Các hàm tính toán `Signals` và `BestPeriod` phải được gọi **trước** bước này để đảm bảo làm việc trên dữ liệu giá gốc.

---

## 2. Thiết kế cấu trúc dữ liệu (Data Structure)

Hệ thống dùng cấu trúc **Struct lồng nhau 3 cấp** ánh xạ trực tiếp theo mô hình dữ liệu tài chính thực tế:

```
Portfolio  (Toàn bộ danh mục)
 └── Stock[]  (Từng mã cổ phiếu, VD: AAL, AAPL)
      ├── Session[]  (Từng phiên giao dịch theo ngày)
      │    ├── date    : string  -- Ngày giao dịch
      │    ├── close   : double  -- Giá đóng cửa
      │    ├── volume  : long    -- Khối lượng khớp lệnh
      │    └── ma      : double  -- Giá trị Moving Average
      └── PortfolioStats
           ├── min_price : double
           ├── max_price : double
           ├── mean      : double
           └── std       : double
```

---

## 3. Sơ đồ phân cấp hàm (Functional Hierarchy)

Chương trình được tổ chức thành **3 module độc lập**, tuân theo nguyên tắc tách biệt trách nhiệm (Separation of Concerns):

```
main.cpp  (Điều phối luồng hoạt động)
├── Module IO  (io.h / io.cpp)
│   ├── read_configTXT()            -- Đọc tham số k, S từ config.txt
│   ├── read_PortfolioCSV()         -- Đọc & phân tích dữ liệu từ file CSV
│   ├── Signals()                   -- Xuất signals.txt
│   ├── BestPeriod()                -- Xuất best_period.txt
│   └── Analysis()                  -- Xuất Analysis.csv
└── Module Processing  (processing.h / processing.cpp)
    ├── check_empty()                         -- Kiểm tra mảng rỗng (guard check)
    ├── processing_missing_data()             -- Điền dữ liệu thiếu NA
    ├── calculate_Statistics()                -- Thống kê & Chuẩn hóa Min-Max
    ├── calculate_MA()                        -- Tính Moving Average
    ├── calculate_MaxProfit()                 -- Kadane: Lợi nhuận tối đa
    ├── Longest_Increasing_Subarray()         -- LIS: Chuỗi tăng dài nhất
    ├── findShortestWindowForTargetProfit()   -- Sliding Window: Cửa sổ ngắn nhất đạt S
    ├── Prefix_Sum()                          -- Xây dựng mảng cộng dồn
    └── Query_PrefixSum()                     -- Truy vấn tổng đoạn con O(1)
```

---

## 4. Mô tả chi tiết hàm (Function Specification)

| Tên hàm | Input | Output | Mô tả |
|---|---|---|---|
| `read_configTXT` | `string path`, `int &k`, `double &S` | `void` | Đọc `config.txt`, gán `k` và `S`. |
| `read_PortfolioCSV` | `string path`, `Portfolio &p` | `void` | Đọc CSV, xử lý `NA`, nhóm `Session` vào đúng `Stock`. |
| `check_empty` | `Stock &s`, `string &start`, `string &end` | `int` (0/1) | Trả về 0 nếu mảng phiên rỗng, đồng thời gán `start = end = "NA"`. |
| `processing_missing_data` | `Portfolio &p` | `void` | Duyệt mọi phiên, nếu `close == -1` thì nội suy từ phiên trước/sau. |
| `calculate_Statistics` | `Portfolio &p` | `void` | Tính `min, max, mean, std`; sau đó ghi đè `close` bằng giá trị đã chuẩn hóa Min-Max. |
| `calculate_MA` | `Portfolio &p`, `int k` | `void` | Dùng Prefix Sum để tính MA cửa sổ `k` ngày. |
| `calculate_MaxProfit` | `Stock &s`, `string &start`, `string &end` | `double` | Kadane trên mảng hiệu `diff[i] = close[i] - close[i-1]`. Trả về lợi nhuận cao nhất. |
| `Longest_Increasing_Subarray` | `Stock &s`, `string &start`, `string &end` | `int` | Quét O(N), đếm chuỗi liên tiếp tăng, trả về độ dài dài nhất kèm khoảng ngày. |
| `findShortestWindowForTargetProfit` | `Stock &s`, `double &target`, `string &start`, `string &end` | `int` | Sliding Window + Prefix Sum, tìm cửa sổ ngắn nhất có tổng `close >= S`. Trả về `-1` nếu không tồn tại. |
| `Prefix_Sum` | `Stock &s` | `vector<double>` | Trả về mảng cộng dồn kích thước `N+1` với `prefix[0] = 0`. |
| `Query_PrefixSum` | `vector<double> &prefix`, `int l`, `int r`, `double &result` | `void` | `result = prefix[r+1] - prefix[l]`. Gán `-1` nếu chỉ số không hợp lệ. |

---

## 5. Phân tích thuật toán (Algorithm Analysis)

| Thuật toán | Hàm áp dụng | Độ phức tạp | Mô tả ngắn |
|---|---|---|---|
| **Kadane** | `calculate_MaxProfit` | O(N) thời gian, O(1) không gian | Duyệt mảng hiệu giá, duy trì tổng tích lũy. Reset khi tổng âm. |
| **LIS tuyến tính** | `Longest_Increasing_Subarray` | O(N) thời gian, O(1) không gian | So sánh hai phiên liền kề, đếm chuỗi tăng liên tiếp. |
| **Prefix Sum** | `Prefix_Sum` + `Query_PrefixSum` | O(N) xây dựng, O(1) truy vấn | `prefix[i] = prefix[i-1] + close[i-1]`. Công thức: `Sum[l..r] = prefix[r+1] - prefix[l]`. |
| **Sliding Window** | `findShortestWindowForTargetProfit` | O(N) thời gian, O(N) không gian (prefix) | Hai con trỏ `left/right`, mở rộng `right` và co `left` để thu nhỏ cửa sổ. |
| **Min-Max Normalization** | `calculate_Statistics` | O(N) thời gian, O(1) không gian | Công thức: `x' = (x - min) / (max - min)`. Ghi đè trực tiếp lên `close`. |

### Ví dụ minh họa thuật toán Kadane

Với chuỗi giá: `[10, 12, 9, 14, 11]`

```
Mảng hiệu:  [+2, -3, +5, -3]

i=1: diff=+2  → MaxEndingHere=2,  MaxProfit=2   (mua ngày 0, bán ngày 1)
i=2: diff=-3  → MaxEndingHere=-1 → Reset=−3
i=3: diff=+5  → MaxEndingHere=2,  MaxProfit=2   (mua ngày 2, bán ngày 3)
               (không cập nhật vì 2 = MaxProfit)
→ Lợi nhuận tối đa = 2 (mua ở 10, bán ở 12  hoặc  mua ở 9, bán ở 14)
```

---

## 6. Xử lý trường hợp đặc biệt (Edge Case Handling)

Chương trình được thiết kế để xử lý an toàn các tình huống bất thường mà không bị crash:

| Tình huống | Cách xử lý |
|---|---|
| **File không tồn tại** | `is_open()` kiểm tra trước khi đọc/ghi. In ra `"File not found"` và `return` ngay lập tức. |
| **File CSV rỗng (chỉ có header)** | `Portfolio.stocks` rỗng. Vòng lặp không thực thi. Output files được tạo nhưng không có nội dung cổ phiếu. |
| **Dữ liệu thiếu (NA)** | `read_PortfolioCSV` gán `close = -1`. `processing_missing_data` nội suy từ phiên trước/sau, hoặc gán `0` nếu chỉ có 1 phiên. |
| **Chỉ có 1 phiên giao dịch** | `calculate_MaxProfit` trả về `0` (không thể mua rồi bán). `Longest_Increasing_Subarray` trả về `1`. |
| **Giá chỉ giảm liên tục** | Kadane trả về giá trị âm. `BestPeriod` vẫn in ngày mua/bán nhưng kèm thêm dòng khuyến nghị không nên giao dịch. |
| **LIS không tìm được chuỗi tăng** | Mặc định trả về `1` (mọi phiên đơn lẻ đều là chuỗi dài 1). |
| **Sliding Window không đạt ngưỡng S** | `min_len` vẫn bằng `1e9` sau khi duyệt xong → trả về `-1`. Output ghi `"Khong dat duoc muc tieu"`. |

---

## 7. Kế hoạch kiểm thử (Testing Plan)

Hệ thống được kiểm thử với **5 bộ dữ liệu** bao gồm cả trường hợp thường và các trường hợp biên:

| Mã Test | File | Nội dung | Mục đích kiểm thử |
|---|---|---|---|
| TC1 | `test_logic_fluctuating.csv` | Nhiều phiên, giá lên xuống đan xen, không có NA | Toàn bộ pipeline: MA, Kadane, LIS, Sliding Window |
| TC2 | `test_edge_1_row.csv` | Chỉ có **1 dòng** dữ liệu duy nhất | Edge Case: không crash, output hợp lệ với 1 phiên |
| TC3 | `test_edge_empty.csv` | File CSV **không có dữ liệu** (chỉ có header) | Edge Case: mảng rỗng, không truy cập ngoài vùng nhớ |
| TC4 | `test_edge_decreasing.csv` | Giá **liên tục giảm** toàn bộ chuỗi | Kadane với Bear Market: lợi nhuận âm + khuyến nghị |
| TC5 | `test_edge_big_data.csv` | Tập dữ liệu lớn nhiều phiên | Stress Test: hiệu năng, không bị treo hay tràn bộ nhớ |

**Kỳ vọng Output:**

- **TC1:** `best_period.txt` ghi lợi nhuận dương kèm ngày mua/bán. `signals.txt` ghi chuỗi tăng > 1 và cửa sổ đạt S. `Analysis.csv` ghi giá chuẩn hóa `[0, 1]`.
- **TC2:** `best_period.txt` ghi `Loi nhuan cao nhat : 0`, ngày mua = ngày bán. `signals.txt` ghi chuỗi dài nhất = 1 phiên.
- **TC3:** Tất cả output files được tạo ra nhưng không có nội dung cổ phiếu bên trong.
- **TC4:** `best_period.txt` ghi lợi nhuận âm + `Khuyen nghi: Khong nen thuc hien mua`. `signals.txt` ghi chuỗi tăng = 1 phiên.
- **TC5:** Chương trình chạy hoàn thành, kết quả đúng trong thời gian hợp lý.

---

## 8. Cấu trúc thư mục dự án

```
Program(KTLT)/
├── src/
│   ├── struct.h          -- Định nghĩa cấu trúc dữ liệu (Session, Stock, Portfolio...)
│   ├── io.h              -- Khai báo các hàm đọc/ghi file
│   ├── io.cpp            -- Triển khai các hàm đọc/ghi file
│   ├── processing.h      -- Khai báo các hàm xử lý và thuật toán
│   └── processing.cpp    -- Triển khai các hàm xử lý và thuật toán
│   └── main.cpp          -- Điểm khởi chạy, điều phối luồng xử lý
├── Input/
│   ├── config.txt                   -- Tham số cấu hình (k và S)
│   ├── test_logic_fluctuating.csv   -- TC1: Dữ liệu thường
│   ├── test_edge_1_row.csv          -- TC2: 1 phiên giao dịch
│   ├── test_edge_empty.csv          -- TC3: File rỗng
│   ├── test_edge_decreasing.csv     -- TC4: Giá giảm liên tục
│   └── test_edge_big_data.csv       -- TC5: Dữ liệu lớn
├── Output/
│   ├── Analysis.csv      -- Giá đã chuẩn hóa + MA
│   ├── signals.txt       -- Tín hiệu LIS và Sliding Window
│   └── best_period.txt   -- Kỳ đầu tư tối ưu (Kadane)
├── app.exe               -- File thực thi sau khi biên dịch
└── README.md             -- Tài liệu thiết kế này
```

---

## 9. Hướng dẫn biên dịch và chạy chương trình (Build Instructions)

**Yêu cầu:** Máy tính đã cài đặt trình biên dịch `g++` (MinGW trên Windows hoặc GCC trên Linux/macOS).

**Bước 1 – Biên dịch mã nguồn** (chạy tại thư mục gốc của dự án):
```bash
g++ src\main.cpp src\io.cpp src\processing.cpp -o app.exe
```

**Bước 2 – Khởi chạy chương trình:**
```bash
.\app.exe
```

Khi chương trình hoàn thành, Terminal xuất ra thông báo:
```
Chuong trinh da chay xong
```

Kết quả phân tích được lưu trong thư mục `Output/` gồm 3 file: `Analysis.csv`, `signals.txt`, `best_period.txt`.

> **Để thay đổi file dữ liệu đầu vào:** Mở `src/main.cpp` và sửa tên file trong dòng `read_PortfolioCSV("Input/ten_file.csv", portfolio);` rồi biên dịch lại.
