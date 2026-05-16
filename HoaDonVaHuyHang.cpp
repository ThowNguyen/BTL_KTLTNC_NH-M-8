// =====================================================================
// PHẦN 10: CẤU TRÚC HÓA ĐƠN VÀ HỦY HÀNG
// =====================================================================

// Chi tiết từng dòng trong hóa đơn
struct ChiTietHoaDon {
    string maHang;
    string tenHang;
    float  giaGoc;
    float  giaSauGiam;
    int    soLuong;
    float  thanhTien;
};

// Hóa đơn đầy đủ
class HoaDonLichSu {
public:
    string maHoaDon;
    string thoiGian;
    string tenKhachHang;
    string sdtKhachHang;
    vector<ChiTietHoaDon> chiTiet;
    float tongGoc       = 0;
    float tongGiam      = 0;
    float tongThanhToan = 0;

    void inHoaDon() const;
    void ghiVaoFile(ofstream &f) const;
};

void HoaDonLichSu::inHoaDon() const {
    cout << "\n===================================================\n";
    cout << "               HOA DON BAN HANG\n";
    cout << "Ma hoa don : " << maHoaDon << "\n";
    cout << "Thoi gian  : " << thoiGian << "\n";
    if (!tenKhachHang.empty()) {
        cout << "Khach hang : " << tenKhachHang << " - SDT: " << sdtKhachHang << "\n";
    }
    cout << "---------------------------------------------------\n";
    cout << left
         << setw(8)  << "Ma"
         << setw(25) << "Ten hang"
         << setw(6)  << "SL"
         << setw(14) << "Gia goc"
         << setw(14) << "Giam gia"
         << "Thanh tien" << "\n";
    cout << "---------------------------------------------------\n";
    for (const auto& ct : chiTiet) {
        cout << left
             << setw(8)  << ct.maHang
             << setw(25) << ct.tenHang
             << setw(6)  << ct.soLuong
             << setw(14) << fixed << setprecision(0) << ct.giaGoc
             << setw(14) << fixed << setprecision(0) << (ct.giaGoc - ct.giaSauGiam)
             << fixed << setprecision(0) << ct.thanhTien << "\n";
    }
    cout << "---------------------------------------------------\n";
    cout << "Tong tien goc  : " << fixed << setprecision(0) << tongGoc       << " VND\n";
    cout << "Tong giam gia  : " << fixed << setprecision(0) << tongGiam      << " VND\n";
    cout << "THANH TOAN     : " << fixed << setprecision(0) << tongThanhToan << " VND\n";
    cout << "===================================================\n";
}

void HoaDonLichSu::ghiVaoFile(ofstream &f) const {
    f << maHoaDon << "|" << thoiGian << "|"
      << tenKhachHang << "|" << sdtKhachHang << "|"
      << tongGoc << "|" << tongGiam << "|" << tongThanhToan << "\n";
    for (const auto& ct : chiTiet) {
        f << ct.maHang << "|" << ct.tenHang << "|"
          << ct.soLuong << "|" << ct.giaGoc << "|"
          << ct.giaSauGiam << "|" << ct.thanhTien << "\n";
    }
    f << "===END===\n";
}

// Cấu trúc lưu thông tin hàng đã hủy
struct HangHuy {
    string maHang;
    string tenHang;
    string keHang;
    string ngayHetHan;
    string lyDo;
    string thoiGianHuy;
    string loaiHang;
    float  soLuong;
};

// Danh sách hàng hủy (chỉ dùng để ghi file lịch sử)
class DanhSachHangHuy {
private:
    vector<HangHuy> ds;
public:
    void them(const HangHuy& h) { ds.push_back(h); }

    void xuatLichSu() const {
        cout << "\n=== LICH SU HUY HANG ===\n";
        if (ds.empty()) {
            cout << "Chua co lich su huy hang.\n";
            return;
        }
        for (const auto& h : ds) {
            cout << h.thoiGianHuy << " | " << h.maHang << " | "
                 << h.tenHang << " | SL: " << h.soLuong
                 << " | Ly do: " << h.lyDo << "\n";
        }
    }

    void ghiFile(const string& filename) const {
        ofstream f(filename, ios::app);
        for (const auto& h : ds) {
            f << h.thoiGianHuy << "|" << h.maHang << "|"
              << h.tenHang << "|" << h.keHang << "|"
              << h.ngayHetHan << "|" << h.soLuong << "|"
              << h.lyDo << "|" << h.loaiHang << "\n";
        }
        f.close();
    }
};