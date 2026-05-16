# Phần Mềm Quản Lý Bán Hàng Siêu Thị

## Giới thiệu
Phần mềm quản lý bán hàng siêu thị với đầy đủ chức năng quản lý kho, bán hàng, khách hàng và báo cáo.

## Yêu cầu hệ thống
- Windows 10/11 (64-bit)
- Không cần cài đặt Qt

## Hướng dẫn cài đặt

### Cách 1: Sử dụng file đã đóng gói
1. Giải nén file `PhanMemBanHang_v1.0.zip`
2. Chạy file `phan_mem_ban_hang_st.exe`

### Cách 2: Build từ source code
1. Mở Qt Creator
2. Chọn chế độ **Release** (góc trái dưới)
3. Build project (Ctrl+B)
4. Chạy script `package.bat` để đóng gói

## Tài khoản đăng nhập mặc định

### Admin (Toàn quyền)
- Mã NV: `NV001`
- Mật khẩu: `123456`

### Quản lý (Quản lý kho + Bán hàng)
- Mã NV: `NV002`
- Mật khẩu: `123456`

### Thu ngân (Chỉ bán hàng)
- Mã NV: `NV003`
- Mật khẩu: `123456`

## Chức năng chính

### 1. Tổng quan (Dashboard)
- Thống kê tổng số hàng hóa
- Cảnh báo hàng sắp hết hạn
- Thống kê khách hàng
- Cảnh báo hàng sắp hết kho

### 2. Quản lý kho
- Thêm/sửa/xóa hàng hóa
- Nhập kho
- Hủy hàng hết hạn
- Sắp xếp kho
- Tạo danh sách theo dõi
- Tìm kiếm và lọc theo loại

### 3. Bán hàng
- Thêm sản phẩm vào giỏ hàng
- Tìm kiếm khách hàng theo SĐT
- Áp dụng giảm giá tự động:
  - Giảm giá theo hạn sử dụng
  - Giảm giá theo hạng thành viên
- Thanh toán và in hóa đơn

### 4. Tra cứu hàng hóa
- Xem thông tin chi tiết sản phẩm
- Kiểm tra tồn kho
- Xem giá và hạn sử dụng

### 5. Báo cáo
- Báo cáo doanh thu
- Lịch sử hóa đơn (double-click để xem chi tiết)
- Lịch sử hủy hàng
- Thông tin khách hàng

## Phân quyền

| Chức năng | Admin | Quản lý | Thu ngân |
|-----------|-------|---------|----------|
| Tổng quan | ✓ | ✓ | ✓ |
| Quản lý kho | ✓ | ✓ | ✗ |
| Bán hàng | ✓ | ✓ | ✓ |
| Tra cứu hàng hóa | ✓ | ✓ | ✓ |
| Báo cáo | ✓ | ✓ | ✗ |

## Cấu trúc file dữ liệu

### data.txt (Hàng hóa)
Format: 7 dòng/sản phẩm
```
Loại (ThucPhamCheBien/DoUong/DoGiaDung/MyPham/HangTuoiSong)
Mã hàng
Tên hàng
Đơn vị
Vị trí kệ
Ngày hết hạn (dd/mm/yyyy)
giaNhap giaNiemYet soLuong
```

### khachhang.txt (Khách hàng)
Format: `Ma|Ten|SDT|TongTien|Diem|LoaiThanhVien`

### nhanvien.txt (Nhân viên)
Format: `Ma|Ten|SDT|MatKhau|ChucVu`
- ChucVu: Admin, QuanLy, ThuNgan

### lichsuhoadon.txt (Lịch sử hóa đơn)
Format:
```
MaHD|ThoiGian
||TenKH|SDT|TongGoc|TongGiam|TongThanhToan
MaHang|TenHang|SoLuong|DonGia|ThanhTien
...
===END===
```

### lichsuhuy.txt (Lịch sử hủy)
Format: `MaHang|TenHang|SoLuong|NgayHetHan|LyDo`

## Lưu ý
- Các file dữ liệu phải nằm cùng thư mục với file .exe
- Backup dữ liệu thường xuyên
- Không xóa các file .dll trong thư mục

## Hỗ trợ
Nếu gặp lỗi, kiểm tra:
1. File dữ liệu có đúng định dạng không
2. Các file .dll có đầy đủ không
3. Hệ điều hành Windows 10/11 64-bit

## Phiên bản
Version 1.0 - Tháng 5/2026

---
Phát triển bởi: Nhóm sinh viên KTLT
