// =====================================================================
// PHẦN 11: LỚP CỬA HÀNG (TRUNG TÂM ĐIỀU PHỐI)
// =====================================================================

class CuaHang {
private:
    DanhSachHangHoa        ds;
    DanhSach<KhachHang>    dsKhachHang;
    DanhSach<NhanVien>     dsNhanVien;
    NhanVien*              nhanVienHienTai;
    vector<HoaDonLichSu>   lichSuHoaDon;

public:
    CuaHang() : nhanVienHienTai(nullptr) {}

    // --- Đọc / ghi file ---
    void docFile(const string& filename);
    void ghiFile(const string& filename);
    void docKhachHang(const string& filename);
    void ghiKhachHang(const string& filename);
    void docNhanVien(const string& filename);
    void ghiNhanVien(const string& filename);

    // --- Đăng nhập ---
    bool dangNhap();
    bool coQuyen(const string& quyen);

    // =================================================================
    // MENU CHÍNH
    // =================================================================
    void menuChinh();

    // =================================================================
    // MENU QUẢN LÝ KHO (3 chức năng chính)
    //   1. Nhap hang vao kho
    //   2. Sua thong tin hang  (Admin only - sua + xoa vinh vien)
    //   3. Xem toan bo kho hang
    // =================================================================
    void menuQuanLyKho();
    void nhapHangVaoKho();
    void suaVaXoaHang();       // Mục 2: gồm cả sửa và xóa vĩnh viễn
    void xemToanBoKhoHang();   // Mục 3: hiển thị theo phân loại + submenu

    // --- Submenu của "Xem toàn bộ kho" ---
    void huyHangHetHan();             // Hủy hàng đã hết hạn (≤ 0 ngày)
    void sapXepKho();                 // Sắp xếp toàn bộ kho
    void taoDanhSachTheoDoiHetHan();  // Tạo file theo dõi hàng < 20 ngày

    // =================================================================
    // MENU TRA CỨU THÔNG TIN HÀNG (menu chính mục 3)
    //   1. Tra cuu hang hoa
    //   2. Loc hang theo loai
    // =================================================================
    void menuTraCuuThongTin();
    void traCuuHangHoa();      // Tìm kiếm → tiêu chí → tăng/giảm → hiển thị
    void locHangTheoLoai();    // Chọn loại rồi hiển thị

    // =================================================================
    // BÁO CÁO & THỐNG KÊ (menu chính mục 4)
    // =================================================================
    void menuBaoCao();
    void baoCaoTongVon();

    // --- Nâng cấp báo cáo doanh thu ---
    struct KetQuaDoanhThu {
        float tongBanDuoc  = 0;  // Tổng tiền bán (sau giảm giá)
        float tongGiaGoc   = 0;  // Tổng giá gốc hàng bán
        float tongGiam     = 0;  // Tổng tiền đã giảm
        float doanhThuRong = 0;  // Doanh thu ròng = bán - gốc
        int   soHoaDon     = 0;  // Số hóa đơn trong kỳ
        int   soHangHuy    = 0;  // Số dòng hủy hàng trong kỳ
    };

    // Hàm tái sử dụng: lọc và tính doanh thu theo ngày hoặc tháng
    KetQuaDoanhThu tinhDoanhThuKy(const string& loai,
                                   int ngay, int thang, int nam);

    // Báo cáo doanh thu lời lỗ (mục 1 mới)
    void baoCaoDoanhThu();

    // Xem lịch sử hủy hàng (mục 2 mới - đọc file)
    void xemLichSuHuyHang();

    // Xem lịch sử hóa đơn (mục 3 mới - dùng lichSuHoaDon)
    void xemLichSuHoaDon();

    // Xem thông tin khách hàng (mục 4 mới - dùng dsKhachHang)
    void xemThongTinKhachHang();

    // Lưu báo cáo doanh thu ra file txt
    void luuBaoCaoRaFile(const KetQuaDoanhThu& kq,
                          const string& tieuDe,
                          const string& kyBaoCao);

    // =================================================================
    // BÁN HÀNG (menu chính mục 2)
    // =================================================================
    void banHang();

    // --- Tiện ích ---
    void hienThiCanhBaoKho();
    void themHangMoi();
};
