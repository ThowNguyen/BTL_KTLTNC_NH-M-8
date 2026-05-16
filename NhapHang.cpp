/ =====================================================================
// PHẦN 16: NHẬP HÀNG VÀO KHO
// Tìm hàng (theo mã hoặc tìm kiếm) rồi nhập lô mới cùng mã
// =====================================================================

void CuaHang::nhapHangVaoKho() {
    int luaChon;
    do {
        cout << "\n--- NHAP HANG VAO KHO ---\n";
        cout << "1. Nhap truc tiep theo ma\n";
        cout << "2. Tim kiem\n";
        cout << "0. Quay lai\n";
        cout << "Chon: ";
        cin >> luaChon;

        if (luaChon == 0) return;

        HangHoa* hangChon = nullptr;

        if (luaChon == 1) {
            // Nhập thẳng mã hàng
            string ma;
            cout << "Nhap ma hang: ";
            cin >> ma;
            hangChon = ds.timTheoMa(ma);
            if (hangChon == nullptr) {
                cout << "Khong tim thay hang voi ma [" << ma << "] trong kho!\n";
                continue;
            }
            cout << "Tim thay: ";
            hangChon->xuat();
        }
        else if (luaChon == 2) {
            // Tìm kiếm theo từ khóa
            hangChon = ds.chonHangTuTimKiem("\nNhap tu khoa tim kiem: ");
            if (hangChon == nullptr) continue;
        }
        else {
            cout << "Lua chon khong hop le!\n";
            continue;
        }

        // Nhập thông tin lô hàng mới
        cout << "\n--- NHAP LO HANG MOI ---\n";
        cout << "Ma hang : " << hangChon->getMa()  << "\n";
        cout << "Ten hang: " << hangChon->getTen()  << "\n";

        // Giá trị mặc định lấy từ hàng đã chọn
        string dvtMoi  = hangChon->getDefaultDvt();
        string keMoi   = hangChon->getDefaultKeHang();

        cin.ignore(1000, '\n');

        cout << "Don vi tinh (mac dinh: " << dvtMoi << "): ";
        string dvtNhap;
        getline(cin, dvtNhap);
        if (!dvtNhap.empty()) dvtMoi = dvtNhap;

        cout << "Ke hang A-F (mac dinh: " << keMoi << "): ";
        string keNhap;
        getline(cin, keNhap);
        if (!keNhap.empty()) keMoi = keNhap;

        // Nhập ngày hết hạn của lô mới
        string ngayMoi = nhapNgayHopLe();

        // Nhập giá và số lượng
        float giaNhapMoi, giaNiemYetMoi;
        int   soLuongMoi;

        do { cout << "Gia nhap   : "; cin >> giaNhapMoi;    } while (giaNhapMoi <= 0);
        do { cout << "Gia niem yet: "; cin >> giaNiemYetMoi; } while (giaNiemYetMoi <= 0);
        do { cout << "So luong   : "; cin >> soLuongMoi;    } while (soLuongMoi < 0);

        // Tạo node mới cùng loại với hàng đã chọn
        HangHoa* loMoi = nullptr;
        string loaiHang = hangChon->getLoai();
        if      (loaiHang == "ThucPhamCheBien") loMoi = new ThucPhamCheBien();
        else if (loaiHang == "DoUong")          loMoi = new DoUong();
        else if (loaiHang == "DoGiaDung")       loMoi = new DoGiaDung();
        else if (loaiHang == "MyPham")          loMoi = new MyPham();
        else if (loaiHang == "HangTuoiSong")    loMoi = new HangTuoiSong();
        else if (loaiHang == "TraiCay")         loMoi = new TraiCay();

        if (loMoi != nullptr) {
            loMoi->setMa(hangChon->getMa());
            loMoi->setTen(hangChon->getTen());
            loMoi->setDvt(dvtMoi);
            loMoi->setKeHang(keMoi);
            loMoi->setNgayHetHan(ngayMoi);
            loMoi->setGiaNhap(giaNhapMoi);
            loMoi->setGiaNiemYet(giaNiemYetMoi);
            loMoi->setSoLuong(soLuongMoi);

            ds.themCuoi(loMoi);
            cout << "\nDa nhap lo hang moi thanh cong! (Ma: "
                 << hangChon->getMa() << ")\n";
        }

    } while (true);
}

