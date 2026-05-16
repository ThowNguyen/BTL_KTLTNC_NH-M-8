// =====================================================================
// PHẦN 21: TẠO DANH SÁCH THEO DÕI HẾT HẠN
// Lọc hàng < 20 ngày, sắp xếp gần hết nhất lên trên,
// cho chọn nhiều mặt hàng, lưu vào danhsachtheodoi.txt
// =====================================================================

void CuaHang::taoDanhSachTheoDoiHetHan() {
    cout << "\n--- TAO DANH SACH THEO DOI HET HAN ---\n";

    // Lọc hàng có 0 <= ngày còn lại < 20
    vector<HangHoa*> sapHetHan;
    Node<HangHoa>* t = ds.getHead();
    while (t != nullptr) {
        int ngayCon = t->data->tinhSoNgayConLai();
        if (ngayCon >= 0 && ngayCon < 20) {
            sapHetHan.push_back(t->data);
        }
        t = t->next;
    }

    if (sapHetHan.empty()) {
        cout << "Hien khong co mat hang nao sap het han (trong vong 20 ngay).\n";
        return;
    }

    // Sắp xếp: hàng sắp hết nhất lên đầu
    sort(sapHetHan.begin(), sapHetHan.end(), [](HangHoa* a, HangHoa* b) {
        return a->tinhSoNgayConLai() < b->tinhSoNgayConLai();
    });

    // Hiển thị danh sách
    cout << "Hang sap het han (duoi 20 ngay):\n";
    cout << string(65, '-') << "\n";
    cout << left
         << setw(5)  << "STT"
         << setw(10) << "Ma hang"
         << setw(30) << "Ten hang"
         << setw(14) << "Ngay het han"
         << "Con lai\n";
    cout << string(65, '-') << "\n";

    for (int i = 0; i < (int)sapHetHan.size(); i++) {
        int ngayCon = sapHetHan[i]->tinhSoNgayConLai();
        cout << left
             << setw(5)  << (i + 1)
             << setw(10) << sapHetHan[i]->getMa()
             << setw(30) << sapHetHan[i]->getTen()
             << setw(14) << sapHetHan[i]->getNgayHetHan()
             << ngayCon << " ngay\n";
    }
    cout << string(65, '-') << "\n";

    // Cho chọn nhiều mặt hàng
    vector<int> danhSachChon;
    cout << "\nNhap so thu tu muon them vao danh sach theo doi:\n";
    cout << "(Nhap 0 de ket thuc)\n";

    while (true) {
        cout << "Nhap so: ";
        int chon;
        cin >> chon;

        if (chon == 0) break;

        if (chon < 1 || chon > (int)sapHetHan.size()) {
            cout << "So khong hop le! Vui long nhap lai.\n";
            continue;
        }

        // Kiểm tra đã chọn chưa
        bool daChon = false;
        for (int idx : danhSachChon) {
            if (idx == chon - 1) { daChon = true; break; }
        }
        if (daChon) {
            cout << "Mat hang nay da duoc chon roi!\n";
            continue;
        }

        danhSachChon.push_back(chon - 1);
        cout << "  => Da them: " << sapHetHan[chon - 1]->getTen() << "\n";
    }

    if (danhSachChon.empty()) {
        cout << "Khong co mat hang nao duoc chon. Thoat.\n";
        return;
    }

    // Ghi vào file danhsachtheodoi.txt
    ofstream f("danhsachtheodoi.txt");
    if (!f.is_open()) {
        cout << "Loi: Khong the tao file danhsachtheodoi.txt!\n";
        return;
    }

    time_t now = time(nullptr);
    string thoiGian = ctime(&now);
    if (!thoiGian.empty() && thoiGian.back() == '\n') thoiGian.pop_back();

    f << "=== DANH SACH THEO DOI HET HAN ===\n";
    f << "Ngay tao: " << thoiGian << "\n";
    f << string(50, '-') << "\n";
    f << left
      << setw(10) << "Ma hang"
      << setw(30) << "Ten hang"
      << "Ngay het han\n";
    f << string(50, '-') << "\n";

    for (int idx : danhSachChon) {
        f << left
          << setw(10) << sapHetHan[idx]->getMa()
          << setw(30) << sapHetHan[idx]->getTen()
          << sapHetHan[idx]->getNgayHetHan() << "\n";
    }

    f.close();

    cout << "\nDa tao danh sach theo doi thanh cong!\n";
    cout << "File luu tai: danhsachtheodoi.txt\n";
    cout << "Tong so mat hang: " << danhSachChon.size() << "\n";
}
