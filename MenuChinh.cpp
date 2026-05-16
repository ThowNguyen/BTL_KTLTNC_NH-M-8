// =====================================================================
// PHẦN 14: MENU CHÍNH
// =====================================================================

void CuaHang::menuChinh() {
    int luaChon;
    do {
        cout << "\n============ HE THONG QUAN LY SIEU THI ============\n";
        cout << "1. Quan ly kho hang\n";
        cout << "2. Ban hang\n";
        cout << "3. Tra cuu thong tin hang\n";
        cout << "4. Bao cao & Thong ke\n";
        cout << "0. Thoat chuong trinh\n";
        cout << "====================================================\n";
        cout << "Chon chuc nang: ";
        cin >> luaChon;

        switch (luaChon) {
            case 1: menuQuanLyKho();       break;
            case 2: banHang();             break;
            case 3: menuTraCuuThongTin();  break;
            case 4: menuBaoCao();          break;
            case 0:
                cout << "Tam biet! Hen gap lai.\n";
                return;
            default:
                cout << "Lua chon khong hop le! Vui long chon tu 0 den 4.\n";
        }
    } while (true);
}

