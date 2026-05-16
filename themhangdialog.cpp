#include "themhangdialog.h"
#include <QMessageBox>
#include <QWidget>

ThemHangDialog::ThemHangDialog(CuaHang* shop, QWidget *parent)
    : QDialog(parent), shop(shop), hangHoaMoi(nullptr)
{
    setWindowTitle("Them hang hoa moi");
    setMinimumWidth(500);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);

    // Title
    auto* titleLabel = new QLabel("<b style='font-size:16px;'>Them hang hoa moi</b>");
    mainLayout->addWidget(titleLabel);

    // Form layout
    auto* formLayout = new QFormLayout();
    formLayout->setSpacing(10);

    // Loại hàng
    comboLoai = new QComboBox();
    comboLoai->addItem("Thuc pham che bien", "ThucPhamCheBien");
    comboLoai->addItem("Do uong", "DoUong");
    comboLoai->addItem("Do gia dung", "DoGiaDung");
    comboLoai->addItem("My pham", "MyPham");
    comboLoai->addItem("Hang tuoi song", "HangTuoiSong");
    comboLoai->addItem("Trai cay", "TraiCay");
    comboLoai->setMinimumHeight(32);
    formLayout->addRow("Loai hang:", comboLoai);

    // Mã hàng
    editMa = new QLineEdit();
    editMa->setPlaceholderText("VD: SP001");
    editMa->setMinimumHeight(32);
    formLayout->addRow("Ma hang:", editMa);

    // Tên hàng
    editTen = new QLineEdit();
    editTen->setPlaceholderText("VD: Sua tuoi Vinamilk");
    editTen->setMinimumHeight(32);
    formLayout->addRow("Ten hang:", editTen);

    // Giá nhập
    spinGiaNhap = new QDoubleSpinBox();
    spinGiaNhap->setMinimum(0);
    spinGiaNhap->setMaximum(999999999);
    spinGiaNhap->setValue(0);
    spinGiaNhap->setMinimumHeight(32);
    spinGiaNhap->setSuffix(" d");
    formLayout->addRow("Gia nhap:", spinGiaNhap);

    // Giá niêm yết
    spinGiaNiemYet = new QDoubleSpinBox();
    spinGiaNiemYet->setMinimum(0);
    spinGiaNiemYet->setMaximum(999999999);
    spinGiaNiemYet->setValue(0);
    spinGiaNiemYet->setMinimumHeight(32);
    spinGiaNiemYet->setSuffix(" d");
    formLayout->addRow("Gia niem yet:", spinGiaNiemYet);

    // Số lượng
    spinSoLuong = new QSpinBox();
    spinSoLuong->setMinimum(0);
    spinSoLuong->setMaximum(99999);
    spinSoLuong->setValue(0);
    spinSoLuong->setMinimumHeight(32);
    formLayout->addRow("So luong:", spinSoLuong);

    // Ngày hết hạn
    editNgayHetHan = new QLineEdit();
    editNgayHetHan->setPlaceholderText("dd/mm/yyyy");
    editNgayHetHan->setMinimumHeight(32);
    formLayout->addRow("Ngay het han:", editNgayHetHan);

    mainLayout->addLayout(formLayout);

    // Stacked widget for type-specific fields
    stackedSpecific = new QStackedWidget();

    // Page 0: ThucPhamCheBien - no extra fields
    auto* pageTPCB = new QWidget();
    stackedSpecific->addWidget(pageTPCB);

    // Page 1: DoUong - no extra fields
    auto* pageDU = new QWidget();
    stackedSpecific->addWidget(pageDU);

    // Page 2: DoGiaDung - bảo hành
    auto* pageDGD = new QWidget();
    auto* layoutDGD = new QFormLayout(pageDGD);
    spinBaoHanh = new QSpinBox();
    spinBaoHanh->setMinimum(0);
    spinBaoHanh->setMaximum(120);
    spinBaoHanh->setValue(12);
    spinBaoHanh->setMinimumHeight(32);
    spinBaoHanh->setSuffix(" thang");
    layoutDGD->addRow("Bao hanh:", spinBaoHanh);
    stackedSpecific->addWidget(pageDGD);

    // Page 3: MyPham - no extra fields
    auto* pageMP = new QWidget();
    stackedSpecific->addWidget(pageMP);

    // Page 4: HangTuoiSong - khối lượng + tình trạng
    auto* pageHTS = new QWidget();
    auto* layoutHTS = new QFormLayout(pageHTS);
    spinKhoiLuongHTS = new QDoubleSpinBox();
    spinKhoiLuongHTS->setMinimum(0);
    spinKhoiLuongHTS->setMaximum(9999);
    spinKhoiLuongHTS->setValue(1.0);
    spinKhoiLuongHTS->setMinimumHeight(32);
    spinKhoiLuongHTS->setSuffix(" kg");
    layoutHTS->addRow("Khoi luong:", spinKhoiLuongHTS);
    editTinhTrangHTS = new QLineEdit();
    editTinhTrangHTS->setPlaceholderText("VD: Tuoi, Tot");
    editTinhTrangHTS->setMinimumHeight(32);
    layoutHTS->addRow("Tinh trang:", editTinhTrangHTS);
    stackedSpecific->addWidget(pageHTS);

    // Page 5: TraiCay - khối lượng + tình trạng
    auto* pageTC = new QWidget();
    auto* layoutTC = new QFormLayout(pageTC);
    spinKhoiLuongTC = new QDoubleSpinBox();
    spinKhoiLuongTC->setMinimum(0);
    spinKhoiLuongTC->setMaximum(9999);
    spinKhoiLuongTC->setValue(1.0);
    spinKhoiLuongTC->setMinimumHeight(32);
    spinKhoiLuongTC->setSuffix(" kg");
    layoutTC->addRow("Khoi luong:", spinKhoiLuongTC);
    editTinhTrangTC = new QLineEdit();
    editTinhTrangTC->setPlaceholderText("VD: Tuoi, Tot");
    editTinhTrangTC->setMinimumHeight(32);
    layoutTC->addRow("Tinh trang:", editTinhTrangTC);
    stackedSpecific->addWidget(pageTC);

    mainLayout->addWidget(stackedSpecific);

    // Connect loại changed
    connect(comboLoai, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ThemHangDialog::onLoaiChanged);

    // Buttons
    auto* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    auto* btnHuy = new QPushButton("Huy");
    btnHuy->setMinimumSize(80, 36);
    btnHuy->setStyleSheet(
        "QPushButton{background:#E0E0E0;border-radius:6px;}"
        "QPushButton:hover{background:#BDBDBD;}");
    connect(btnHuy, &QPushButton::clicked, this, &QDialog::reject);
    btnLayout->addWidget(btnHuy);

    auto* btnThem = new QPushButton("Them hang");
    btnThem->setMinimumSize(100, 36);
    btnThem->setStyleSheet(
        "QPushButton{background:#1A237E;color:white;border-radius:6px;font-weight:bold;}"
        "QPushButton:hover{background:#3F51B5;}");
    connect(btnThem, &QPushButton::clicked, this, &ThemHangDialog::onXacNhan);
    btnLayout->addWidget(btnThem);

    mainLayout->addLayout(btnLayout);

    // Set initial page
    onLoaiChanged(0);
}

void ThemHangDialog::onLoaiChanged(int index)
{
    stackedSpecific->setCurrentIndex(index);
}

void ThemHangDialog::onXacNhan()
{
    // Validate inputs
    QString ma = editMa->text().trimmed();
    QString ten = editTen->text().trimmed();
    float giaNhap = spinGiaNhap->value();
    float giaNiemYet = spinGiaNiemYet->value();
    int soLuong = spinSoLuong->value();
    QString ngayHetHan = editNgayHetHan->text().trimmed();

    if (ma.isEmpty()) {
        QMessageBox::warning(this, "Loi", "Vui long nhap ma hang.");
        return;
    }

    if (shop->ds.coTrungMa(ma.toStdString())) {
        QMessageBox::warning(this, "Loi", "Ma hang da ton tai. Vui long chon ma khac.");
        return;
    }

    if (ten.isEmpty()) {
        QMessageBox::warning(this, "Loi", "Vui long nhap ten hang.");
        return;
    }

    if (giaNhap <= 0 || giaNiemYet <= 0) {
        QMessageBox::warning(this, "Loi", "Gia nhap va gia niem yet phai lon hon 0.");
        return;
    }

    // Validate ngày hết hạn
    if (!ngayHetHan.isEmpty()) {
        QStringList parts = ngayHetHan.split('/');
        if (parts.size() != 3) {
            QMessageBox::warning(this, "Loi", "Ngay het han phai co dinh dang dd/mm/yyyy");
            return;
        }
        bool ok1, ok2, ok3;
        int d = parts[0].toInt(&ok1);
        int m = parts[1].toInt(&ok2);
        int y = parts[2].toInt(&ok3);
        if (!ok1 || !ok2 || !ok3 || d < 1 || d > 31 || m < 1 || m > 12 || y < 2000 || y > 2100) {
            QMessageBox::warning(this, "Loi", "Ngay het han khong hop le.");
            return;
        }
    }

    // Create appropriate object based on type
    QString loaiData = comboLoai->currentData().toString();
    HangHoa* h = nullptr;

    if (loaiData == "ThucPhamCheBien") {
        h = new ThucPhamCheBien();
    } else if (loaiData == "DoUong") {
        h = new DoUong();
    } else if (loaiData == "DoGiaDung") {
        auto* dg = new DoGiaDung();
        dg->setBaoHanh(spinBaoHanh->value());
        h = dg;
    } else if (loaiData == "MyPham") {
        h = new MyPham();
    } else if (loaiData == "HangTuoiSong") {
        auto* hts = new HangTuoiSong();
        hts->setKhoiLuong(spinKhoiLuongHTS->value());
        hts->setTinhTrang(editTinhTrangHTS->text().trimmed().toStdString());
        h = hts;
    } else if (loaiData == "TraiCay") {
        auto* tc = new TraiCay();
        tc->setKhoiLuong(spinKhoiLuongTC->value());
        tc->setTinhTrang(editTinhTrangTC->text().trimmed().toStdString());
        h = tc;
    }

    if (!h) {
        QMessageBox::critical(this, "Loi", "Khong the tao hang hoa.");
        return;
    }

    // Set common fields
    h->setMa(ma.toStdString());
    h->setTen(ten.toStdString());
    h->setGiaNhap(giaNhap);
    h->setGiaNiemYet(giaNiemYet);
    h->setSoLuong(soLuong);
    h->setNgayHetHan(ngayHetHan.toStdString());
    h->setDvt(h->getDefaultDvt());
    h->setKeHang(h->getDefaultKeHang());

    hangHoaMoi = h;
    accept();
}

HangHoa* ThemHangDialog::getHangHoaMoi() const
{
    return hangHoaMoi;
}
