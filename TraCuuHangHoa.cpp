// =====================================================================
// PHẦN 23: TRA CỨU HÀNG HÓA
// Luồng: nhập từ khóa → chọn tiêu chí → tăng/giảm → hiển thị kết quả
// =====================================================================

void CuaHang::traCuuHangHoa() {
    cout << "\n--- TRA CUU HANG HOA ---\n";

    // Bước 1: Nhập từ khóa
    cin.ignore(1000, '\n');
    cout << "Nhap tu khoa tim kiem: ";
    string tuKhoa;
    getline(cin, tuKhoa);

    // Tìm kiếm
    vector<HangHoa*> ketQua = ds.timKiem(tuKhoa);
    if (ketQua.empty()) {
        cout << "Khong tim thay hang hoa nao phu hop voi tu khoa [" << tuKhoa << "]\n";
        return;
    }

    // Bước 2: Chọn tiêu chí sắp xếp
    cout << "\nChon tieu chi sap xep:\n";
    cout << "1. Ngay het han\n";
    cout << "2. Gia nhap\n";
    cout << "3. So luong\n";
    cout << "Chon (0 de khong sap xep): ";
    int tieuChi;
    cin >> tieuChi;

    if (tieuChi >= 1 && tieuChi <= 3) {
        // Bước 3: Chọn hướng sắp xếp
        cout << "\nSap xep:\n";
        cout << "1. Tang dan\n";
        cout << "2. Giam dan\n";
        cout << "Chon: ";
        int huong;
        cin >> huong;

        // Sắp xếp vector kết quả (không ảnh hưởng danh sách chính)
        sort(ketQua.begin(), ketQua.end(), [&](HangHoa* a, HangHoa* b) {
            if (tieuChi == 1) return a->tinhSoNgayConLai() < b->tinhSoNgayConLai();
            if (tieuChi == 2) return a->getGiaNhap()       < b->getGiaNhap();
            if (tieuChi == 3) return a->getSoLuong()       < b->getSoLuong();
            return false;
        });

        // Nếu giảm dần thì đảo ngược
        if (huong == 2) {
            reverse(ketQua.begin(), ketQua.end());
        }
    }

    // Bước 4: Hiển thị kết quả
    cout << "\n=== KET QUA TIM KIEM (DA SAP XEP) ===\n";
    cout << string(60, '-') << "\n";
    for (int i = 0; i < (int)ketQua.size(); i++) {
        cout << i + 1 << ". ";
        ketQua[i]->xuat();
    }
    cout << string(60, '-') << "\n";
    cout << "Tong cong: " << ketQua.size() << " mat hang.\n";
}

