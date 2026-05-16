// =====================================================================
// PHẦN 22: MENU TRA CỨU THÔNG TIN HÀNG (menu chính mục 3)
// Gồm 2 chức năng: tra cứu hàng hóa và lọc theo loại
// =====================================================================

void CuaHang::menuTraCuuThongTin() {
    int luaChon;
    do {
        cout << "\n--- TRA CUU THONG TIN HANG ---\n";
        cout << "1. Tra cuu hang hoa\n";
        cout << "2. Loc hang theo loai\n";
        cout << "0. Quay lai menu chinh\n";
        cout << "Chon chuc nang: ";
        cin >> luaChon;

        if      (luaChon == 1) traCuuHangHoa();
        else if (luaChon == 2) locHangTheoLoai();
        else if (luaChon == 0) return;
        else cout << "Lua chon khong hop le!\n";

    } while (true);
}


