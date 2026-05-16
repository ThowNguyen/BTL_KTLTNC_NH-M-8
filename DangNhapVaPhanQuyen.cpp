// =====================================================================
// PHẦN 13: ĐĂNG NHẬP VÀ PHÂN QUYỀN
// =====================================================================

bool CuaHang::dangNhap() {
    string ma, mk;
    int lanThu = 0;

    while (true) {
        cout << "\n=== DANG NHAP HE THONG ===\n";
        cout << "Ma nhan vien : "; cin >> ma;
        cout << "Mat khau     : "; cin >> mk;

        Node<NhanVien>* t = dsNhanVien.getHead();
        while (t != nullptr) {
            if (t->data->getMa() == ma && t->data->kiemTraMatKhau(mk)) {
                nhanVienHienTai = t->data;
                cout << "Dang nhap thanh cong! Xin chao "
                     << nhanVienHienTai->getTen() << ".\n";
                return true;
            }
            t = t->next;
        }

        lanThu++;
        cout << "Sai ma hoac mat khau! (Lan " << lanThu << ")\n";
    }
}

bool CuaHang::coQuyen(const string& quyen) {
    if (nhanVienHienTai == nullptr) return false;

    string cv = nhanVienHienTai->getChucVu();

    // Admin có tất cả quyền
    if (cv == "Admin") return true;

    // QuanLy có quyền quản lý kho
    if (cv == "QuanLy" && quyen != "BanHang") return true;

    // ThuNgan chỉ có quyền bán hàng
    if (cv == "ThuNgan" && quyen == "BanHang") return true;

    return false;
}
