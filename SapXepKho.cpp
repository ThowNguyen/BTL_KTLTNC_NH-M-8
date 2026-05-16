// =====================================================================
// PHẦN 20: SẮP XẾP KHO
// (Chuyển từ menuTraCuu vào submenu của xemToanBoKhoHang)
// =====================================================================

void CuaHang::sapXepKho() {
    cout << "\n--- SAP XEP KHO HANG ---\n";
    cout << "Chon tieu chi sap xep:\n";
    cout << "1. Ngay het han\n";
    cout << "2. Gia nhap\n";
    cout << "3. So luong\n";
    cout << "4. Ten hang\n";
    cout << "Chon: ";
    int tieuChi;
    cin >> tieuChi;

    if (tieuChi < 1 || tieuChi > 4) {
        cout << "Tieu chi khong hop le!\n";
        return;
    }

    cout << "Sap xep:\n";
    cout << "1. Tang dan\n";
    cout << "2. Giam dan\n";
    cout << "Chon: ";
    int huong;
    cin >> huong;

    bool tangDan = (huong == 1);

    ds.sapXep(tieuChi, tangDan);

    cout << "\nDa sap xep kho hang thanh cong!\n";
    cout << "\n=== KHO HANG SAU KHI SAP XEP ===\n";
    ds.xuat();
}

