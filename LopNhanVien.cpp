// =====================================================================
// PHẦN 4: NHÂN VIÊN
// =====================================================================

class NhanVien : public ConNguoi {
private:
    string chucVu;
    string caLam;
    string matKhau;

public:
    // Setter đặc biệt
    void setChucVu(const string& cv) { chucVu  = cv; }
    void setMatKhau(const string& mk){ matKhau = mk; }

    void nhap() override;
    void xuat() const override;

    string getLoai()   const override { return "NhanVien"; }
    string getChucVu() const { return chucVu; }

    // Kiểm tra mật khẩu đăng nhập
    bool kiemTraMatKhau(const string& mk) const { return matKhau == mk; }
};

void NhanVien::nhap() {
    // Nhập mã nhân viên
    while (true) {
        cout << "Nhap Ma NV (NVxxx): ";
        cin >> ma;
        if (ma.length() == 5 && ma.substr(0, 2) == "NV") break;
        cout << "Ma phai co dang NVxxx (vi du: NV001)!\n";
    }
    cin.ignore(1000, '\n');

    // Nhập tên
    while (true) {
        cout << "Nhap Ten: ";
        getline(cin, ten);
        ten = chuanHoaTen(ten);
        if (!ten.empty() && ten.length() >= 2) break;
        cout << "Ten khong hop le! Vui long nhap lai.\n";
    }

    // Nhập số điện thoại
    while (true) {
        cout << "Nhap SDT: ";
        getline(cin, sdt);
        if (chuanHoaSDT(sdt)) break;
        cout << "So dien thoai khong hop le! Vui long nhap lai.\n";
    }

    // Nhập chức vụ
    while (true) {
        cout << "Nhap Chuc vu (ThuNgan / QuanLy / Admin): ";
        getline(cin, chucVu);
        if (chucVu == "ThuNgan" || chucVu == "QuanLy" || chucVu == "Admin") break;
        cout << "Chi chap nhan: ThuNgan, QuanLy, hoac Admin!\n";
    }

    // Nhập ca làm
    while (true) {
        cout << "Nhap Ca lam (Sang / Chieu / Toi): ";
        getline(cin, caLam);
        if (caLam == "Sang" || caLam == "Chieu" || caLam == "Toi") break;
        cout << "Chi chap nhan: Sang, Chieu, hoac Toi!\n";
    }

    // Nhập mật khẩu
    cout << "Nhap Mat khau: ";
    cin >> matKhau;
}

void NhanVien::xuat() const {
    cout << "[NHAN VIEN] "
         << ma << " | " << ten
         << " | SDT: " << sdt
         << " | Chuc vu: " << chucVu
         << " | Ca lam: " << caLam
         << endl;
}

