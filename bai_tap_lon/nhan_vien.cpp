/*Bắt lỗi phần Nhân viên:
"Lương, hệ số lương, thưởng, phạt: luongCoBan <= 0 → không hợp lệ
heSoLuong <= 0 hoặc quá lớn (vd > 10) → sai thực tế
thuong < 0 → không hợp lệ
phat < 0 → không hợp lệ
Tràn số khi tính lương (giá trị quá lớn)
Nhập kiểu sai (nhập chữ → cin fail)
Tính lương sai công thức (quên thưởng/phạt)
Không làm tròn (float gây sai số)
Không kiểm tra logic: thưởng + phạt > lương cơ bản (bất thường)"

"Chức vụ, ca làm: Chức vụ rỗng
Nhập sai giá trị (vd: “abcxyz”)
Không thuộc danh sách cho phép (NV, QL, Admin…)
Ca làm rỗng
Ca làm không hợp lệ (Sáng/Chiều/Tối)
Viết sai format (vd: “sang”, “SANG”)
Có ký tự đặc biệt
Không đồng bộ với lương (QL mà lương thấp bất thường)"

"Số ngày làm: Âm (< 0)
Quá lớn (> 31 ngày/tháng)
Nhập chữ → lỗi
Không khớp với ngày nghỉ
Không reset khi nhập lại
Không cập nhật khi tính lương
Không kiểm soát max theo tháng (28–31)
Logic sai: làm + nghỉ > số ngày tháng"

"Số ngày nghỉ: Âm
Quá lớn (> số ngày tháng)
Không khớp với soNgayLam
Không tính vào phạt
Không kiểm tra tổng ngày
Nhập sai kiểu
Không reset dữ liệu
Không có giới hạn hợp lý"

"Giờ tăng ca: Âm
Quá lớn (vd > 24h/ngày * số ngày)
Không phải số
Không quy đổi thành tiền thưởng
Không giới hạn max (vd 100h/tháng)
Không đồng bộ với ngày làm
Không kiểm tra logic (nghỉ mà vẫn tăng ca?)
Không cập nhật vào lương"

"Logic tính lương: Không nhân hệ số lương
Không cộng thưởng
Không trừ phạt
Không tính tăng ca
Lương âm (do phạt lớn)
Sai thứ tự tính toán
Không validate trước khi tính
Không làm tròn
Không kiểm tra overflow"
"Công thức tính lương: Luong = luongCoBan * heSoLuong + thuong + (gioTangCa * tienTangCa) - phat;"
*/
#include <iostream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cmath>
using namespace std;

// ===== CLASS CHA =====
class ConNguoi {
protected:
    string ten;
    string diaChi;

public:
    virtual void nhap() {
        cin.ignore();
        cout << "Ten: ";
        getline(cin, ten);

        cout << "Dia chi: ";
        getline(cin, diaChi);
    }

    virtual void xuat() {
        cout << "Ten: " << ten << " | Dia chi: " << diaChi;
    }

    virtual ~ConNguoi() {}
};

// ===== HỖ TRỢ =====
string toLower(string s) {
    transform(s.begin(), s.end(), s.begin(), ::tolower);
    return s;
}

bool isValidText(const string& s) {
    if (s.empty()) return false;
    for (char c : s) {
        if (!isalpha(c) && c != ' ') return false;
    }
    return true;
}

// ===== CLASS NHÂN VIÊN =====
class NhanVien : public ConNguoi {
private:
    double luongCoBan, heSoLuong, thuong, phat;
    string chucVu, caLam;

    int soNgayLam, soNgayNghi, soNgayTrongThang;
    double gioTangCa;

public:
    NhanVien() {
        luongCoBan = heSoLuong = thuong = phat = 0;
        soNgayLam = soNgayNghi = 0;
        soNgayTrongThang = 30;
        gioTangCa = 0;
    }

    // ===== NHẬP =====
    void nhap() override {
        cout << "\n=== NHAP NHAN VIEN ===\n";
        ConNguoi::nhap();

        // ===== LƯƠNG =====
        do {
            cout << "Luong co ban (>0): ";
            cin >> luongCoBan;
            if (cin.fail() || luongCoBan <= 0) {
                cout << Loi!\n";
                cin.clear(); cin.ignore(1000, '\n');
            }
        } while (luongCoBan <= 0);

        do {
            cout << "He so luong (0 < hsl <= 10): ";
            cin >> heSoLuong;
            if (cin.fail() || heSoLuong <= 0 || heSoLuong > 10) {
                cout << " Loi!\n";
                cin.clear(); cin.ignore(1000, '\n');
            }
        } while (heSoLuong <= 0 || heSoLuong > 10);

        do {
            cout << "Thuong: ";
            cin >> thuong;
            if (cin.fail() || thuong < 0) {
                cout << " Loi!\n";
                cin.clear(); cin.ignore(1000, '\n');
            }
        } while (thuong < 0);

        do {
            cout << "Phat: ";
            cin >> phat;
            if (cin.fail() || phat < 0) {
                cout << " Loi!\n";
                cin.clear(); cin.ignore(1000, '\n');
            }
        } while (phat < 0);

        cin.ignore();

        // ===== CHỨC VỤ =====
        do {
            cout << "Chuc vu (NV / QL / Admin): ";
            getline(cin, chucVu);
            string t = toLower(chucVu);

            if (!isValidText(chucVu))
                cout << " Loi!\n";
            else if (t == "nv") { chucVu = "NV"; break; }
            else if (t == "ql") { chucVu = "QL"; break; }
            else if (t == "admin") { chucVu = "Admin"; break; }
            else cout << " Sai!\n";

        } while (true);

        // ===== CA LÀM =====
        do {
            cout << "Ca lam (Sang / Chieu / Toi): ";
            getline(cin, caLam);
            string t = toLower(caLam);

            if (!isValidText(caLam))
                cout << " Loi!\n";
            else if (t == "sang") { caLam = "Sang"; break; }
            else if (t == "chieu") { caLam = "Chieu"; break; }
            else if (t == "toi") { caLam = "Toi"; break; }
            else cout << " Sai!\n";

        } while (true);

        // ===== NGÀY TRONG THÁNG =====
        do {
            cout << "So ngay trong thang (28-31): ";
            cin >> soNgayTrongThang;

            if (cin.fail() || soNgayTrongThang < 28 || soNgayTrongThang > 31) {
                cout << " Loi!\n";
                cin.clear(); cin.ignore(1000, '\n');
            }
        } while (soNgayTrongThang < 28 || soNgayTrongThang > 31);

        // ===== NGÀY LÀM + NGHỈ =====
        do {
            do {
                cout << "So ngay lam: ";
                cin >> soNgayLam;
                if (cin.fail() || soNgayLam < 0 || soNgayLam > soNgayTrongThang) {
                    cout << " Loi!\n";
                    cin.clear(); cin.ignore(1000, '\n');
                }
            } while (soNgayLam < 0 || soNgayLam > soNgayTrongThang);

            do {
                cout << "So ngay nghi: ";
                cin >> soNgayNghi;
                if (cin.fail() || soNgayNghi < 0 || soNgayNghi > soNgayTrongThang) {
                    cout << " Loi!\n";
                    cin.clear(); cin.ignore(1000, '\n');
                }
            } while (soNgayNghi < 0 || soNgayNghi > soNgayTrongThang);

            if (soNgayLam + soNgayNghi != soNgayTrongThang)
                cout << " Tong ngay phai = " << soNgayTrongThang << endl;
            else break;

        } while (true);

        // ===== TĂNG CA =====
        do {
            cout << "Gio tang ca: ";
            cin >> gioTangCa;

            if (cin.fail() || gioTangCa < 0) {
                cout << " Loi!\n";
                cin.clear(); cin.ignore(1000, '\n');
            }
            else if (gioTangCa > 100)
                cout << " Vuot 100h!\n";
            else if (gioTangCa > soNgayLam * 24)
                cout << " Vuot gio lam!\n";
            else break;

        } while (true);
    }

    // ===== TÍNH LƯƠNG (CHUẨN THEO ĐỀ) =====
    double tinhLuong() {
        // validate
        if (luongCoBan <= 0 || heSoLuong <= 0)
            return 0;

        if (soNgayLam + soNgayNghi != soNgayTrongThang)
            return 0;

        if (gioTangCa < 0 || gioTangCa > 100)
            return 0;

        // chống overflow logic
        if (luongCoBan > 1e9 || heSoLuong > 100)
            cout << " Du lieu lon!\n";

        double luongChinh = luongCoBan * heSoLuong;
        double tienTangCa = gioTangCa * 50000;

        double tong = luongChinh + thuong + tienTangCa - phat;

        if (tong < 0) {
            cout << " Luong am!\n";
            tong = 0;
        }

        return round(tong);
    }

    // ===== XUẤT =====
    void xuat() override {
        cout << fixed << setprecision(0);

        cout << "\n[NhanVien] ";
        ConNguoi::xuat();

        cout << " | CV: " << chucVu
             << " | Ca: " << caLam
             << " | Lam: " << soNgayLam
             << " | Nghi: " << soNgayNghi
             << " | Tang ca: " << gioTangCa
             << " | Luong: " << tinhLuong();
    }
};







