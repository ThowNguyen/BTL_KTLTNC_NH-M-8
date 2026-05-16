// =====================================================================
// PHẦN 1: CÁC HÀM HỖ TRỢ CHUNG
// =====================================================================

// Hàm bỏ dấu tiếng Việt và chuyển về chữ thường (dùng cho tìm kiếm)
string boDauVaInThuong(string str) {
    string result = str;
    for (int i = 0; i < (int)result.size(); i++) {
        unsigned char uc = (unsigned char)result[i];
        uc = tolower(uc);
        result[i] = uc;

        switch (uc) {
            case 'a': break;
            case 'e': break;
        }

        // Xử lý dấu tiếng Việt - nguyên âm A
        if (uc == 'a' || uc == '\xe1' || uc == '\xe0' || uc == '\xe3' || uc == '\xa3') result[i] = 'a';
        // (xu ly them qua vong lap UTF-8 ben duoi)
    }

    // Dùng cách đơn giản hơn: replace từng ký tự UTF-8 đặc biệt
    string res = "";
    for (int i = 0; i < (int)str.size(); i++) {
        unsigned char c = (unsigned char)str[i];
        char lower_c = tolower((char)c);

        // Ký tự ASCII bình thường
        if (c < 128) {
            res += lower_c;
            continue;
        }

        // Ký tự UTF-8 2 byte (tiếng Việt cơ bản)
        if (i + 1 < (int)str.size()) {
            unsigned char c2 = (unsigned char)str[i + 1];

            // á à ả ã ạ => a
            if ((c == 0xC3 && (c2 == 0xA1 || c2 == 0xA0 || c2 == 0xA3)) ||
                (c == 0xE1 && c2 == 0xBA)) {
                res += 'a'; i++; continue;
            }
            // ă ắ ằ ẳ ẵ ặ => a
            if (c == 0xC4 && (c2 == 0x83)) { res += 'a'; i++; continue; }
            // â ấ ầ ẩ ẫ ậ => a
            if (c == 0xC3 && c2 == 0xA2) { res += 'a'; i++; continue; }
            // é è ẻ ẽ ẹ => e
            if (c == 0xC3 && (c2 == 0xA9 || c2 == 0xA8)) { res += 'e'; i++; continue; }
            // ê ế ề ể ễ ệ => e
            if (c == 0xC3 && c2 == 0xAA) { res += 'e'; i++; continue; }
            // í ì ỉ ĩ ị => i
            if (c == 0xC3 && (c2 == 0xAD || c2 == 0xAC)) { res += 'i'; i++; continue; }
            // ó ò ỏ õ ọ => o
            if (c == 0xC3 && (c2 == 0xB3 || c2 == 0xB2 || c2 == 0xB5)) { res += 'o'; i++; continue; }
            // ô ố ồ ổ ỗ ộ => o
            if (c == 0xC3 && c2 == 0xB4) { res += 'o'; i++; continue; }
            // ơ ớ ờ ở ỡ ợ => o
            if (c == 0xC6 && c2 == 0xA1) { res += 'o'; i++; continue; }
            // ú ù ủ ũ ụ => u
            if (c == 0xC3 && (c2 == 0xBA || c2 == 0xB9)) { res += 'u'; i++; continue; }
            // ư ứ ừ ử ữ ự => u
            if (c == 0xC6 && c2 == 0xB0) { res += 'u'; i++; continue; }
            // ý ỳ ỷ ỹ ỵ => y
            if (c == 0xC3 && c2 == 0xBD) { res += 'y'; i++; continue; }
            // đ => d
            if (c == 0xC4 && c2 == 0x91) { res += 'd'; i++; continue; }

            // Nếu là ký tự UTF-8 nhiều byte khác thì bỏ qua
            if (c >= 0xC0) { i++; continue; }
        }

        res += lower_c;
    }
    return res;
}

// Hàm chuẩn hóa tên: viết hoa chữ đầu mỗi từ
string chuanHoaTen(string s) {
    // Xóa khoảng trắng đầu cuối
    s.erase(0, s.find_first_not_of(" \t"));
    s.erase(s.find_last_not_of(" \t") + 1);

    bool dauTu = true;
    for (int i = 0; i < (int)s.size(); i++) {
        if (isspace(s[i])) {
            dauTu = true;
        } else if (dauTu) {
            s[i] = toupper(s[i]);
            dauTu = false;
        } else {
            s[i] = tolower(s[i]);
        }
    }
    return s;
}

// Hàm kiểm tra và chuẩn hóa số điện thoại Việt Nam
bool chuanHoaSDT(string &sdt) {
    // Xóa khoảng trắng
    sdt.erase(remove(sdt.begin(), sdt.end(), ' '), sdt.end());

    // Đổi +84 thành 0
    if (sdt.size() >= 3 && sdt.substr(0, 3) == "+84") {
        sdt = "0" + sdt.substr(3);
    }

    // Chỉ giữ lại chữ số
    string clean = "";
    for (char c : sdt) {
        if (isdigit(c)) clean += c;
    }
    sdt = clean;

    // Kiểm tra độ dài và đầu số
    if (sdt.length() != 10 || sdt[0] != '0') return false;
    string dau = sdt.substr(0, 2);
    if (dau != "03" && dau != "05" && dau != "07" && dau != "08" && dau != "09") return false;

    return true;
}
