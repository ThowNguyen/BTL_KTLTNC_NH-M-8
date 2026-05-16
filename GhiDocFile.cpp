// =====================================================================
// PHẦN 12: TRIỂN KHAI ĐỌC / GHI FILE
// =====================================================================

void CuaHang::docFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Khong tim thay file " << filename << ". Se tao file moi khi luu.\n";
        return;
    }

    string loai;
    while (getline(file, loai)) {
        // Bỏ qua dòng trống hoặc dòng không hợp lệ
        if (loai.empty() || loai == "\r") continue;

        // Kiểm tra xem có phải tên loại hàng hợp lệ không
        bool hopLe = (loai == "ThucPhamCheBien" || loai == "DoUong"      ||
                      loai == "DoGiaDung"        || loai == "MyPham"      ||
                      loai == "HangTuoiSong"     || loai == "TraiCay");
        if (!hopLe) continue;

        // Tạo đối tượng tương ứng
        HangHoa* h = nullptr;
        if      (loai == "ThucPhamCheBien") h = new ThucPhamCheBien();
        else if (loai == "DoUong")          h = new DoUong();
        else if (loai == "DoGiaDung")       h = new DoGiaDung();
        else if (loai == "MyPham")          h = new MyPham();
        else if (loai == "HangTuoiSong")    h = new HangTuoiSong();
        else if (loai == "TraiCay")         h = new TraiCay();

        if (h != nullptr) {
            h->docTuFile(file);
            ds.themCuoi(h);
        }
    }
    file.close();
}

void CuaHang::ghiFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) return;

    Node<HangHoa>* t = ds.getHead();
    while (t != nullptr) {
        t->data->ghiVaoFile(file);
        t = t->next;
    }
    file.close();
}

void CuaHang::docKhachHang(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        KhachHang* kh = new KhachHang();
        stringstream ss(line);
        string temp;

        getline(ss, temp, '|'); kh->setMa(temp);
        getline(ss, temp, '|'); kh->setTen(temp);
        getline(ss, temp, '|'); kh->setSDT(temp);
        getline(ss, temp, '|'); kh->setTongTien(stoll(temp));
        getline(ss, temp, '|'); kh->setDiem(stoi(temp));
        getline(ss, temp, '|'); kh->setLoaiThanhVien(temp);

        dsKhachHang.themCuoi(kh);
    }
    file.close();
}

void CuaHang::ghiKhachHang(const string& filename) {
    ofstream file(filename);
    Node<KhachHang>* t = dsKhachHang.getHead();
    while (t != nullptr) {
        file << t->data->getMa()           << "|"
             << t->data->getTen()           << "|"
             << t->data->getSDT()           << "|"
             << t->data->getTongTien()      << "|"
             << t->data->getDiem()          << "|"
             << t->data->getLoaiThanhVien() << "\n";
        t = t->next;
    }
    file.close();
}

void CuaHang::docNhanVien(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Khong tim thay file " << filename << "!\n";
        return;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        NhanVien* nv = new NhanVien();
        stringstream ss(line);
        string temp;

        getline(ss, temp, '|'); nv->setMa(temp);
        getline(ss, temp, '|'); nv->setTen(temp);
        getline(ss, temp, '|'); nv->setSDT(temp);
        getline(ss, temp, '|'); nv->setChucVu(temp);
        getline(ss, temp, '|'); nv->setMatKhau(temp);

        dsNhanVien.themCuoi(nv);
    }
    file.close();
}

void CuaHang::ghiNhanVien(const string& filename) {
    ofstream file(filename);
    Node<NhanVien>* t = dsNhanVien.getHead();
    while (t != nullptr) {
        file << t->data->getMa()     << "|"
             << t->data->getTen()    << "|"
             << t->data->getSDT()    << "|"
             << t->data->getChucVu() << "\n";
        t = t->next;
    }
    file.close();
}
