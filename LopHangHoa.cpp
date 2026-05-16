// =====================================================================
// PHẦN 6: HÀNG HÓA (BASE CLASS)
// =====================================================================

class HangHoa {
protected:
    string ma;
    string ten;
    string dvt;
    string keHang;
    string ngayHetHan;
    float  giaNhap;
    float  giaNiemYet;
    int    soLuong;

public:
    virtual ~HangHoa() {}

    // Hàm thuần ảo - lớp con bắt buộc cài đặt
    virtual float  tinhGiaBan()       = 0;
    virtual string getLoai()          = 0;
    virtual string getDefaultDvt()    const = 0;
    virtual string getDefaultKeHang() const = 0;

    // Hàm ảo - lớp con có thể override
    virtual void nhap();
    virtual void xuat();
    virtual float  tinhGiaBanSauGiamHetHan();
    virtual float  getKhoiLuong() const { return 0.0f; }
    virtual void ghiVaoFile(ofstream &f);
    virtual void docTuFile(ifstream &f);
    virtual void nhapKhongMa();

    // Getter
    string getMa()         const { return ma; }
    string getTen()        const { return ten; }
    string getDvt()        const { return dvt; }
    string getKeHang()     const { return keHang; }
    string getNgayHetHan() const { return ngayHetHan; }
    int    getSoLuong()    const { return soLuong; }
    float  getGiaNhap()    const { return giaNhap; }
    float  getGiaNiemYet() const { return giaNiemYet; }

    // Setter
    void setMa(const string& m)         { ma         = m; }
    void setTen(const string& t)        { ten        = t; }
    void setDvt(const string& d)        { dvt        = d; }
    void setKeHang(const string& k)     { keHang     = k; }
    void setNgayHetHan(const string& n) { ngayHetHan = n; }
    void setGiaNhap(float g)            { giaNhap    = g; }
    void setGiaNiemYet(float g)         { giaNiemYet = g; }
    void setSoLuong(int sl)             { soLuong    = sl; }

    // Tính số ngày còn lại đến hết hạn
    int tinhSoNgayConLai() const;
    // Trả về % giảm giá do gần hết hạn (0.0 / 0.10 / 0.20 / 0.30)
    float tinhPctGiamHetHan() const;

    // Thêm/bớt số lượng
    void giamSoLuong(int sl) { if (soLuong >= sl) soLuong -= sl; }
    void tangSoLuong(int sl) { soLuong += sl; }
};

void HangHoa::nhap() {
    // Nhập mã hàng
    do {
        cout << "Ma hang: ";
        cin >> ma;
    } while (ma.empty());
    cin.ignore(1000, '\n');

    // Nhập tên hàng
    cout << "Ten hang: ";
    getline(cin, ten);

    // Lấy giá trị mặc định rồi cho phép nhập lại
    dvt    = getDefaultDvt();
    keHang = getDefaultKeHang();

    cout << "Don vi tinh (cai/kg/hop/chai/goi/) [mac dinh: " << dvt << "]: ";
    string dvtNhap;
    getline(cin, dvtNhap);
    if (!dvtNhap.empty()) dvt = dvtNhap;

    cout << "Ke hang A-F (mac dinh: " << keHang << "): ";
    string keNhap;
    getline(cin, keNhap);
    if (!keNhap.empty()) keHang = keNhap;

    // Nhập ngày hết hạn
    ngayHetHan = nhapNgayHopLe();

    // Nhập giá nhập
    do {
        cout << "Gia nhap: ";
        cin >> giaNhap;
    } while (giaNhap <= 0);

    // Nhập giá niêm yết (phải >= giá nhập)
    do {
        cout << "Gia niem yet: ";
        cin >> giaNiemYet;
        if (giaNiemYet < giaNhap)
            cout << "[!] Gia niem yet phai >= gia nhap! Nhap lai.\n";
    } while (giaNiemYet < giaNhap);

    // Nhập số lượng
    do {
        cout << "So luong: ";
        cin >> soLuong;
    } while (soLuong < 0);
}

void HangHoa::xuat() {
    // Tên hiển thị có thể gồm tên + khối lượng (nếu có)
    string tenHienThi = ten;
    float kl = getKhoiLuong();
    if (kl > 0.0f) {
        stringstream ss;
        ss << fixed << setprecision(1) << kl << "kg";
        tenHienThi += " " + ss.str();
    }

    cout << left
         << setw(8)  << ma
         << setw(18) << ("[" + getLoai() + "]")
         << setw(28) << tenHienThi
         << setw(8)  << ("Ke:" + keHang)
         << setw(10) << ("SL:" + to_string(soLuong))
         << setw(20) << ("Gia nhap:" + to_string((int)giaNhap))
         << setw(24) << ("Gia niem yet:" + to_string((int)giaNiemYet))
         << "HSD:" << ngayHetHan
         << endl;
}

float HangHoa::tinhPctGiamHetHan() const {
    int ngayCon = tinhSoNgayConLai();
    if (ngayCon <= 3)  return 0.30f;
    if (ngayCon <= 7)  return 0.20f;
    if (ngayCon <= 15) return 0.10f;
    return 0.0f;
}

float HangHoa::tinhGiaBanSauGiamHetHan() {
    return giaNiemYet * (1.0f - tinhPctGiamHetHan());
}

int HangHoa::tinhSoNgayConLai() const {
    if (ngayHetHan.empty()) return 999;

    int d, m, y;
    char c1, c2;
    stringstream ss(ngayHetHan);
    if (!(ss >> d >> c1 >> m >> c2 >> y)) return 999;

    time_t now = time(nullptr);
    tm homNay = *localtime(&now);
    homNay.tm_hour  = 0;
    homNay.tm_min   = 0;
    homNay.tm_sec   = 0;
    homNay.tm_isdst = -1;

    tm ngayHan = {};
    ngayHan.tm_mday  = d;
    ngayHan.tm_mon   = m - 1;
    ngayHan.tm_year  = y - 1900;
    ngayHan.tm_hour  = 0;
    ngayHan.tm_min   = 0;
    ngayHan.tm_sec   = 0;
    ngayHan.tm_isdst = -1;

    time_t t1 = mktime(&ngayHan);
    time_t t2 = mktime(&homNay);
    if (t1 == -1 || t2 == -1) return 999;

    return (int)(difftime(t1, t2) / (60 * 60 * 24));
}

void HangHoa::ghiVaoFile(ofstream &f) {
    f << getLoai()    << endl;
    f << ma           << endl;
    f << ten          << endl;
    f << dvt          << endl;
    f << keHang       << endl;
    f << ngayHetHan   << endl;
    f << giaNhap << " " << giaNiemYet << " " << soLuong << endl;
}

void HangHoa::docTuFile(ifstream &f) {
    getline(f, ma);
    if (ma.empty() || ma == "\r") getline(f, ma);
    getline(f, ten);
    getline(f, dvt);
    getline(f, keHang);
    getline(f, ngayHetHan);
    f >> giaNhap >> giaNiemYet >> soLuong;
    f.ignore(1000, '\n');
}

void HangHoa::nhapKhongMa() {
    // Dùng khi sửa thông tin - không cần nhập lại mã
    cin.ignore(1000, '\n');

    cout << "Ten hang: ";
    getline(cin, ten);

    dvt    = getDefaultDvt();
    keHang = getDefaultKeHang();

    cout << "Don vi tinh (cai/kg/hop/chai/goi/) [mac dinh: " << dvt << "]: ";
    string dvtNhap;
    getline(cin, dvtNhap);
    if (!dvtNhap.empty()) dvt = dvtNhap;

    cout << "Ke hang A-F (mac dinh: " << keHang << "): ";
    string keNhap;
    getline(cin, keNhap);
    if (!keNhap.empty()) keHang = keNhap;

    ngayHetHan = nhapNgayHopLe();

    do { cout << "Gia nhap: ";    cin >> giaNhap;    } while (giaNhap <= 0);
    do {
        cout << "Gia niem yet: "; cin >> giaNiemYet;
        if (giaNiemYet < giaNhap)
            cout << "[!] Gia niem yet phai >= gia nhap! Nhap lai.\n";
    } while (giaNiemYet < giaNhap);
    do { cout << "So luong: ";    cin >> soLuong;    } while (soLuong < 0);
}
