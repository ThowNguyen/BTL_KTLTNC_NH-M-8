// =====================================================================
// PHẦN 8: NODE VÀ DANH SÁCH LIÊN KẾT
// =====================================================================

// Node dùng chung cho mọi kiểu dữ liệu
template <class T>
class Node {
public:
    T* data;
    Node<T>* next;

    Node(T* x) : data(x), next(nullptr) {}
};

// Danh sách liên kết đơn dùng chung cho KhachHang và NhanVien
template <class T>
class DanhSach {
private:
    Node<T>* head;
    Node<T>* tail;
    bool isOwner; // Nếu true thì destructor sẽ giải phóng bộ nhớ data

public:
    DanhSach(bool owner = true) : head(nullptr), tail(nullptr), isOwner(owner) {}

    ~DanhSach() {
        if (!isOwner) return;
        Node<T>* t = head;
        while (t != nullptr) {
            Node<T>* x = t;
            t = t->next;
            delete x->data;
            delete x;
        }
    }

    Node<T>* getHead() { return head; }

    // Thêm vào cuối danh sách
    void themCuoi(T* x) {
        Node<T>* p = new Node<T>(x);
        if (tail == nullptr) {
            head = tail = p;
        } else {
            tail->next = p;
            tail = p;
        }
    }

    // Tìm theo mã
    T* timTheoMa(const string& ma) {
        Node<T>* t = head;
        while (t != nullptr) {
            if (t->data->getMa() == ma) return t->data;
            t = t->next;
        }
        return nullptr;
    }

    bool coTrungMa(const string& ma) {
        return timTheoMa(ma) != nullptr;
    }

    // Xóa theo mã
    bool xoaTheoMa(const string& ma) {
        Node<T>* t    = head;
        Node<T>* prev = nullptr;
        while (t != nullptr) {
            if (t->data->getMa() == ma) {
                if (prev == nullptr) head = t->next;
                else prev->next = t->next;
                if (t == tail) tail = prev;
                if (isOwner) delete t->data;
                delete t;
                return true;
            }
            prev = t;
            t = t->next;
        }
        return false;
    }

    // In tất cả
    void xuat() {
        Node<T>* t = head;
        int stt = 1;
        while (t != nullptr) {
            cout << stt++ << ". ";
            t->data->xuat();
            t = t->next;
        }
        if (head == nullptr) cout << "Danh sach rong!\n";
    }
};


