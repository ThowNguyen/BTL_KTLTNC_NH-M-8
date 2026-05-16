// =====================================================================
// PHẦN 24: LỌC HÀNG THEO LOẠI
// =====================================================================

void CuaHang::locHangTheoLoai() {
    cout << "\n--- LOC HANG THEO LOAI ---\n";
    cout << "1. Thuc pham che bien\n";
    cout << "2. Do uong\n";
    cout << "3. Do gia dung\n";
    cout << "4. My pham\n";
    cout << "5. Hang tuoi song\n";
    cout << "6. Trai cay\n";
    cout << "0. Xem tat ca\n";
    cout << "Chon: ";
    int luaChon;
    cin >> luaChon;

    string loai;
    switch (luaChon) {
        case 1: loai = "ThucPhamCheBien"; break;
        case 2: loai = "DoUong";          break;
        case 3: loai = "DoGiaDung";       break;
        case 4: loai = "MyPham";          break;
        case 5: loai = "HangTuoiSong";    break;
        case 6: loai = "TraiCay";         break;
        case 0:
            cout << "\n=== TOAN BO HANG HOA ===\n";
            ds.xuat();
            return;
        default:
            cout << "Lua chon khong hop le!\n";
            return;
    }

    cout << "\n=== HANG HOA LOAI: " << loai << " ===\n";
    ds.locTheoLoai(loai);
}
