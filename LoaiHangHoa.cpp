// =====================================================================
// PHẦN 7: 6 LỚP CON HÀNG HÓA
// =====================================================================

// --- Thực phẩm chế biến ---
class ThucPhamCheBien : public HangHoa {
public:
    void nhap() override {
        cout << "\n--- NHAP THUC PHAM CHE BIEN ---\n";
        HangHoa::nhap();
    }
    void xuat() override {
        HangHoa::xuat();
    }
    float tinhGiaBan() override { return giaNiemYet; }
    string getLoai() override   { return "ThucPhamCheBien"; }
    string getDefaultDvt()    const override { return "kg"; }
    string getDefaultKeHang() const override { return "F"; }
    void ghiVaoFile(ofstream &f) override { HangHoa::ghiVaoFile(f); }
    void docTuFile(ifstream &f) override  { HangHoa::docTuFile(f);  }
};

// --- Đồ uống ---
class DoUong : public HangHoa {
public:
    void nhap() override {
        cout << "\n--- NHAP DO UONG ---\n";
        HangHoa::nhap();
    }
    void xuat() override {
        HangHoa::xuat();
    }
    float tinhGiaBan() override { return giaNiemYet * 1.08f; }
    string getLoai() override   { return "DoUong"; }
    string getDefaultDvt()    const override { return "so luong"; }
    string getDefaultKeHang() const override { return "A"; }
    void ghiVaoFile(ofstream &f) override { HangHoa::ghiVaoFile(f); }
    void docTuFile(ifstream &f) override  { HangHoa::docTuFile(f);  }
};

// --- Đồ gia dụng ---
class DoGiaDung : public HangHoa {
private:
    int baoHanh; // Số tháng bảo hành

public:
    DoGiaDung() : baoHanh(0) {}

    void nhap() override {
        cout << "\n--- NHAP DO GIA DUNG ---\n";
        HangHoa::nhap();
        do {
            cout << "Bao hanh (thang): ";
            cin >> baoHanh;
        } while (baoHanh < 0);
    }
    void xuat() override {
        HangHoa::xuat();
        // In thêm thông tin bảo hành
        cout << "         => Bao hanh: " << baoHanh << " thang\n";
    }
    float tinhGiaBan() override { return giaNiemYet; }
    string getLoai() override   { return "DoGiaDung"; }
    string getDefaultDvt()    const override { return "so luong"; }
    string getDefaultKeHang() const override { return "B"; }
    void ghiVaoFile(ofstream &f) override {
        HangHoa::ghiVaoFile(f);
        f << baoHanh << endl;
    }
    void docTuFile(ifstream &f) override {
        HangHoa::docTuFile(f);
        f >> baoHanh;
        f.ignore(1000, '\n');
    }
};

// --- Mỹ phẩm ---
class MyPham : public HangHoa {
public:
    void nhap() override {
        cout << "\n--- NHAP MY PHAM ---\n";
        HangHoa::nhap();
    }
    void xuat() override {
        HangHoa::xuat();
    }
    float tinhGiaBan() override { return giaNiemYet * 1.12f; }
    string getLoai() override   { return "MyPham"; }
    string getDefaultDvt()    const override { return "so luong"; }
    string getDefaultKeHang() const override { return "C"; }
    void ghiVaoFile(ofstream &f) override { HangHoa::ghiVaoFile(f); }
    void docTuFile(ifstream &f) override  { HangHoa::docTuFile(f);  }
};

// --- Hàng tươi sống ---
class HangTuoiSong : public HangHoa {
private:
    float  khoiLuong;
    string tinhTrang;

public:
    HangTuoiSong() : khoiLuong(0.0f) {}

    void nhap() override {
        cout << "\n--- NHAP HANG TUOI SONG ---\n";
        HangHoa::nhap();
        cout << "Khoi luong (kg): ";
        cin >> khoiLuong;
        cin.ignore(1000, '\n');
        cout << "Tinh trang: ";
        getline(cin, tinhTrang);
    }
    void xuat() override {
        HangHoa::xuat();
        cout << "         => Khoi luong: " << khoiLuong << "kg"
             << " | Tinh trang: " << tinhTrang << "\n";
    }
    float tinhGiaBan() override { return giaNiemYet * 1.05f; }
    string getLoai() override   { return "HangTuoiSong"; }
    string getDefaultDvt()    const override { return "kg"; }
    string getDefaultKeHang() const override { return "F"; }
    float getKhoiLuong()      const override { return khoiLuong; }
    void ghiVaoFile(ofstream &f) override {
        HangHoa::ghiVaoFile(f);
        f << khoiLuong << " " << tinhTrang << endl;
    }
    void docTuFile(ifstream &f) override {
        HangHoa::docTuFile(f);
        f >> khoiLuong;
        f.ignore(1, ' ');
        getline(f, tinhTrang);
    }
};

// --- Trái cây ---
class TraiCay : public HangHoa {
private:
    float  khoiLuong;
    string tinhTrang;

public:
    TraiCay() : khoiLuong(0.0f) {}

    void nhap() override {
        cout << "\n--- NHAP TRAI CAY ---\n";
        HangHoa::nhap();
        cout << "Khoi luong (kg): ";
        cin >> khoiLuong;
        cin.ignore(1000, '\n');
        cout << "Tinh trang: ";
        getline(cin, tinhTrang);
    }
    void xuat() override {
        HangHoa::xuat();
        cout << "         => Khoi luong: " << khoiLuong << "kg"
             << " | Tinh trang: " << tinhTrang << "\n";
    }
    float tinhGiaBan() override { return giaNiemYet * 1.05f; }
    string getLoai() override   { return "TraiCay"; }
    string getDefaultDvt()    const override { return "kg"; }
    string getDefaultKeHang() const override { return "E"; }
    float getKhoiLuong()      const override { return khoiLuong; }
    void ghiVaoFile(ofstream &f) override {
        HangHoa::ghiVaoFile(f);
        f << khoiLuong << " " << tinhTrang << endl;
    }
    void docTuFile(ifstream &f) override {
        HangHoa::docTuFile(f);
        f >> khoiLuong;
        f.ignore(1, ' ');
        getline(f, tinhTrang);
    }
};
