// =====================================================================
// PHẦN 18: XEM TOÀN BỘ KHO HÀNG (THEO PHÂN LOẠI)
// Sau khi hiển thị sẽ hiện submenu: Hủy hàng / Sắp xếp / Tạo danh sách
// =====================================================================

void CuaHang::xemToanBoKhoHang() {
    // Danh sách 6 loại hàng theo thứ tự cố định
    vector<string> cacLoai = {
        "ThucPhamCheBien",
        "DoUong",
        "DoGiaDung",
        "MyPham",
        "HangTuoiSong",
        "TraiCay"
    };

    cout << "\n========== TOAN BO KHO HANG (THEO PHAN LOAI) ==========\n";

    bool coHangNao = false;

    for (const string& loai : cacLoai) {
        // Kiểm tra loại này có hàng không
        Node<HangHoa>* t = ds.getHead();
        bool coHangLoaiNay = false;
        while (t != nullptr) {
            if (t->data->getLoai() == loai) {
                coHangLoaiNay = true;
                break;
            }
            t = t->next;
        }
        if (!coHangLoaiNay) continue;

        // In tiêu đề nhóm
        cout << "\n[ " << loai << " ]\n";
        cout << string(60, '-') << "\n";

        // In từng mặt hàng trong nhóm
        t = ds.getHead();
        int stt = 1;
        while (t != nullptr) {
            if (t->data->getLoai() == loai) {
                cout << stt++ << ". ";
                t->data->xuat();
                coHangNao = true;
            }
            t = t->next;
        }
    }

    if (!coHangNao) {
        cout << "\nKho hang hien tai dang trong!\n";
    }

    cout << "\n========================================================\n";

    // Hiển thị submenu sau khi xem kho
    int luaChon;
    do {
        cout << "\n--- CHUC NANG KHAC ---\n";
        cout << "1. Huy hang (hang da het han)\n";
        cout << "2. Sap xep kho\n";
        cout << "3. Tao danh sach theo doi het han\n";
        cout << "0. Quay lai menu quan ly kho\n";
        cout << "Chon chuc nang: ";
        cin >> luaChon;

        if      (luaChon == 1) huyHangHetHan();
        else if (luaChon == 2) sapXepKho();
        else if (luaChon == 3) taoDanhSachTheoDoiHetHan();
        else if (luaChon == 0) return;
        else cout << "Lua chon khong hop le!\n";

    } while (true);
}

