// =====================================================================
// PHẦN 9: DANH SÁCH HÀNG HÓA CHUYÊN BIỆT
// =====================================================================

class DanhSachHangHoa {
private:
    Node<HangHoa>* head;
    Node<HangHoa>* tail;

public:
    DanhSachHangHoa() : head(nullptr), tail(nullptr) {}

    ~DanhSachHangHoa() {
        Node<HangHoa>* t = head;
        while (t != nullptr) {
            Node<HangHoa>* x = t;
            t = t->next;
            delete x->data;
            delete x;
        }
    }

    Node<HangHoa>* getHead() { return head; }

    // Thêm vào cuối
    void themCuoi(HangHoa* x) {
        Node<HangHoa>* p = new Node<HangHoa>(x);
        if (tail == nullptr) {
            head = tail = p;
        } else {
            tail->next = p;
            tail = p;
        }
    }

    // Tìm theo mã (trả về mặt hàng đầu tiên tìm được)
    HangHoa* timTheoMa(const string& ma) {
        Node<HangHoa>* t = head;
        while (t != nullptr) {
            if (t->data->getMa() == ma) return t->data;
            t = t->next;
        }
        return nullptr;
    }

    bool coTrungMa(const string& ma) {
        return timTheoMa(ma) != nullptr;
    }

    // Xóa 1 node theo mã (chỉ xóa node đầu tiên tìm được)
    bool xoaMotNodeTheoMa(const string& ma) {
        Node<HangHoa>* t    = head;
        Node<HangHoa>* prev = nullptr;
        while (t != nullptr) {
            if (t->data->getMa() == ma) {
                if (prev == nullptr) head = t->next;
                else prev->next = t->next;
                if (t == tail) tail = prev;
                delete t->data;
                delete t;
                return true;
            }
            prev = t;
            t = t->next;
        }
        return false;
    }

    // Xóa TẤT CẢ node có cùng mã (dùng cho chức năng xóa vĩnh viễn)
    int xoaTatCaTheoMa(const string& ma) {
        int soLuongXoa = 0;
        while (xoaMotNodeTheoMa(ma)) {
            soLuongXoa++;
        }
        return soLuongXoa;
    }

    // Tìm kiếm theo từ khóa, trả về danh sách con trỏ
    vector<HangHoa*> timKiem(const string& tuKhoa) {
        vector<HangHoa*> ketQua;
        string keyword = boDauVaInThuong(tuKhoa);

        Node<HangHoa>* t = head;
        while (t != nullptr) {
            string tenHang = boDauVaInThuong(t->data->getTen());
            // Nếu từ khóa rỗng thì trả về tất cả, ngược lại tìm khớp
            if (tuKhoa.empty() || tenHang.find(keyword) != string::npos) {
                ketQua.push_back(t->data);
            }
            t = t->next;
        }
        return ketQua;
    }

    // Cho người dùng tìm kiếm rồi chọn 1 mặt hàng (dùng trong bán hàng)
    HangHoa* chonHangTuTimKiem(const string& thongBao = "\nNhap tu khoa tim kiem: ") {
        cout << thongBao;
        string tuKhoa;
        cin.ignore(1000, '\n');
        getline(cin, tuKhoa);

        vector<HangHoa*> ketQua = timKiem(tuKhoa);
        if (ketQua.empty()) {
            cout << "Khong tim thay hang hoa nao!\n";
            return nullptr;
        }

        cout << "\n=== KET QUA TIM KIEM ===\n";
        for (int i = 0; i < (int)ketQua.size(); i++) {
            cout << i + 1 << ". ";
            ketQua[i]->xuat();
        }

        int chon;
        cout << "Chon so thu tu (0 de huy): ";
        cin >> chon;
        if (chon <= 0 || chon > (int)ketQua.size()) return nullptr;

        return ketQua[chon - 1];
    }

    // Tính tổng vốn tồn kho
    float tinhTongVon() {
        float tong = 0.0f;
        Node<HangHoa>* t = head;
        while (t != nullptr) {
            tong += t->data->getGiaNhap() * t->data->getSoLuong();
            t = t->next;
        }
        return tong;
    }

    // Lọc và hiển thị theo loại hàng
    void locTheoLoai(const string& loaiInput) {
        string loai = boDauVaInThuong(loaiInput);
        loai.erase(remove(loai.begin(), loai.end(), ' '), loai.end());

        Node<HangHoa>* t = head;
        bool coHang = false;
        while (t != nullptr) {
            string loaiHang = boDauVaInThuong(t->data->getLoai());
            loaiHang.erase(remove(loaiHang.begin(), loaiHang.end(), ' '), loaiHang.end());
            if (loaiHang.find(loai) != string::npos) {
                t->data->xuat();
                coHang = true;
            }
            t = t->next;
        }
        if (!coHang) cout << "Khong co hang hoa loai " << loaiInput << "!\n";
    }

    // Sắp xếp bằng insertion sort
    void sapXep(int tieuChi, bool tangDan) {
        if (head == nullptr || head->next == nullptr) return;

        // Hàm so sánh dựa trên tiêu chí
        auto soSanh = [&](HangHoa* a, HangHoa* b) -> bool {
            bool ketQua = false;
            if (tieuChi == 1) {
                ketQua = a->tinhSoNgayConLai() < b->tinhSoNgayConLai();
            } else if (tieuChi == 2) {
                ketQua = a->getGiaNhap() < b->getGiaNhap();
            } else if (tieuChi == 3) {
                ketQua = a->getSoLuong() < b->getSoLuong();
            } else {
                ketQua = a->getTen() < b->getTen();
            }
            // Nếu giảm dần thì đảo ngược kết quả so sánh
            return tangDan ? ketQua : !ketQua;
        };

        // Insertion sort trên danh sách liên kết
        Node<HangHoa>* sorted = nullptr;
        Node<HangHoa>* current = head;
        while (current != nullptr) {
            Node<HangHoa>* next = current->next;
            if (sorted == nullptr || soSanh(current->data, sorted->data)) {
                current->next = sorted;
                sorted = current;
            } else {
                Node<HangHoa>* temp = sorted;
                while (temp->next != nullptr && !soSanh(current->data, temp->next->data)) {
                    temp = temp->next;
                }
                current->next = temp->next;
                temp->next = current;
            }
            current = next;
        }

        // Cập nhật lại head và tail
        head = sorted;
        tail = head;
        while (tail != nullptr && tail->next != nullptr) tail = tail->next;
    }

    // In tất cả hàng hóa
    void xuat() {
        Node<HangHoa>* t = head;
        int stt = 1;
        while (t != nullptr) {
            cout << stt++ << ". ";
            t->data->xuat();
            t = t->next;
        }
        if (head == nullptr) cout << "Kho hang dang trong!\n";
    }
};
