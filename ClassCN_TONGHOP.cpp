// ================= HE THONG QUAN LY =================
// Chương trình quản lý Nhân viên và Khách hàng với validation đầy đủ

#include <iostream>
#include <ctime>
#include <set>
#include <vector>
#include <regex>
#include <algorithm>
#include <string>
#include <limits>
#include <cmath>        // cho round()
using namespace std;

// ================= LOP TRUU TUONG CON NGUOI =================
class ConNguoi {
protected:
	string ma, ten, sdt, cccd;
	static set<string> maTinh;
	static vector<string> dsMa;      // Danh sách mã 
	static vector<string> dsSDT;     // Danh sách SĐT 
	static vector<string> dsCCCD;    // Danh sách CCCD

public:
	virtual void nhap() = 0;
	virtual void xuat() const = 0;
	virtual ~ConNguoi() {}
	
protected:
	// ================= KIỂM TRA TRÙNG DỮ LIỆU =================
	// Hàm kiểm tra xem một giá trị có tồn tại trong danh sách hay không
	static bool trung(const string &v, const vector<string> &ds) {
		return find(ds.begin(), ds.end(), v) != ds.end();
	}
	
	// ================= CHUẨN HOÁ TÊN =================
	// Xoá khoảng trắng thừa, viết hoa chữ cái đầu mỗi từ
	string chuanHoaTen(string s) {
		// Xoá khoảng trắng đầu/cuối và thay nhiều khoảng trắng liên tiếp bằng 1 khoảng trắng
		s = regex_replace(s, regex("^\\s+|\\s+$"), "");
		s = regex_replace(s, regex("\\s+"), " ");
		
		// Chuyển hết về chữ thường trước
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		
		// Viết hoa chữ cái đầu mỗi từ
		bool newWord = true;
		for (char &c : s) {
			if (c == ' ') newWord = true;
			else if (newWord) {
				c = toupper(c);
				newWord = false;
			}
		}
		return s;
	}
	// ================= CHUẨN HOÁ TÊN =================
	string chuanHoaCCCD(string s) {
		s.erase(remove(s.begin(), s.end(), ' '), s.end());
		return s;
	}
	// ================= CHUẨN HOÁ SỐ ĐIỆN THOẠI =================
	// Xoá khoảng trắng, chuyển +84 → 0
	string chuanHoaSDT(string s) {
		s.erase(remove(s.begin(), s.end(), ' '), s.end());
		if (s.substr(0, 3) == "+84") 
			s = "0" + s.substr(3);
		return s;
	}
	
	// ================= BẮT LỖI TÊN (TỔNG QUÁT) =================
	// Kiểm tra tên hợp lệ: không rỗng, độ dài, không chứa số/ký tự đặc biệt, không nhiều khoảng trắng
	void ktTen(const string &s) {
		if (s.empty()) 
			throw invalid_argument("Ten khong duoc rong!");
		if (s.length() < 2 || s.length() > 50) 
			throw invalid_argument("Do dai ten phai tu 2 den 50 ky tu!");
		if (regex_search(s, regex("[0-9]"))) 
			throw invalid_argument("Ten khong duoc chua so!");
		if (regex_search(s, regex("[@#$%^&*()!~`.,?;:]"))) 
			throw invalid_argument("Ten khong duoc chua ky tu dac biet!");
		if (regex_search(s, regex("  ")))   // nhiều khoảng trắng liên tiếp
			throw invalid_argument("Ten khong duoc co nhieu khoang trang lien tiep!");
	}
	
	// ================= BẮT LỖI MÃ (TỔNG QUÁT) =================
	// Kiểm tra mã theo prefix (NV hoặc KH), định dạng XXX001, không trùng
	void ktMa(const string &s, const string &prefix) {
		if (s.empty()) 
			throw invalid_argument("Ma khong duoc rong!");
		if (regex_search(s, regex("\\s"))) 
			throw invalid_argument("Ma khong duoc chua khoang trang!");
		
		regex p("^" + prefix + "\\d{3}$");
		if (!regex_match(s, p)) 
			throw invalid_argument("Ma phai co dang " + prefix + "001 (3 chu so)!");
		
		if (s.substr(prefix.size()) == "000") 
			throw invalid_argument("So thu tu trong ma khong duoc la 000!");
		
		if (trung(s, dsMa)) 
			throw invalid_argument("Ma nay da ton tai trong he thong!");
	}
	
	// ================= BẮT LỖI SỐ ĐIỆN THOẠI (TỔNG QUÁT) =================
	// Kiểm tra SĐT: chỉ chứa số, độ dài 9-11, bắt đầu bằng 0, không trùng
	void ktSDT(const string &s) {
		if (!regex_match(s, regex("\\d+"))) 
			throw invalid_argument("So dien thoai chi duoc chua so!");
		if (s.length() < 9 || s.length() > 11) 
			throw invalid_argument("Do dai so dien thoai phai tu 9 den 11 so!");
		if (s[0] != '0') 
			throw invalid_argument("So dien thoai phai bat dau bang so 0!");
		if (trung(s, dsSDT)) 
			throw invalid_argument("So dien thoai nay da ton tai trong he thong!");
	}
	// ================= BẮT LỖI CĂN CƯỚC CÔNG DÂN =================
	void ktCCCD(const string &s) {
		if (!regex_match(s, regex("\\d{12}")))
			throw invalid_argument("CCCD phai 12 so");
		
		if (regex_match(s, regex("(\\d)\\1{11}")))
			throw invalid_argument("CCCD khong hop le");
		
		string ma = s.substr(0,3);
		if (maTinh.find(ma) == maTinh.end())
			throw invalid_argument("Ma tinh sai");
		
		int gt = s[3] - '0';
		if (gt < 0 || gt > 3)
			throw invalid_argument("Gioi tinh sai");
		
		int nam = stoi(s.substr(4,2));
		int fullYear = (gt <= 1 ? 1900 : 2000) + nam;
		
		time_t t = time(0);
		tm* now = localtime(&t);
		int currentYear = now->tm_year + 1900;
		
		if (fullYear > currentYear || fullYear < 1900)
			throw invalid_argument("Nam sinh sai");
		
		if (trung(s, dsCCCD))
			throw invalid_argument("Trung CCCD");
	}
	// ================= NHẬP THÔNG TIN CƠ BẢN (CÓ VALIDATION) =================
	// Hàm chung để nhập Mã - Tên - SĐT với bắt lỗi
	void nhapCoBan(const string &prefix) {
		// ----- Nhập và kiểm tra Mã -----
		while (true) {
			try {
				cout << "Nhap Ma (" << prefix << "xxx): ";
				cin >> ma;
				ktMa(ma, prefix);
				dsMa.push_back(ma);
				break;
			} catch (const exception &e) {
				cout << "Loi: " << e.what() << endl;
			}
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Xóa bộ đệm
		
		// ----- Nhập và kiểm tra Tên -----
		while (true) {
			try {
				cout << "Nhap Ten: ";
				getline(cin, ten);
				ten = chuanHoaTen(ten);
				ktTen(ten);
				break;
			} catch (const exception &e) {
				cout << "Loi: " << e.what() << endl;
			}
		}
		
		// ----- Nhập và kiểm tra SĐT -----
		while (true) {
			try {
				cout << "Nhap SDT: ";
				cin >> sdt;
				sdt = chuanHoaSDT(sdt);
				ktSDT(sdt);
				dsSDT.push_back(sdt);
				break;
			} catch (const exception &e) {
				cout << "Loi: " << e.what() << endl;
			}
		}
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	// ----- Nhập và kiểm tra CCCD -----
		while (true) {
			try {
				cout << "Nhap CCCD: ";
				cin >> cccd;
				
				cccd = chuanHoaCCCD(cccd);
				
				ktCCCD(cccd);
				dsCCCD.push_back(cccd);
				break;
			} catch (const exception &e) {
				cout << "Loi: " << e.what() << endl;
			}
		}
	}
	void xuatCoBan() const {
		cout << ma << " | " << ten << " | " << sdt << " | " << cccd;
	}
};
set<string> ConNguoi::maTinh = {
	"001","002","004","006","008","010","011","012",
	"014","015","017","019","020","022","024","025",
	"026","027","030","031","033","034","035","036",
	"037","038","040","042","044","045","046","048",
	"049","051","052","054","056","058","060","062",
	"064","066","067","068","070","072","074","075",
	"077","079","080","082","083","084","086","087",
	"089","091","092","093","094","095","096"
};
// Khởi tạo static members
vector<string> ConNguoi::dsMa;
vector<string> ConNguoi::dsSDT;
vector<string> ConNguoi::dsCCCD;

// ================= LỚP NHÂN VIÊN =================
class NhanVien : public ConNguoi {
private:
	string chucVu, caLam;
public:
	void nhap() override {
		nhapCoBan("NV");
		
		// Nhập chức vụ (NV / QL / Admin)
		while (true) {
			try {
				cout << "Nhap Chuc vu (NV/QL/Admin): ";
				getline(cin, chucVu);
				chucVu = regex_replace(chucVu, regex("^\\s+|\\s+$"), "");
				string t = chucVu;
				transform(t.begin(), t.end(), t.begin(), ::tolower);
				
				if (t == "nv") chucVu = "NV";
				else if (t == "ql") chucVu = "QL";
				else if (t == "admin") chucVu = "Admin";
				else throw invalid_argument("Chuc vu chi duoc la NV, QL hoac Admin!");
				
				break;
			} catch (const exception &e) {
				cout << "Loi: " << e.what() << endl;
			}
		}
		
		// Nhập ca làm (Sáng / Chiều / Tối)
		while (true) {
			try {
				cout << "Nhap Ca lam (Sang/Chieu/Toi): ";
				getline(cin, caLam);
				caLam = regex_replace(caLam, regex("^\\s+|\\s+$"), "");
				string t = caLam;
				transform(t.begin(), t.end(), t.begin(), ::tolower);
				
				if (t == "sang") caLam = "Sang";
				else if (t == "chieu") caLam = "Chieu";
				else if (t == "toi") caLam = "Toi";
				else throw invalid_argument("Ca lam chi duoc la Sang, Chieu hoac Toi!");
				
				break;
			} catch (const exception &e) {
				cout << "Loi: " << e.what() << endl;
			}
		}
	}
	
	void xuat() const override {
		cout << "[NHAN VIEN] ";
		xuatCoBan();
		cout << " | Chuc vu: " << chucVu << " | Ca lam: " << caLam << endl;
	}
};

// ================= LỚP KHÁCH HÀNG =================
class KhachHang : public ConNguoi {
private:
	vector<long long> lichSu;   // Lịch sử các lần mua/thanh toán
	long long tongTien;
	int diem;
	string loai;
	
public:
	KhachHang() : tongTien(0), diem(0), loai("Member") {}
	
	// Cập nhật tổng tiền mua hàng (tránh tràn số)
	void capNhatTongTien() {
		long long temp = 0;
		for (auto x : lichSu) {
			if (x <= 0) continue;
			if (temp > LLONG_MAX - x) {
				temp = LLONG_MAX;
				break;
			}
			temp += x;
		}
		tongTien = temp;
	}
	
	// Cập nhật loại khách hàng dựa trên tổng tiền
	void capNhatLoai() {
		if (tongTien >= 5000000) loai = "VVIP";
		else if (tongTien >= 1000000) loai = "VIP";
		else loai = "Member";
	}
	
	// Cộng điểm (1.000đ = 1 điểm)
	void congDiem(long long tien) {
		if (tien <= 0) return;
		int d = (int)round((double)tien / 10000);
		if (d < 0) d = 0;
		if (diem > INT_MAX - d) diem = INT_MAX;
		else diem += d;
	}
	
	// Trừ điểm khi dùng để thanh toán
	void truDiem(int d) {
		if (d < 0) return;
		if (d > diem) d = diem;
		diem -= d;
	}
	
	// ================= MUA HÀNG (KHÔNG DÙNG ĐIỂM) =================
	void themMua() {
		long long tien;
		cout << "Nhap tien mua: ";
		cin >> tien;
		if (cin.fail() || tien <= 0) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Du lieu tien mua khong hop le!\n";
			return;
		}
		
		int tich;
		cout << "Tich diem khong? (1 = Co, 0 = Khong): ";
		cin >> tich;
		if (tich != 0 && tich != 1) tich = 0;
		
		long long tienSau = tien;
		if (tich == 1) congDiem(tienSau);
		
		lichSu.push_back(tienSau);
		capNhatTongTien();
		capNhatLoai();
		
		cout << "Mua hang thanh cong!\n";
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	
	// ================= THANH TOÁN (CÓ THỂ DÙNG ĐIỂM) =================
	void thanhToan() {
		long long tong;
		cout << "Nhap tong tien: ";
		cin >> tong;
		if (cin.fail() || tong <= 0) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Du lieu tong tien khong hop le!\n";
			return;
		}
		
		int dung;
		cout << "Su dung diem? (1 = Co, 0 = Khong): ";
		cin >> dung;
		if (dung != 0 && dung != 1) dung = 0;
		
		int diemDung = 0;
		long long giam = 0;
		
		if (dung == 1) {
			cout << "Nhap so diem muon dung: ";
			cin >> diemDung;
			if (cin.fail() || diemDung < 0) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "So diem khong hop le!\n";
				return;
			}
			if (diemDung > diem) diemDung = diem;
			
			giam = (long long)diemDung * 1000;
			if (giam > tong / 2) giam = tong / 2;   // Giới hạn giảm tối đa 50%
			
			diemDung = (int)(giam / 1000);   // Quy đổi lại điểm thực tế đã dùng
		}
		
		long long tienSau = tong - giam;
		if (tienSau < 0) tienSau = 0;
		
		truDiem(diemDung);   // Trừ điểm trước
		
		// Hỏi có tích điểm sau thanh toán không
		int tich = 0;
		cout << "Tich diem sau thanh toan? (1 = Co, 0 = Khong): ";
		cin >> tich;
		if (tich != 0 && tich != 1) tich = 0;
		
		if (tich == 1) congDiem(tienSau);
		
		lichSu.push_back(tienSau);
		capNhatTongTien();
		capNhatLoai();
		
		cout << "Thanh toan thanh cong! Tien phai tra: " << tienSau << endl;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	
	void nhap() override {
		nhapCoBan("KH");
	}
	
	void xuat() const override {
		cout << "[KHACH HANG] ";
		xuatCoBan();
		cout << " | Tong tien: " << tongTien 
		<< " | Diem: " << diem 
		<< " | Loai: " << loai << endl;
	}
};

// ================= CHƯƠNG TRÌNH CHÍNH =================
int main() {
	KhachHang kh;
	cout << "=== NHAP THONG TIN KHACH HANG ===\n";
	kh.nhap();
	
	int chon;
	do {
		cout << "\n=== MENU ===\n";
		cout << "1. Mua hang (khong dung diem)\n";
		cout << "2. Thanh toan (co the dung diem)\n";
		cout << "3. Xuat thong tin khach hang\n";
		cout << "0. Thoat\n";
		cout << "Chon chuc nang: ";
		cin >> chon;
		
		if (chon == 1) kh.themMua();
		else if (chon == 2) kh.thanhToan();
		else if (chon == 3) kh.xuat();
		else if (chon != 0) 
			cout << "Lua chon khong hop le!\n";
		
		cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Đảm bảo sạch bộ đệm
	} while (chon != 0);
	
	cout << "Cam on ban da su dung chuong trinh!\n";
	return 0;
}
