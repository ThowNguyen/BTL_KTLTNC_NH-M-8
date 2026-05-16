// =====================================================================
// PHẦN 17: SỬA THÔNG TIN VÀ XÓA HÀNG (Admin only)
// Gồm 2 lựa chọn: sửa thông tin HOẶC xóa vĩnh viễn tất cả lô cùng mã
// =====================================================================

void CuaHang::suaVaXoaHang() {
    // Kiểm tra quyền Admin trước khi cho vào
    if (nhanVienHienTai == nullptr || nhanVienHienTai->getChucVu() != "Admin") {
        cout << "\n[THONG BAO] Ban khong co quyen thuc hien chuc nang nay.\n";
        cout << "Chi nhan vien co chuc vu Admin moi duoc phep sua/xoa hang hoa.\n";
        return;
    }

    int luaChon;
    do {
        cout << "\n--- SUA THONG TIN HANG (Admin) ---\n";
        cout << "1. Sua thong tin mot lo hang\n";
        cout << "2. Xoa vinh vien toan bo hang theo ma\n";
        cout << "0. Quay lai\n";
        cout << "Chon: ";
        cin >> luaChon;

        if (luaChon == 0) return;

        // ------------------------------------------
        // OPTION 1: Sửa thông tin 1 lô hàng
        // ------------------------------------------
        if (luaChon == 1) {
            cout << "Nhap ma hang can sua: ";
            string ma;
            cin >> ma;

            HangHoa* h = ds.timTheoMa(ma);
            if (h == nullptr) {
                cout << "Khong tim thay hang voi ma [" << ma << "]!\n";
                continue;
            }

            cout << "\nThong tin hien tai:\n";
            h->xuat();
            cout << "\nNhap thong tin moi:\n";
            h->nhapKhongMa();
            cout << "Da sua thong tin hang hoa thanh cong!\n";
        }

        // ------------------------------------------
        // OPTION 2: Xóa vĩnh viễn tất cả lô cùng mã
        // ------------------------------------------
        else if (luaChon == 2) {
            cout << "Nhap ma hang can xoa vinh vien: ";
            string ma;
            cin >> ma;

            // Kiểm tra mã có tồn tại không
            HangHoa* h = ds.timTheoMa(ma);
            if (h == nullptr) {
                cout << "Khong tim thay hang voi ma [" << ma << "]!\n";
                continue;
            }

            // Hiển thị cảnh báo
            cout << "\n[CANH BAO] Thao tac nay se XOA VINH VIEN tat ca lo hang co ma ["
                 << ma << "]!\n";
            cout << "Ten hang: " << h->getTen() << "\n";
            cout << "KHONG THE PHUC HOI sau khi xoa!\n";

            // Xác nhận
            char xacNhan;
            cout << "Ban chac chan muon xoa? (Y/N): ";
            cin >> xacNhan;
            if (toupper(xacNhan) != 'Y') {
                cout << "Da huy thao tac.\n";
                continue;
            }

            // Xóa tất cả lô có cùng mã
            int soLuongXoa = ds.xoaTatCaTheoMa(ma);
            cout << "Da xoa " << soLuongXoa << " lo hang co ma ["
                 << ma << "] khoi kho thanh cong!\n";
        }

        else {
            cout << "Lua chon khong hop le!\n";
        }

    } while (true);
}

