=====================================================================
// PHẦN 2: XỬ LÝ NGÀY THÁNG
// =====================================================================

// Kiểm tra năm nhuận
bool laNamNhuan(int y) {
    return (y % 400 == 0) || (y % 4 == 0 && y % 100 != 0);
}

// Kiểm tra ngày hợp lệ
bool ngayHopLe(int d, int m, int y) {
    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12) return false;

    int soNgayTrongThang[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (laNamNhuan(y)) soNgayTrongThang[2] = 29;

    return (d >= 1 && d <= soNgayTrongThang[m]);
}

// Kiểm tra ngày có phải trong quá khứ không
bool laNgayQuaKhu(int d, int m, int y) {
    time_t now = time(nullptr);
    tm homNay = *localtime(&now);
    homNay.tm_hour = 0;
    homNay.tm_min  = 0;
    homNay.tm_sec  = 0;
    homNay.tm_isdst = -1;

    tm ngayNhap = {};
    ngayNhap.tm_mday = d;
    ngayNhap.tm_mon  = m - 1;
    ngayNhap.tm_year = y - 1900;
    ngayNhap.tm_hour = 0;
    ngayNhap.tm_min  = 0;
    ngayNhap.tm_sec  = 0;
    ngayNhap.tm_isdst = -1;

    time_t tNhap = mktime(&ngayNhap);
    time_t tNow  = mktime(&homNay);

    if (tNhap == -1 || tNow == -1) return true;

    return difftime(tNhap, tNow) < 0;
}

// Hàm nhập ngày hợp lệ (không cho nhập ngày quá khứ)
string nhapNgayHopLe() {
    string ngay;
    while (true) {
        cout << "Nhap ngay het han (dd/mm/yyyy): ";
        getline(cin, ngay);

        int d, m, y;
        char c1, c2;
        stringstream ss(ngay);

        if (!(ss >> d >> c1 >> m >> c2 >> y) || c1 != '/' || c2 != '/') {
            cout << "Sai dinh dang! Vui long nhap theo dang dd/mm/yyyy\n";
            continue;
        }
        if (!ngayHopLe(d, m, y)) {
            cout << "Ngay khong hop le! Vui long nhap lai.\n";
            continue;
        }
        if (laNgayQuaKhu(d, m, y)) {
            cout << "Khong duoc nhap ngay trong qua khu!\n";
            continue;
        }

        // Định dạng lại cho chuẩn dd/mm/yyyy
        stringstream out;
        out << setw(2) << setfill('0') << d << "/"
            << setw(2) << setfill('0') << m << "/"
            << y;
        return out.str();
    }
}
