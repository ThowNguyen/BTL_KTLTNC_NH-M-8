// =====================================================================
// PHẦN 19: HỦY HÀNG ĐÃ HẾT HẠN
// Chỉ hiển thị hàng có số ngày còn lại <= 0
// Xác nhận 2 lần trước khi xóa
// =====================================================================

void CuaHang::huyHangHetHan() {
    cout << "\n--- HUY HANG DA HET HAN ---\n";

    // Lọc ra danh sách hàng đã hết hạn
    vector<HangHoa*> hangHetHan;
    Node<HangHoa>* t = ds.getHead();
    while (t != nullptr) {
        if (t->data->tinhSoNgayConLai() <= 0) {
            hangHetHan.push_back(t->data);
        }
        t = t->next;
    }

    // Thông báo nếu không có hàng hết hạn
    if (hangHetHan.empty()) {
        cout << "Hien khong co mat hang nao can huy.\n";
        cout << "(Tat ca hang deu con trong han)\n";
        return;
    }

    // Hiển thị danh sách hàng hết hạn
    cout << "Danh sach hang da het han:\n";
    cout << string(60, '-') << "\n";
    for (int i = 0; i < (int)hangHetHan.size(); i++) {
        int ngayCon = hangHetHan[i]->tinhSoNgayConLai();
        cout << i + 1 << ". ";
        hangHetHan[i]->xuat();
        cout << "     => Da qua han: " << (-ngayCon) << " ngay\n";
    }
    cout << string(60, '-') << "\n";

    // Cho chọn mặt hàng cần hủy
    int chon;
    cout << "Chon so thu tu can huy (0 de thoat): ";
    cin >> chon;

    if (chon <= 0 || chon > (int)hangHetHan.size()) {
        cout << "Da huy thao tac.\n";
        return;
    }

    HangHoa* hangChon = hangHetHan[chon - 1];

    cout << "\nBan chac chan muon huy mat hang sau?\n";
    hangChon->xuat();

    // Xác nhận lần 1
    char xn1;
    cout << "Xac nhan lan 1 (Y/N): ";
    cin >> xn1;
    if (toupper(xn1) != 'Y') {
        cout << "Da huy thao tac.\n";
        return;
    }

    // Xác nhận lần 2
    char xn2;
    cout << "Xac nhan lan 2 - Thao tac nay KHONG THE HOAN TAC (Y/N): ";
    cin >> xn2;
    if (toupper(xn2) != 'Y') {
        cout << "Da huy thao tac.\n";
        return;
    }

    // Lấy thông tin trước khi xóa
    time_t now = time(nullptr);
    string thoiGian = ctime(&now);
    if (!thoiGian.empty() && thoiGian.back() == '\n') thoiGian.pop_back();

    HangHuy hh;
    hh.maHang    = hangChon->getMa();
    hh.tenHang   = hangChon->getTen();
    hh.keHang    = hangChon->getKeHang();
    hh.ngayHetHan= hangChon->getNgayHetHan();
    hh.soLuong   = (float)hangChon->getSoLuong();
    hh.lyDo      = "Hang da het han";
    hh.thoiGianHuy = thoiGian;
    hh.loaiHang  = hangChon->getLoai();

    // Ghi lịch sử hủy vào file
    DanhSachHangHuy dsHuy;
    dsHuy.them(hh);
    dsHuy.ghiFile("lichsuhuy.txt");

    // Lưu mã rồi xóa (xóa sẽ giải phóng bộ nhớ con trỏ hangChon)
    string maHang = hangChon->getMa();
    ds.xoaMotNodeTheoMa(maHang);

    cout << "Da huy hang [" << hh.tenHang << "] thanh cong!\n";
    cout << "Da luu vao file lichsuhuy.txt\n";
}

