// =====================================================================
// PHẦN 15: MENU QUẢN LÝ KHO
// Chỉ gồm 3 mục chính
// =====================================================================

void CuaHang::menuQuanLyKho() {
    // Hiển thị cảnh báo tự động khi vào kho
    hienThiCanhBaoKho();

    int luaChon;
    do {
        cout << "\n--- QUAN LY KHO HANG ---\n";
        cout << "1. Nhap hang vao kho\n";
        cout << "2. Sua thong tin hang\n";
        cout << "3. Xem toan bo kho hang\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "Chon: ";
        cin >> luaChon;

        if      (luaChon == 1) nhapHangVaoKho();
        else if (luaChon == 2) suaVaXoaHang();
        else if (luaChon == 3) xemToanBoKhoHang();
        else if (luaChon == 0) return;
        else cout << "Lua chon khong hop le!\n";

    } while (true);
}
