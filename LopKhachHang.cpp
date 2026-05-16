// =====================================================================
// PHẦN 5: KHÁCH HÀNG
// =====================================================================

class KhachHang : public ConNguoi {
private:
    long long tongTien;
    int diem;
    string loaiThanhVien;

public:
    // Constructor khởi tạo giá trị mặc định
    KhachHang() : tongTien(0), diem(0), loaiThanhVien("Bac") {}

    void nhap() override;
    void xuat() const override;

    string getLoai() const override { return "KhachHang"; }
    string getLoaiThanhVien() const { return loaiThanhVien; }
    int getDiem() const { return diem; }
    long long getTongTien() const { return tongTien; }

    // Setter
    void setTongTien(long long t)       { tongTien = t; }
    void setDiem(int d)                 { diem = d; }
    void setLoaiThanhVien(const string& l) { loaiThanhVien = l; }

    // Tính phần trăm giảm giá theo loại thành viên
    float apDungGiamGia() const {
        if (loaiThanhVien == "Kim Cuong") return 0.08f;
        if (loaiThanhVien == "Vang")      return 0.05f;
        return 0.03f;
    }

    void congDiem(long long tienMua);
    void capNhatLoaiThanhVien();
    void capNhatTongTien(long long tienMua);
};

void KhachHang::nhap() {
    // Nhập mã khách hàng
    while (true) {
        cout << "Nhap Ma KH (KHxxx): ";
        cin >> ma;
        if (ma.length() == 5 && ma.substr(0, 2) == "KH") break;
        cout << "Ma phai co dang KHxxx (vi du: KH001)!\n";
    }
    cin.ignore(1000, '\n');

    // Nhập tên
    while (true) {
        cout << "Nhap Ten: ";
        getline(cin, ten);
        ten = chuanHoaTen(ten);
        if (!ten.empty() && ten.length() >= 2) break;
        cout << "Ten khong hop le!\n";
    }

    // Nhập số điện thoại
    while (true) {
        cout << "Nhap SDT: ";
        getline(cin, sdt);
        if (chuanHoaSDT(sdt)) break;
        cout << "So dien thoai khong hop le!\n";
    }
}

void KhachHang::xuat() const {
    cout << "[KHACH HANG] "
         << ma << " | " << ten
         << " | SDT: " << sdt
         << " | Loai: " << loaiThanhVien
         << " | Tong tien: " << tongTien
         << " | Diem: " << diem
         << endl;
}

void KhachHang::congDiem(long long tienMua) {
    if (tienMua <= 0) return;
    diem += (int)(tienMua / 10000);
    capNhatLoaiThanhVien();
}

void KhachHang::capNhatLoaiThanhVien() {
    if      (diem >= 500) loaiThanhVien = "Kim Cuong";
    else if (diem >= 100) loaiThanhVien = "Vang";
    else                  loaiThanhVien = "Bac";
}

void KhachHang::capNhatTongTien(long long tienMua) {
    tongTien += tienMua;
    capNhatLoaiThanhVien();
}

