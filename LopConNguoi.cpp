// =====================================================================
// PHẦN 3: LỚP CON NGƯỜI (BASE CLASS TRỪU TƯỢNG)
// =====================================================================

class ConNguoi {
protected:
    string ma;
    string ten;
    string sdt;
public:
    virtual ~ConNguoi() {}

    // Các hàm thuần ảo - lớp con bắt buộc phải cài đặt
    virtual void nhap() = 0;
    virtual void xuat() const = 0;
    virtual string getLoai() const = 0;

    // Getter
    string getMa()  const { return ma; }
    string getTen() const { return ten; }
    string getSDT() const { return sdt; }

    // Setter
    void setMa(const string& m)  { ma  = m; }
    void setTen(const string& t) { ten = t; }
    void setSDT(const string& s) { sdt = s; }
};
