#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "logindialog.h"
#include "nhapkhodialog.h"
#include "suahangdialog.h"
#include "themhangdialog.h"
#include <QDebug>
#include <fstream>
#include <ctime>

#include <QButtonGroup>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QMessageBox>
#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QColor>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QFont>
#include <QString>
#include <QSet>
#include <QLocale>
#include <QLabel>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QDateEdit>
#include <QEvent>
#include <QMouseEvent>
#include <QFormLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , menuGroup(nullptr)
{
    ui->setupUi(this);
    resize(1280, 780);
    applyStyleSheet();
    setupMenuGroup();

    connect(ui->btnTongQuan,   &QPushButton::clicked, this, &MainWindow::onTongQuan);
    connect(ui->btnQuanLyKho,  &QPushButton::clicked, this, &MainWindow::onQuanLyKho);
    connect(ui->btnBanHang,    &QPushButton::clicked, this, &MainWindow::onBanHang);
    connect(ui->btnKhachHang,  &QPushButton::clicked, this, &MainWindow::onTraCuuHangHoa); // Đổi thành tra cứu hàng hóa
    connect(ui->btnBaoCao,     &QPushButton::clicked, this, &MainWindow::onBaoCao);
    connect(ui->btnDangXuat,   &QPushButton::clicked, this, &MainWindow::onDangXuat);

    connect(ui->lineEditTimKiem, &QLineEdit::textChanged, this, &MainWindow::onTimKiemHangHoa);
    connect(ui->comboLoai, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onLocLoai);
    connect(ui->btnThemHang, &QPushButton::clicked, this, &MainWindow::onThemHang);
    connect(ui->btnSuaHang, &QPushButton::clicked, this, &MainWindow::onSuaHang);
    connect(ui->btnNhapKho, &QPushButton::clicked, this, &MainWindow::onNhapKho);
    connect(ui->btnXoaHang, &QPushButton::clicked, this, &MainWindow::onXoaHang);
    connect(ui->btnHuyHangHetHan, &QPushButton::clicked, this, &MainWindow::onHuyHangHetHan);
    connect(ui->btnSapXepKho, &QPushButton::clicked, this, &MainWindow::onSapXepKho);
    connect(ui->btnTaoDanhSachTheoDoi, &QPushButton::clicked, this, &MainWindow::onTaoDanhSachTheoDoi);
    connect(ui->lineEditTimKH, &QLineEdit::textChanged, this, &MainWindow::onTimKiemKhachHang);

    connect(ui->lineEditTimHang, &QLineEdit::textChanged, this, &MainWindow::onTimHangBanHang);
    connect(ui->btnThemVaoGio, &QPushButton::clicked, this, &MainWindow::onThemVaoGio);
    connect(ui->btnXoaGio,    &QPushButton::clicked, this, &MainWindow::onXoaGio);
    connect(ui->btnTimKhach,  &QPushButton::clicked, this, &MainWindow::onTimKhachHangBH);
    connect(ui->btnThanhToan, &QPushButton::clicked, this, &MainWindow::onThanhToan);

    // Báo cáo
    connect(ui->btnBaoCaoDoanhThu, &QPushButton::clicked, this, &MainWindow::onBaoCaoDoanhThu);
    connect(ui->btnLichSuHuy, &QPushButton::clicked, this, &MainWindow::onLichSuHuy);
    connect(ui->btnThongTinKH, &QPushButton::clicked, this, &MainWindow::onThongTinKH);

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::khoiDong()
{

    shop.docFile     ("data.txt");
    shop.docKhachHang("khachhang.txt");
    shop.docNhanVien ("nhanvien.txt");
    shop.docLichSuHoaDon("lichsuhoadon.txt");

    qDebug() << "Hang hoa loaded:" << shop.ds.toVector().size();
    qDebug() << "Nhan vien loaded:" << shop.dsNhanVien.toVector().size();
    qDebug() << "Khach hang loaded:" << shop.dsKhachHang.toVector().size();
    qDebug() << "Lich su hoa don loaded:" << shop.lichSuHoaDon.size();

    while (true) {
        LoginDialog dlg(this);
        if (dlg.exec() != QDialog::Accepted) {
            QApplication::quit(); return;
        }
        QString maNV    = dlg.getMaNV();
        QString matKhau = dlg.getMatKhau();
        if (shop.dangNhapCore(maNV.toStdString(), matKhau.toStdString())) {
            NhanVien* nv = shop.nhanVienHienTai;
            QString tenNV  = nv ? QString::fromStdString(nv->getTen())    : maNV;
            QString chucVu = nv ? QString::fromStdString(nv->getChucVu()) : "";
            setWindowTitle("Sieu Thi - " + tenNV);
            // Set text + căn giữa bằng code, không dùng qproperty trong CSS
            ui->labelNhanVien->setText("NV: " + tenNV + "\n" + chucVu);
            ui->labelNhanVien->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            break;
        }
        QMessageBox::warning(this, "Dang nhap that bai",
                             "Ma nhan vien hoac mat khau khong dung.");
    }

    loadDashboard();
    loadHangHoa();
    loadKhachHang();
    loadDanhSachHangBH();
    loadBaoCao();
    show();
}

void MainWindow::setupMenuGroup()
{
    menuGroup = new QButtonGroup(this);
    menuGroup->setExclusive(true);
    menuGroup->addButton(ui->btnTongQuan,  0);
    menuGroup->addButton(ui->btnQuanLyKho, 1);
    menuGroup->addButton(ui->btnBanHang,   2);
    menuGroup->addButton(ui->btnKhachHang, 3);
    menuGroup->addButton(ui->btnBaoCao,    4);
}

void MainWindow::chuyenTrang(int index) {
    ui->stackedWidget->setCurrentIndex(index);
}

void MainWindow::onTongQuan()  { chuyenTrang(0); }
void MainWindow::onQuanLyKho() {
    chuyenTrang(1);
    hienThiCanhBaoKho();
}
void MainWindow::onBanHang()   { chuyenTrang(2); }
void MainWindow::onKhachHang() { chuyenTrang(3); }
void MainWindow::onBaoCao()    { chuyenTrang(4); }

void MainWindow::onTraCuuHangHoa()
{
    QDialog* dlg = new QDialog(this);
    dlg->setWindowTitle("Tra cuu hang hoa");
    dlg->resize(1000, 600);

    QVBoxLayout* layout = new QVBoxLayout(dlg);

    // Thanh tìm kiếm
    QHBoxLayout* searchLayout = new QHBoxLayout();
    QLabel* lblSearch = new QLabel("Tim kiem:", dlg);
    QLineEdit* editSearch = new QLineEdit(dlg);
    editSearch->setPlaceholderText("Nhap ma hang hoac ten hang...");
    editSearch->setMinimumWidth(400);
    searchLayout->addWidget(lblSearch);
    searchLayout->addWidget(editSearch);
    searchLayout->addStretch();
    layout->addLayout(searchLayout);

    // Bảng hiển thị kết quả
    QTableWidget* table = new QTableWidget(dlg);
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({"Ma hang", "Ten hang", "Loai", "DVT", "Gia nhap", "Gia ban", "Ton kho"});
    table->setAlternatingRowColors(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    layout->addWidget(table);

    // Hàm load dữ liệu
    auto loadData = [&](const QString& tuKhoa) {
        auto dsH = shop.ds.toVector();
        vector<HangHoa*> ketQua;

        QString tk = tuKhoa.trimmed().toLower();
        if (tk.isEmpty()) {
            ketQua = dsH;
        } else {
            for (HangHoa* h : dsH) {
                QString ma = QString::fromStdString(h->getMa()).toLower();
                QString ten = QString::fromStdString(h->getTen()).toLower();
                if (ma.contains(tk) || ten.contains(tk)) {
                    ketQua.push_back(h);
                }
            }
        }

        table->setRowCount(ketQua.size());
        for (int i = 0; i < (int)ketQua.size(); i++) {
            HangHoa* h = ketQua[i];
            table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(h->getMa())));
            table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(h->getTen())));
            table->setItem(i, 2, new QTableWidgetItem(loaiHienThi(h->getLoai())));
            table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(h->getDvt())));
            table->setItem(i, 4, new QTableWidgetItem(formatTien(h->getGiaNhap())));
            table->setItem(i, 5, new QTableWidgetItem(formatTien(h->getGiaNiemYet())));
            table->setItem(i, 6, new QTableWidgetItem(QString::number(h->getSoLuong())));
        }
        table->resizeColumnsToContents();
        table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    };

    // Load tất cả hàng hóa ban đầu
    loadData("");

    // Kết nối tìm kiếm
    connect(editSearch, &QLineEdit::textChanged, [=](const QString& text) {
        loadData(text);
    });

    // Double-click để xem chi tiết
    connect(table, &QTableWidget::cellDoubleClicked, [=](int row, int col) {
        Q_UNUSED(col);
        if (row < 0 || row >= table->rowCount()) return;

        QString maHang = table->item(row, 0)->text();
        HangHoa* h = shop.ds.timTheoMa(maHang.toStdString());
        if (!h) return;

        // Dialog hiển thị chi tiết
        QDialog* detailDlg = new QDialog(dlg);
        detailDlg->setWindowTitle("Thong tin chi tiet hang hoa");
        detailDlg->setMinimumWidth(500);

        QVBoxLayout* detailLayout = new QVBoxLayout(detailDlg);

        // Thông tin cơ bản
        QGroupBox* basicGroup = new QGroupBox("Thong tin co ban", detailDlg);
        QFormLayout* basicForm = new QFormLayout(basicGroup);
        basicForm->addRow("Ma hang:", new QLabel(QString::fromStdString(h->getMa())));
        basicForm->addRow("Ten hang:", new QLabel(QString::fromStdString(h->getTen())));
        basicForm->addRow("Loai:", new QLabel(loaiHienThi(h->getLoai())));
        basicForm->addRow("Don vi tinh:", new QLabel(QString::fromStdString(h->getDvt())));
        basicForm->addRow("So luong ton:", new QLabel(QString::number(h->getSoLuong())));
        detailLayout->addWidget(basicGroup);

        // Thông tin giá
        QGroupBox* priceGroup = new QGroupBox("Thong tin gia", detailDlg);
        QFormLayout* priceForm = new QFormLayout(priceGroup);
        priceForm->addRow("Gia goc:", new QLabel(formatTien(h->getGiaNhap())));
        priceForm->addRow("Gia ban:", new QLabel(formatTien(h->getGiaNiemYet())));
        detailLayout->addWidget(priceGroup);

        // Thông tin hạn sử dụng
        QGroupBox* dateGroup = new QGroupBox("Han su dung", detailDlg);
        QFormLayout* dateForm = new QFormLayout(dateGroup);
        dateForm->addRow("Ngay het han:", new QLabel(QString::fromStdString(h->getNgayHetHan())));
        int soNgayConLai = h->tinhSoNgayConLai();
        QString trangThai = trangThaiHang(h);
        QLabel* lblTrangThai = new QLabel(trangThai);
        lblTrangThai->setStyleSheet(QString("color: %1; font-weight: bold;").arg(mauTrangThai(h).name()));
        dateForm->addRow("Trang thai:", lblTrangThai);
        dateForm->addRow("So ngay con lai:", new QLabel(soNgayConLai >= 0 ? QString::number(soNgayConLai) : "Da het han"));
        detailLayout->addWidget(dateGroup);

        // Nút đóng
        QPushButton* btnClose = new QPushButton("Dong", detailDlg);
        btnClose->setStyleSheet("QPushButton{background:#757575;color:white;padding:8px 16px;border-radius:4px;}"
                                "QPushButton:hover{background:#616161;}");
        connect(btnClose, &QPushButton::clicked, detailDlg, &QDialog::accept);
        detailLayout->addWidget(btnClose, 0, Qt::AlignRight);

        detailDlg->exec();
        delete detailDlg;
    });

    // Nút đóng
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnClose = new QPushButton("Dong", dlg);
    btnClose->setStyleSheet("QPushButton{background:#757575;color:white;padding:8px 16px;border-radius:4px;}"
                            "QPushButton:hover{background:#616161;}");
    connect(btnClose, &QPushButton::clicked, dlg, &QDialog::accept);
    btnLayout->addStretch();
    btnLayout->addWidget(btnClose);
    layout->addLayout(btnLayout);

    dlg->exec();
    delete dlg;
}

    void MainWindow::onDangXuat()
    {
        if (QMessageBox::question(this, "Dang xuat", "Ban co chac muon dang xuat?",
                                  QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
            shop.nhanVienHienTai = nullptr;
            // KHÔNG ghi file ở đây
            gioHang.clear();
            khoiDong();
        }
    }

void MainWindow::loadDashboard()
{
    auto dsH = shop.ds.toVector();
    auto dsK = shop.dsKhachHang.toVector();
    int sapHetHan=0, sapHetKho=0;
    for (HangHoa* h : dsH) {
        int n = h->tinhSoNgayConLai();
        if (n >= 0 && n <= 7) sapHetHan++;
        if (h->getSoLuong() < 10) sapHetKho++;
    }
    ui->labelCardHH_So ->setText(QString::number((int)dsH.size()));
    ui->labelCardHan_So->setText(QString::number(sapHetHan));
    ui->labelCardKH_So ->setText(QString::number((int)dsK.size()));
    ui->labelCardSH_So ->setText(QString::number(sapHetKho));
    loadCanhBao();
}

void MainWindow::loadCanhBao()
{
    auto dsH = shop.ds.toVector();
    vector<HangHoa*> ds;
    for (HangHoa* h : dsH) {
        int n = h->tinhSoNgayConLai();
        if ((n >= 0 && n <= 7) || h->getSoLuong() < 10)
            ds.push_back(h);
    }
    QTableWidget* tbl = ui->tableCanhBao;
    tbl->setRowCount((int)ds.size());
    tbl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    for (int i = 0; i < (int)ds.size(); i++) {
        HangHoa* h = ds[i];
        auto cell = [&](int col, const QString& text) {
            auto* it = new QTableWidgetItem(text);
            it->setTextAlignment(Qt::AlignCenter);
            tbl->setItem(i, col, it);
        };
        cell(0, QString::fromStdString(h->getMa()));
        cell(1, QString::fromStdString(h->getTen()));
        cell(2, loaiHienThi(h->getLoai()));
        cell(3, QString::number(h->getSoLuong()));
        cell(4, QString::fromStdString(h->getNgayHetHan()));
        auto* itTT = new QTableWidgetItem(trangThaiHang(h));
        QColor mau = mauTrangThai(h);
        itTT->setTextAlignment(Qt::AlignCenter);
        itTT->setForeground(mau);
        QFont f=itTT->font(); f.setBold(true); itTT->setFont(f);
        tbl->setItem(i, 5, itTT);
        QColor bg = mau; bg.setAlpha(25);
        for (int c=0;c<6;c++) if(tbl->item(i,c)) tbl->item(i,c)->setBackground(bg);
    }
    tbl->resizeColumnsToContents();
    tbl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void MainWindow::loadHangHoa()
{
    auto dsH = shop.ds.toVector();
    QSet<QString> setLoai;
    for (HangHoa* h : dsH) setLoai.insert(loaiHienThi(h->getLoai()));
    ui->comboLoai->blockSignals(true);
    ui->comboLoai->clear();
    ui->comboLoai->addItem("Tat ca loai");
    for (const QString& l : setLoai) ui->comboLoai->addItem(l);
    ui->comboLoai->blockSignals(false);
    locHangHoa();
}

void MainWindow::locHangHoa()
{
    auto dsH = shop.ds.toVector();
    QString tuKhoa   = ui->lineEditTimKiem->text().trimmed().toLower();
    QString loaiChon = ui->comboLoai->currentIndex() > 0
                           ? ui->comboLoai->currentText() : "";
    vector<HangHoa*> ds;
    for (HangHoa* h : dsH) {
        bool okTK   = tuKhoa.isEmpty() ||
                    QString::fromStdString(h->getTen()).toLower().contains(tuKhoa) ||
                    QString::fromStdString(h->getMa()).toLower().contains(tuKhoa);
        bool okLoai = loaiChon.isEmpty() || loaiHienThi(h->getLoai()) == loaiChon;
        if (okTK && okLoai) ds.push_back(h);
    }
    QTableWidget* tbl = ui->tableHangHoa;
    tbl->setRowCount((int)ds.size());

    // Set column resize modes BEFORE populating data
    tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tbl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch); // Tên hàng - stretch

    for (int i=0;i<(int)ds.size();i++) {
        HangHoa* h = ds[i];
        int ngay = h->tinhSoNgayConLai();
        auto cell = [&](int col, const QString& text,
                        Qt::Alignment align=Qt::AlignCenter) {
            auto* it = new QTableWidgetItem(text);
            it->setTextAlignment(align);
            tbl->setItem(i, col, it);
        };
        cell(0, QString::fromStdString(h->getMa()));
        cell(1, QString::fromStdString(h->getTen()), Qt::AlignVCenter|Qt::AlignLeft);
        cell(2, loaiHienThi(h->getLoai()));
        cell(3, formatTien((long long)h->getGiaNhap()));      // Giá gốc
        cell(4, formatTien((long long)h->getGiaNiemYet()));   // Giá bán = Giá niêm yết
        cell(5, QString::number(h->getSoLuong()));
        cell(6, QString::fromStdString(h->getNgayHetHan()));
        cell(7, ngay>=0 ? QString::number(ngay) : "Het han");
        auto* itTT = new QTableWidgetItem(trangThaiHang(h));
        QColor mau = mauTrangThai(h);
        itTT->setTextAlignment(Qt::AlignCenter);
        itTT->setForeground(mau);
        QFont f=itTT->font(); f.setBold(true); itTT->setFont(f);
        tbl->setItem(i,8,itTT);
    }

    // Set compact fixed widths for specific columns to give more space to product name
    tbl->setColumnWidth(0, 70);   // Mã - compact
    tbl->setColumnWidth(1, 200);  // Tên hàng - wider to show full names
    tbl->setColumnWidth(2, 100);  // Loại - compact
    tbl->setColumnWidth(3, 95);   // Giá gốc - compact
    tbl->setColumnWidth(4, 95);   // Giá bán - compact
    tbl->setColumnWidth(5, 60);   // Tồn kho - compact
    tbl->setColumnWidth(6, 90);   // Ngày hết hạn - compact
    tbl->setColumnWidth(7, 75);   // Còn (ngày) - compact
    tbl->setColumnWidth(8, 100);  // Trạng thái - compact

    ui->labelSoDong->setText(QString("Tong: %1 mat hang").arg(ds.size()));
}

void MainWindow::onTimKiemHangHoa(const QString&) { locHangHoa(); }
void MainWindow::onLocLoai(int)                    { locHangHoa(); }

void MainWindow::hienThiCanhBaoKho()
{
    auto dsH = shop.ds.toVector();
    QStringList canhBao;

    // Kiểm tra hàng sắp hết hạn (≤10 ngày)
    for (HangHoa* h : dsH) {
        int ngayCon = h->tinhSoNgayConLai();
        if (ngayCon >= 0 && ngayCon <= 10) {
            canhBao << QString("⚠️ <b>%1</b> sắp hết hạn (còn %2 ngày)")
                        .arg(QString::fromStdString(h->getTen()))
                        .arg(ngayCon);
        }
    }

    // Kiểm tra tồn kho thấp (≤5)
    for (HangHoa* h : dsH) {
        if (h->getSoLuong() <= 5 && h->getSoLuong() > 0) {
            canhBao << QString("📦 <b>%1</b> tồn kho thấp (còn %2 %3)")
                        .arg(QString::fromStdString(h->getTen()))
                        .arg(h->getSoLuong())
                        .arg(QString::fromStdString(h->getDvt()));
        }
    }

    // Hiển thị cảnh báo
    if (canhBao.isEmpty()) {
        ui->labelCanhBaoKho->setVisible(false);
    } else {
        QString html = "<div style='background-color: #FFF3CD; border: 1px solid #FFC107; "
                       "border-radius: 4px; padding: 12px; margin: 4px 0;'>"
                       "<span style='color: #856404; font-size: 13px;'>" +
                       canhBao.join("<br>") +
                       "</span></div>";
        ui->labelCanhBaoKho->setText(html);
        ui->labelCanhBaoKho->setVisible(true);
    }
}

void MainWindow::onXoaHang()
{
    if (!shop.coQuyen("QuanLyKho")) {
        QMessageBox::warning(this, "Khong co quyen", "Ban khong co quyen quan ly kho.");
        return;
    }

    // Chỉ Admin mới được xóa
    if (shop.nhanVienHienTai->getChucVu() != "Admin") {
        QMessageBox::warning(this, "Khong co quyen", "Chi Admin moi duoc xoa hang hoa.");
        return;
    }

    QTableWidget* tbl = ui->tableHangHoa;
    int row = tbl->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Chua chon", "Vui long chon mot mat hang de xoa.");
        return;
    }

    QString ma = tbl->item(row, 0)->text();
    QString ten = tbl->item(row, 1)->text();

    // Xác nhận lần 1
    if (QMessageBox::question(this, "Xac nhan xoa",
                              QString("Ban co chac muon xoa hang '%1' (ma: %2)?").arg(ten).arg(ma),
                              QMessageBox::Yes|QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    // Xác nhận lần 2
    if (QMessageBox::question(this, "XAC NHAN LAN 2",
                              QString("Day la xac nhan lan 2!\nBan THAT SU muon xoa hang '%1'?\nHanh dong nay KHONG THE HOAN TAC!").arg(ten),
                              QMessageBox::Yes|QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    if (shop.ds.xoaMotNodeTheoMa(ma.toStdString())) {
        shop.ghiFile("data.txt");
        loadHangHoa();
        loadDashboard();
        QMessageBox::information(this, "Thanh cong", QString("Da xoa hang '%1'.").arg(ten));
    } else {
        QMessageBox::critical(this, "Loi", "Khong the xoa hang hoa.");
    }
}

void MainWindow::onHuyHangHetHan()
{
    if (!shop.coQuyen("QuanLyKho")) {
        QMessageBox::warning(this, "Khong co quyen", "Ban khong co quyen quan ly kho.");
        return;
    }

    // Lọc hàng hết hạn
    auto dsH = shop.ds.toVector();
    vector<HangHoa*> hangHetHan;
    for (HangHoa* h : dsH) {
        if (h->tinhSoNgayConLai() <= 0) {
            hangHetHan.push_back(h);
        }
    }

    if (hangHetHan.empty()) {
        QMessageBox::information(this, "Thong bao", "Khong co hang hoa nao het han.");
        return;
    }

    // Tạo dialog hiển thị danh sách hàng hết hạn
    QDialog dlg(this);
    dlg.setWindowTitle("Huy hang het han");
    dlg.setMinimumWidth(700);

    QVBoxLayout* layout = new QVBoxLayout(&dlg);

    QLabel* lblTitle = new QLabel(QString("Tim thay %1 mat hang het han:").arg(hangHetHan.size()));
    QFont font = lblTitle->font();
    font.setBold(true);
    font.setPointSize(10);
    lblTitle->setFont(font);
    layout->addWidget(lblTitle);

    // Bảng hiển thị hàng hết hạn
    QTableWidget* table = new QTableWidget();
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"Ma hang", "Ten hang", "Loai", "So luong", "Ngay het han", "So ngay qua han"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setRowCount((int)hangHetHan.size());

    for (int i = 0; i < (int)hangHetHan.size(); i++) {
        HangHoa* h = hangHetHan[i];
        int ngayQuaHan = -h->tinhSoNgayConLai();

        auto cell = [&](int col, const QString& text) {
            auto* it = new QTableWidgetItem(text);
            it->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, col, it);
        };

        cell(0, QString::fromStdString(h->getMa()));
        auto* itTen = new QTableWidgetItem(QString::fromStdString(h->getTen()));
        itTen->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        table->setItem(i, 1, itTen);
        cell(2, loaiHienThi(h->getLoai()));
        cell(3, QString::number(h->getSoLuong()) + " " + QString::fromStdString(h->getDvt()));
        cell(4, QString::fromStdString(h->getNgayHetHan()));
        cell(5, QString::number(ngayQuaHan) + " ngay");

        // Tô màu đỏ cho hàng quá hạn
        QColor bgRed(255, 235, 238);
        for (int c = 0; c < 6; c++) {
            if (table->item(i, c)) {
                table->item(i, c)->setBackground(bgRed);
            }
        }
    }

    table->resizeColumnsToContents();
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    layout->addWidget(table);

    QLabel* lblNote = new QLabel("Chon mot mat hang va nhan OK de huy. Hanh dong nay se ghi vao lich su huy.");
    lblNote->setStyleSheet("color: #666; font-style: italic;");
    layout->addWidget(lblNote);

    // Buttons
    QDialogButtonBox* btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btnBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    layout->addWidget(btnBox);

    if (dlg.exec() != QDialog::Accepted) return;

    // Lấy hàng được chọn
    int row = table->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Chua chon", "Vui long chon mot mat hang de huy.");
        return;
    }

    HangHoa* hangCanHuy = hangHetHan[row];
    QString ma = QString::fromStdString(hangCanHuy->getMa());
    QString ten = QString::fromStdString(hangCanHuy->getTen());

    // Xác nhận lần 1
    if (QMessageBox::question(this, "Xac nhan huy",
                              QString("Ban co chac muon huy hang '%1' (ma: %2)?\nLy do: Het han su dung").arg(ten).arg(ma),
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    // Xác nhận lần 2
    if (QMessageBox::question(this, "XAC NHAN LAN 2",
                              QString("Day la xac nhan lan 2!\nBan THAT SU muon huy hang '%1'?\nHanh dong nay se ghi vao lich su huy va KHONG THE HOAN TAC!").arg(ten),
                              QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    // Ghi vào lichsuhuy.txt
    // Format: thoiGian|maHang|tenHang|keHang|ngayHetHan|soLuong|lyDo|loaiHang
    time_t now = time(nullptr);
    string thoiGian = ctime(&now);
    if (!thoiGian.empty() && thoiGian.back() == '\n') {
        thoiGian.pop_back();
    }

    ofstream file("lichsuhuy.txt", ios::app);
    if (file.is_open()) {
        file << thoiGian << "|"
             << hangCanHuy->getMa() << "|"
             << hangCanHuy->getTen() << "|"
             << hangCanHuy->getKeHang() << "|"
             << hangCanHuy->getNgayHetHan() << "|"
             << hangCanHuy->getSoLuong() << "|"
             << "Het han su dung|"
             << hangCanHuy->getLoai() << "\n";
        file.close();
    }

    // Xóa khỏi kho
    if (shop.ds.xoaMotNodeTheoMa(ma.toStdString())) {
        shop.ghiFile("data.txt");
        loadHangHoa();
        loadDashboard();
        hienThiCanhBaoKho();
        QMessageBox::information(this, "Thanh cong",
                                QString("Da huy hang '%1'.\nThong tin da duoc ghi vao lich su huy.").arg(ten));
    } else {
        QMessageBox::critical(this, "Loi", "Khong the huy hang hoa.");
    }
}

void MainWindow::onSapXepKho()
{
    if (!shop.coQuyen("QuanLyKho")) {
        QMessageBox::warning(this, "Khong co quyen", "Ban khong co quyen quan ly kho.");
        return;
    }

    // Tạo dialog chọn tiêu chí sắp xếp
    QDialog dlg(this);
    dlg.setWindowTitle("Sap xep kho");
    dlg.setMinimumWidth(450);

    QVBoxLayout* layout = new QVBoxLayout(&dlg);
    layout->setSpacing(15);

    QLabel* lblTitle = new QLabel("Chon tieu chi sap xep:");
    QFont font = lblTitle->font();
    font.setBold(true);
    font.setPointSize(10);
    lblTitle->setFont(font);
    layout->addWidget(lblTitle);

    // Tạo button group để quản lý radio buttons
    QButtonGroup* btnGroup = new QButtonGroup(&dlg);

    QRadioButton* rbHanSD = new QRadioButton("Sap xep theo han su dung (gan het han truoc)");
    QRadioButton* rbGiaNhap = new QRadioButton("Sap xep theo gia nhap (thap den cao)");
    QRadioButton* rbSoLuong = new QRadioButton("Sap xep theo so luong ton kho (it den nhieu)");
    QRadioButton* rbTen = new QRadioButton("Sap xep theo ten hang (A-Z)");

    // Style cho radio buttons để dễ nhìn hơn
    QString radioStyle =
        "QRadioButton { "
        "   padding: 8px; "
        "   spacing: 10px; "
        "} "
        "QRadioButton::indicator { "
        "   width: 18px; "
        "   height: 18px; "
        "} "
        "QRadioButton::indicator::unchecked { "
        "   border: 2px solid #999; "
        "   border-radius: 9px; "
        "   background: white; "
        "} "
        "QRadioButton::indicator::checked { "
        "   border: 2px solid #1A237E; "
        "   border-radius: 9px; "
        "   background: #1A237E; "
        "} "
        "QRadioButton::indicator::checked::after { "
        "   content: ''; "
        "   width: 8px; "
        "   height: 8px; "
        "   border-radius: 4px; "
        "   background: white; "
        "} "
        "QRadioButton:hover { "
        "   background: #F5F7FA; "
        "   border-radius: 4px; "
        "}";

    rbHanSD->setStyleSheet(radioStyle);
    rbGiaNhap->setStyleSheet(radioStyle);
    rbSoLuong->setStyleSheet(radioStyle);
    rbTen->setStyleSheet(radioStyle);

    btnGroup->addButton(rbHanSD, 1);
    btnGroup->addButton(rbGiaNhap, 2);
    btnGroup->addButton(rbSoLuong, 3);
    btnGroup->addButton(rbTen, 4);

    rbHanSD->setChecked(true);

    layout->addWidget(rbHanSD);
    layout->addWidget(rbGiaNhap);
    layout->addWidget(rbSoLuong);
    layout->addWidget(rbTen);

    layout->addSpacing(10);

    QDialogButtonBox* btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    btnBox->button(QDialogButtonBox::Ok)->setText("Sap xep");
    btnBox->button(QDialogButtonBox::Cancel)->setText("Huy");
    connect(btnBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    layout->addWidget(btnBox);

    if (dlg.exec() != QDialog::Accepted) return;

    // Xác định tiêu chí sắp xếp
    int tieuChi = 1; // 1=hạn SD, 2=giá nhập, 3=số lượng, 4=tên
    QString tenTieuChi;

    if (rbHanSD->isChecked()) {
        tieuChi = 1;
        tenTieuChi = "han su dung";
    } else if (rbGiaNhap->isChecked()) {
        tieuChi = 2;
        tenTieuChi = "gia nhap";
    } else if (rbSoLuong->isChecked()) {
        tieuChi = 3;
        tenTieuChi = "so luong ton kho";
    } else {
        tieuChi = 4;
        tenTieuChi = "ten hang";
    }

    // Sử dụng method sapXep có sẵn trong DanhSachHangHoa
    shop.ds.sapXep(tieuChi, true); // true = tăng dần

    shop.ghiFile("data.txt");
    loadHangHoa();

    QMessageBox::information(this, "Thanh cong",
                            QString("Da sap xep kho theo %1.").arg(tenTieuChi));
}

void MainWindow::onTaoDanhSachTheoDoi()
{
    if (!shop.coQuyen("QuanLyKho")) {
        QMessageBox::warning(this, "Khong co quyen", "Ban khong co quyen quan ly kho.");
        return;
    }

    // Lọc hàng có hạn sử dụng < 20 ngày
    auto dsH = shop.ds.toVector();
    vector<HangHoa*> hangCanTheoDoi;
    for (HangHoa* h : dsH) {
        int ngayCon = h->tinhSoNgayConLai();
        if (ngayCon >= 0 && ngayCon < 20) {
            hangCanTheoDoi.push_back(h);
        }
    }

    if (hangCanTheoDoi.empty()) {
        QMessageBox::information(this, "Thong bao",
                                "Khong co hang hoa nao can theo doi (han su dung < 20 ngay).");
        return;
    }

    // Tạo dialog cho phép chọn nhiều mặt hàng
    QDialog dlg(this);
    dlg.setWindowTitle("Tao danh sach theo doi");
    dlg.setMinimumWidth(800);
    dlg.setMinimumHeight(500);

    QVBoxLayout* layout = new QVBoxLayout(&dlg);

    QLabel* lblTitle = new QLabel(QString("Tim thay %1 mat hang can theo doi (han su dung < 20 ngay):").arg(hangCanTheoDoi.size()));
    QFont font = lblTitle->font();
    font.setBold(true);
    font.setPointSize(10);
    lblTitle->setFont(font);
    layout->addWidget(lblTitle);

    QLabel* lblNote = new QLabel("Chon cac mat hang muon them vao danh sach theo doi:");
    lblNote->setStyleSheet("color: #666; font-style: italic;");
    layout->addWidget(lblNote);

    // Bảng hiển thị với checkbox
    QTableWidget* table = new QTableWidget();
    table->setColumnCount(7);
    table->setHorizontalHeaderLabels({"Chon", "Ma hang", "Ten hang", "Loai", "So luong", "Ngay het han", "Con lai"});
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::MultiSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setRowCount((int)hangCanTheoDoi.size());

    for (int i = 0; i < (int)hangCanTheoDoi.size(); i++) {
        HangHoa* h = hangCanTheoDoi[i];
        int ngayCon = h->tinhSoNgayConLai();

        // Checkbox
        auto* chk = new QTableWidgetItem();
        chk->setCheckState(Qt::Checked); // Mặc định chọn tất cả
        chk->setTextAlignment(Qt::AlignCenter);
        table->setItem(i, 0, chk);

        auto cell = [&](int col, const QString& text) {
            auto* it = new QTableWidgetItem(text);
            it->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, col, it);
        };

        cell(1, QString::fromStdString(h->getMa()));
        auto* itTen = new QTableWidgetItem(QString::fromStdString(h->getTen()));
        itTen->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        table->setItem(i, 2, itTen);
        cell(3, loaiHienThi(h->getLoai()));
        cell(4, QString::number(h->getSoLuong()) + " " + QString::fromStdString(h->getDvt()));
        cell(5, QString::fromStdString(h->getNgayHetHan()));
        cell(6, QString::number(ngayCon) + " ngay");

        // Tô màu theo mức độ cảnh báo
        QColor bg;
        if (ngayCon <= 7) bg = QColor(255, 235, 238); // Đỏ nhạt
        else if (ngayCon <= 14) bg = QColor(255, 243, 205); // Vàng nhạt
        else bg = QColor(232, 245, 233); // Xanh nhạt

        for (int c = 0; c < 7; c++) {
            if (table->item(i, c)) {
                table->item(i, c)->setBackground(bg);
            }
        }
    }

    table->resizeColumnsToContents();
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    layout->addWidget(table);

    // Buttons chọn/bỏ chọn tất cả
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnChonTatCa = new QPushButton("Chon tat ca");
    QPushButton* btnBoChonTatCa = new QPushButton("Bo chon tat ca");
    btnLayout->addWidget(btnChonTatCa);
    btnLayout->addWidget(btnBoChonTatCa);
    btnLayout->addStretch();
    layout->addLayout(btnLayout);

    connect(btnChonTatCa, &QPushButton::clicked, [table]() {
        for (int i = 0; i < table->rowCount(); i++) {
            table->item(i, 0)->setCheckState(Qt::Checked);
        }
    });

    connect(btnBoChonTatCa, &QPushButton::clicked, [table]() {
        for (int i = 0; i < table->rowCount(); i++) {
            table->item(i, 0)->setCheckState(Qt::Unchecked);
        }
    });

    QDialogButtonBox* btnBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btnBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btnBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    layout->addWidget(btnBox);

    if (dlg.exec() != QDialog::Accepted) return;

    // Thu thập các mặt hàng được chọn
    vector<HangHoa*> hangDuocChon;
    for (int i = 0; i < table->rowCount(); i++) {
        if (table->item(i, 0)->checkState() == Qt::Checked) {
            hangDuocChon.push_back(hangCanTheoDoi[i]);
        }
    }

    if (hangDuocChon.empty()) {
        QMessageBox::information(this, "Thong bao", "Ban chua chon mat hang nao.");
        return;
    }

    // Ghi vào danhsachtheodoi.txt
    // Format: maHang|tenHang|loaiHang|soLuong|ngayHetHan|soNgayConLai
    ofstream file("danhsachtheodoi.txt");
    if (file.is_open()) {
        for (HangHoa* h : hangDuocChon) {
            file << h->getMa() << "|"
                 << h->getTen() << "|"
                 << h->getLoai() << "|"
                 << h->getSoLuong() << "|"
                 << h->getNgayHetHan() << "|"
                 << h->tinhSoNgayConLai() << "\n";
        }
        file.close();

        QMessageBox::information(this, "Thanh cong",
                                QString("Da tao danh sach theo doi voi %1 mat hang.\nFile: danhsachtheodoi.txt")
                                .arg(hangDuocChon.size()));
    } else {
        QMessageBox::critical(this, "Loi", "Khong the ghi file danhsachtheodoi.txt");
    }
}

void MainWindow::onThemHang()
{
    if (!shop.coQuyen("QuanLyKho")) {
        QMessageBox::warning(this, "Khong co quyen", "Ban khong co quyen quan ly kho.");
        return;
    }

    ThemHangDialog dlg(&shop, this);
    if (dlg.exec() == QDialog::Accepted) {
        HangHoa* hangMoi = dlg.getHangHoaMoi();
        if (hangMoi) {
            shop.ds.themCuoi(hangMoi);
            shop.ghiFile("data.txt");
            loadHangHoa();
            loadDashboard();
            QMessageBox::information(this, "Thanh cong",
                                     QString("Da them hang '%1' (ma: %2) vao kho.")
                                         .arg(QString::fromStdString(hangMoi->getTen()))
                                         .arg(QString::fromStdString(hangMoi->getMa())));
        }
    }
}

void MainWindow::onSuaHang()
{
    if (!shop.coQuyen("QuanLyKho")) {
        QMessageBox::warning(this, "Khong co quyen", "Ban khong co quyen quan ly kho.");
        return;
    }

    QTableWidget* tbl = ui->tableHangHoa;
    int row = tbl->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Chua chon", "Vui long chon mot mat hang de sua.");
        return;
    }

    QString ma = tbl->item(row, 0)->text();
    QString ten = tbl->item(row, 1)->text();
    HangHoa* hang = shop.ds.timTheoMa(ma.toStdString());
    if (!hang) return;

    SuaHangDialog dlg(ma, ten, hang->getGiaNiemYet(), hang->getSoLuong(),
                      QString::fromStdString(hang->getNgayHetHan()), this);
    if (dlg.exec() == QDialog::Accepted) {
        hang->setGiaNiemYet(dlg.getGiaNiemYetMoi());
        hang->setSoLuong(dlg.getSoLuongMoi());
        hang->setNgayHetHan(dlg.getNgayHetHanMoi().toStdString());
        shop.ghiFile("data.txt");
        loadHangHoa();
        loadDashboard();
        QMessageBox::information(this, "Thanh cong", QString("Da cap nhat thong tin hang '%1'.").arg(ten));
    }
}

void MainWindow::onNhapKho()
{
    if (!shop.coQuyen("QuanLyKho")) {
        QMessageBox::warning(this, "Khong co quyen", "Ban khong co quyen quan ly kho.");
        return;
    }

    QTableWidget* tbl = ui->tableHangHoa;
    int row = tbl->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Chua chon", "Vui long chon mot mat hang de nhap kho.");
        return;
    }

    QString ma = tbl->item(row, 0)->text();
    QString ten = tbl->item(row, 1)->text();
    HangHoa* hang = shop.ds.timTheoMa(ma.toStdString());
    if (!hang) return;

    NhapKhoDialog dlg(ma, ten, hang->getSoLuong(), hang->getGiaNhap(),
                      QString::fromStdString(hang->getNgayHetHan()), this);
    if (dlg.exec() == QDialog::Accepted) {
        // Tạo lô hàng mới với cùng mã nhưng có thể có giá và hạn khác
        HangHoa* loMoi = nullptr;
        string loai = hang->getLoai();

        if (loai == "DoUong") {
            loMoi = new DoUong();
        } else if (loai == "ThucPhamCheBien") {
            loMoi = new ThucPhamCheBien();
        } else if (loai == "MyPham") {
            loMoi = new MyPham();
        } else if (loai == "DoGiaDung") {
            auto* h = new DoGiaDung();
            h->setBaoHanh(dynamic_cast<DoGiaDung*>(hang)->getBaoHanh());
            loMoi = h;
        } else if (loai == "HangTuoiSong") {
            auto* h = new HangTuoiSong();
            h->setKhoiLuong(dynamic_cast<HangTuoiSong*>(hang)->getKhoiLuong());
            h->setTinhTrang(dynamic_cast<HangTuoiSong*>(hang)->getTinhTrang());
            loMoi = h;
        } else if (loai == "TraiCay") {
            auto* h = new TraiCay();
            h->setKhoiLuong(dynamic_cast<TraiCay*>(hang)->getKhoiLuong());
            h->setTinhTrang(dynamic_cast<TraiCay*>(hang)->getTinhTrang());
            loMoi = h;
        }

        if (loMoi) {
            loMoi->setMa(hang->getMa());
            loMoi->setTen(hang->getTen());
            loMoi->setDvt(hang->getDvt());
            loMoi->setKeHang(hang->getKeHang());
            loMoi->setGiaNhap(dlg.getGiaNhap());
            loMoi->setGiaNiemYet(dlg.getGiaBan());
            loMoi->setSoLuong(dlg.getSoLuongNhap());
            loMoi->setNgayHetHan(dlg.getNgayHetHan().toStdString());

            shop.ds.themCuoi(loMoi);
            shop.ghiFile("data.txt");

            // Debug: Đếm số lô cùng mã
            auto dsH = shop.ds.toVector();
            int soLoCungMa = 0;
            for (HangHoa* h : dsH) {
                if (h->getMa() == hang->getMa()) {
                    soLoCungMa++;
                }
            }

            loadHangHoa();
            loadDashboard();
            hienThiCanhBaoKho();

            QMessageBox::information(this, "NHAP KHO THANH CONG",
                                     QString("✓ Da nhap lo hang moi thanh cong!\n\n"
                                            "Ma hang: %1\n"
                                            "Ten hang: %2\n"
                                            "So luong: %3\n"
                                            "Gia nhap: %4 VND\n"
                                            "Gia ban: %5 VND\n"
                                            "Ngay het han: %6\n\n"
                                            "Tong so lo cua ma %1: %7 lo")
                                         .arg(ma)
                                         .arg(ten)
                                         .arg(dlg.getSoLuongNhap())
                                         .arg(formatTien((long long)dlg.getGiaNhap()))
                                         .arg(formatTien((long long)dlg.getGiaBan()))
                                         .arg(dlg.getNgayHetHan())
                                         .arg(soLoCungMa));
        }
    }
}

void MainWindow::loadKhachHang() { locKhachHang(""); }

void MainWindow::locKhachHang(const QString& tuKhoa)
{
    auto dsK = shop.dsKhachHang.toVector();
    QString tk = tuKhoa.trimmed().toLower();
    vector<KhachHang*> ds;
    for (KhachHang* k : dsK) {
        bool ok = tk.isEmpty() ||
                  QString::fromStdString(k->getTen()).toLower().contains(tk) ||
                  QString::fromStdString(k->getSDT()).toLower().contains(tk);
        if (ok) ds.push_back(k);
    }
    QTableWidget* tbl = ui->tableKhachHang;
    tbl->setRowCount((int)ds.size());
    for (int i=0;i<(int)ds.size();i++) {
        KhachHang* k = ds[i];
        auto cell = [&](int col, const QString& text) {
            auto* it = new QTableWidgetItem(text);
            it->setTextAlignment(Qt::AlignCenter);
            tbl->setItem(i, col, it);
        };
        cell(0, QString::fromStdString(k->getMa()));
        auto* itTen = new QTableWidgetItem(QString::fromStdString(k->getTen()));
        itTen->setTextAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        tbl->setItem(i,1,itTen);
        cell(2, QString::fromStdString(k->getSDT()));
        cell(3, QString::number(k->getDiem()));
        QString ltv = QString::fromStdString(k->getLoaiThanhVien());
        auto* itTV = new QTableWidgetItem(ltv);
        itTV->setTextAlignment(Qt::AlignCenter);
        if      (ltv=="Kim Cuong") itTV->setForeground(QColor("#00BCD4"));
        else if (ltv=="Vang")      itTV->setForeground(QColor("#F9A825"));
        else                       itTV->setForeground(QColor("#78909C"));
        QFont f=itTV->font(); f.setBold(true); itTV->setFont(f);
        tbl->setItem(i,4,itTV);
    }
    tbl->resizeColumnsToContents();
    tbl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->labelSoKH->setText(QString("Tong: %1 khach hang").arg(ds.size()));
}

void MainWindow::onTimKiemKhachHang(const QString& text) { locKhachHang(text); }

void MainWindow::loadDanhSachHangBH(const QString& tuKhoa)
{
    auto dsH = shop.ds.toVector();
    QString tk = tuKhoa.trimmed().toLower();
    vector<HangHoa*> ds;
    for (HangHoa* h : dsH) {
        if (h->tinhSoNgayConLai() <= 0) continue;
        if (h->getSoLuong() <= 0)       continue;
        bool ok = tk.isEmpty() ||
                  QString::fromStdString(h->getTen()).toLower().contains(tk) ||
                  QString::fromStdString(h->getMa()).toLower().contains(tk);
        if (ok) ds.push_back(h);
    }
    QTableWidget* tbl = ui->tableDanhSachHang;
    tbl->setRowCount((int)ds.size());
    tbl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    for (int i=0;i<(int)ds.size();i++) {
        HangHoa* h = ds[i];

        // Hiển thị giá gốc và giá bán
        float giaNiemYet = h->getGiaNiemYet();

        auto cell = [&](int col, const QString& text) {
            auto* it = new QTableWidgetItem(text);
            it->setTextAlignment(Qt::AlignCenter);
            tbl->setItem(i, col, it);
        };
        cell(0, QString::fromStdString(h->getMa()));
        auto* itTen = new QTableWidgetItem(QString::fromStdString(h->getTen()));
        itTen->setTextAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        tbl->setItem(i,1,itTen);
        cell(2, formatTien((long long)giaNiemYet));          // Giá bán = Giá niêm yết
        cell(3, QString::number(h->getSoLuong()));           // Tồn
        int ngay = h->tinhSoNgayConLai();
        if (ngay <= 7) {
            QColor bg = (ngay<=3) ? QColor(255,235,238) : QColor(255,243,224);
            for (int c=0;c<4;c++) if(tbl->item(i,c)) tbl->item(i,c)->setBackground(bg);
        }
    }
    tbl->resizeColumnsToContents();
    tbl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void MainWindow::onTimHangBanHang(const QString& text) { loadDanhSachHangBH(text); }

void MainWindow::onThemVaoGio()
{
    QTableWidget* tbl = ui->tableDanhSachHang;
    int row = tbl->currentRow();
    if (row < 0) {
        QMessageBox::information(this, "Chua chon", "Vui long chon mot mat hang truoc.");
        return;
    }
    QString ma = tbl->item(row, 0)->text();
    int     sl = ui->spinSoLuong->value();
    HangHoa* hang = shop.ds.timTheoMa(ma.toStdString());
    if (!hang) return;
    int daCoTrongGio = gioHang.value(ma, 0);
    if (daCoTrongGio + sl > hang->getSoLuong()) {
        QMessageBox::warning(this, "Khong du hang",
                             QString("Ton kho: %1, da trong gio: %2, khong the them %3.")
                                 .arg(hang->getSoLuong()).arg(daCoTrongGio).arg(sl));
        return;
    }
    gioHang[ma] = daCoTrongGio + sl;
    ui->spinSoLuong->setValue(1);
    refreshGioHang();
}

void MainWindow::refreshGioHang()
{
    QTableWidget* tbl = ui->tableGioHang;
    tbl->setRowCount(0);
    if (gioHang.isEmpty()) { capNhatTongTien(); return; }

    QString sdt = ui->lineEditSDTKhach->text().trimmed();
    KhachHang* khach = sdt.isEmpty() ? nullptr
                                     : shop.dsKhachHang.timTheoSDT(sdt.toStdString());

    tbl->setRowCount(gioHang.size());
    tbl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

    int row = 0;
    for (auto it = gioHang.begin(); it != gioHang.end(); ++it, ++row) {
        QString ma = it.key();
        int     sl = it.value();
        HangHoa* h = shop.ds.timTheoMa(ma.toStdString());
        if (!h) continue;

        float giaNiemYet = h->getGiaNiemYet();
        float pctHan     = h->tinhPctGiamHetHan();
        float pctTV      = khach ? khach->apDungGiamGia() : 0.0f;
        float pctTong    = min(pctHan + pctTV, 0.40f);
        float giaSauGiam = giaNiemYet * (1.0f - pctTong);
        float thanhTien  = giaSauGiam * sl;

        auto cell = [&](int col, const QString& text,
                        Qt::Alignment align=Qt::AlignCenter) {
            auto* item = new QTableWidgetItem(text);
            item->setTextAlignment(align);
            tbl->setItem(row, col, item);
        };
        cell(0, ma);
        cell(1, QString::fromStdString(h->getTen()), Qt::AlignVCenter|Qt::AlignLeft);
        cell(2, formatTien((long long)h->getGiaNhap()));     // Giá gốc
        cell(3, pctTong > 0 ? QString("-%1%").arg((int)(pctTong*100)) : QString("-"));
        cell(4, formatTien((long long)giaNiemYet));          // Giá bán = Giá niêm yết
        cell(5, QString::number(sl));
        cell(6, formatTien((long long)thanhTien));

        auto* btnXoa = new QPushButton("X");
        btnXoa->setFixedSize(28, 28);
        btnXoa->setStyleSheet(
            "QPushButton{color:#E53935;border:none;font-weight:bold;background:transparent;}"
            "QPushButton:hover{background:#FFEBEE;border-radius:4px;}");
        QString maCapture = ma;
        connect(btnXoa, &QPushButton::clicked, this, [this, maCapture]() {
            gioHang.remove(maCapture);
            refreshGioHang();
        });
        tbl->setCellWidget(row, 7, btnXoa);
    }

    // Set column widths for cart table
    tbl->setColumnWidth(0, 80);   // Mã
    tbl->setColumnWidth(1, 200);  // Tên hàng - minimum width to show product names
    tbl->setColumnWidth(2, 90);   // Giá gốc
    tbl->setColumnWidth(3, 70);   // Giảm
    tbl->setColumnWidth(4, 90);   // Giá bán
    tbl->setColumnWidth(5, 50);   // SL
    tbl->setColumnWidth(6, 100);  // Thành tiền
    tbl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    capNhatTongTien();
}

void MainWindow::capNhatTongTien()
{
    QString sdt = ui->lineEditSDTKhach->text().trimmed();
    KhachHang* khach = sdt.isEmpty() ? nullptr
                                     : shop.dsKhachHang.timTheoSDT(sdt.toStdString());
    long long tongGoc=0, tongGiam=0;
    for (auto it = gioHang.begin(); it != gioHang.end(); ++it) {
        HangHoa* h = shop.ds.timTheoMa(it.key().toStdString());
        if (!h) continue;
        int   sl  = it.value();
        float giaNiemYet = h->getGiaNiemYet();  // Dùng giá niêm yết
        float pctHan = h->tinhPctGiamHetHan();
        float pctTV = khach ? khach->apDungGiamGia() : 0.0f;
        float pct = min(pctHan + pctTV, 0.40f);
        float giaBan = giaNiemYet * (1.0f - pct);
        tongGoc  += (long long)(giaNiemYet * sl);  // Tổng gốc = giá niêm yết × số lượng
        tongGiam += (long long)((giaNiemYet - giaBan) * sl);  // Giảm = (niêm yết - bán) × số lượng
    }
    ui->labelTongGoc      ->setText(formatTien(tongGoc));
    ui->labelTongGiam     ->setText("- " + formatTien(tongGiam));
    ui->labelTongThanhToan->setText(formatTien(tongGoc-tongGiam));
}

void MainWindow::onXoaGio()
{
    if (gioHang.isEmpty()) return;
    if (QMessageBox::question(this, "Xoa gio hang", "Ban co chac muon xoa toan bo gio hang?",
                              QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
        gioHang.clear();
        refreshGioHang();
    }
}

void MainWindow::onTimKhachHangBH()
{
    QString sdt = ui->lineEditSDTKhach->text().trimmed();
    if (sdt.isEmpty()) {
        ui->labelThongTinKhach->setText("-- Khach vang lai (khong tich diem) --");
        refreshGioHang();
        return;
    }
    KhachHang* k = shop.dsKhachHang.timTheoSDT(sdt.toStdString());
    if (k) {
        ui->labelThongTinKhach->setText(
            QString("OK: %1 - %2 - %3 diem - Giam %4%")
                .arg(QString::fromStdString(k->getTen()))
                .arg(QString::fromStdString(k->getLoaiThanhVien()))
                .arg(k->getDiem())
                .arg((int)(k->apDungGiamGia()*100)));
    } else {
        ui->labelThongTinKhach->setText("Khong tim thay khach hang voi SDT nay.");
    }
    refreshGioHang();
}

void MainWindow::onThanhToan()
{
    if (gioHang.isEmpty()) {
        QMessageBox::warning(this, "Gio trong", "Vui long them hang vao gio truoc.");
        return;
    }
    string sdtKhach = ui->lineEditSDTKhach->text().trimmed().toStdString();
    vector<pair<string,int>> dsGio;
    for (auto it = gioHang.begin(); it != gioHang.end(); ++it)
        dsGio.push_back({it.key().toStdString(), it.value()});

    CuaHang::KetQuaBanHang kq = shop.thucHienBanHang(sdtKhach, dsGio);
    if (!kq.thanhCong) {
        QMessageBox::critical(this, "Loi thanh toan",
                              QString::fromStdString(kq.loiMessage));
        return;
    }
    gioHang.clear();
    ui->lineEditSDTKhach->clear();
    ui->labelThongTinKhach->setText("-- Khach vang lai (khong tich diem) --");
    refreshGioHang();
    loadDanhSachHangBH();
    loadDashboard();
    loadBaoCao();  // Refresh báo cáo sau khi bán hàng
    hienThiHoaDon(kq);
}

void MainWindow::hienThiHoaDon(const CuaHang::KetQuaBanHang& kq)
{
    const HoaDonLichSu& hd = kq.hoaDon;

    // Format thời gian cho dễ đọc
    QString thoiGianFormatted = QString::fromStdString(hd.thoiGian).trimmed();
    // Parse "Mon May 04 23:53:56 2026" và format lại thành "04/05/2026 - 23:53:56"
    QStringList parts = thoiGianFormatted.split(' ', Qt::SkipEmptyParts);
    if (parts.size() >= 5) {
        QString day = parts[2];
        QString month = parts[1];
        QString time = parts[3];
        QString year = parts[4];

        // Convert month name to number
        QMap<QString, QString> monthMap;
        monthMap["Jan"] = "01"; monthMap["Feb"] = "02"; monthMap["Mar"] = "03";
        monthMap["Apr"] = "04"; monthMap["May"] = "05"; monthMap["Jun"] = "06";
        monthMap["Jul"] = "07"; monthMap["Aug"] = "08"; monthMap["Sep"] = "09";
        monthMap["Oct"] = "10"; monthMap["Nov"] = "11"; monthMap["Dec"] = "12";

        QString monthNum = monthMap.value(month, "01");
        thoiGianFormatted = day + "/" + monthNum + "/" + year + " - " + time;
    }

    // Build HTML content with professional styling
    QString htmlContent = R"(
<!DOCTYPE html>
<html>
<head>
<style>
body {
    font-family: 'Segoe UI', Arial, sans-serif;
    margin: 0;
    padding: 20px;
    background: #f8f9fa;
}
.invoice-container {
    max-width: 600px;
    margin: 0 auto;
    background: white;
    border-radius: 12px;
    box-shadow: 0 2px 8px rgba(0,0,0,0.1);
    overflow: hidden;
}
.invoice-header {
    background: linear-gradient(135deg, #1A237E 0%, #3F51B5 100%);
    color: white;
    padding: 30px 25px;
    text-align: center;
}
.invoice-header h1 {
    margin: 0 0 10px 0;
    font-size: 28px;
    font-weight: 600;
    letter-spacing: 0.5px;
}
.invoice-header .subtitle {
    font-size: 14px;
    opacity: 0.9;
}
.invoice-info {
    padding: 25px;
    border-bottom: 2px solid #E8EAF6;
}
.info-row {
    display: flex;
    justify-content: space-between;
    margin-bottom: 12px;
    font-size: 14px;
}
.info-label {
    color: #546E7A;
    font-weight: 500;
}
.info-value {
    color: #212121;
    font-weight: 600;
}
.items-section {
    padding: 25px;
}
.items-header {
    font-size: 16px;
    font-weight: 600;
    color: #1A237E;
    margin-bottom: 15px;
    padding-bottom: 10px;
    border-bottom: 2px solid #E8EAF6;
}
.item {
    padding: 15px 0;
    border-bottom: 1px solid #F5F5F5;
}
.item:last-child {
    border-bottom: none;
}
.item-header {
    display: flex;
    justify-content: space-between;
    margin-bottom: 8px;
}
.item-name {
    font-weight: 600;
    color: #212121;
    font-size: 15px;
}
.item-qty {
    color: #1A237E;
    font-weight: 600;
    background: #E8EAF6;
    padding: 2px 10px;
    border-radius: 12px;
    font-size: 13px;
}
.item-details {
    display: flex;
    justify-content: space-between;
    font-size: 13px;
    color: #546E7A;
}
.totals-section {
    padding: 25px;
    background: #FAFAFA;
}
.total-row {
    display: flex;
    justify-content: space-between;
    margin-bottom: 10px;
    font-size: 15px;
}
.total-label {
    color: #546E7A;
}
.total-value {
    font-weight: 600;
    color: #212121;
}
.discount-row .total-value {
    color: #E53935;
}
.final-total {
    margin-top: 15px;
    padding-top: 15px;
    border-top: 2px solid #E0E0E0;
}
.final-total .total-label {
    font-size: 18px;
    font-weight: 600;
    color: #1A237E;
}
.final-total .total-value {
    font-size: 22px;
    font-weight: 700;
    color: #2E7D32;
}
.invoice-footer {
    text-align: center;
    padding: 20px;
    background: #E8EAF6;
    color: #1A237E;
    font-size: 15px;
    font-weight: 500;
}
</style>
</head>
<body>
<div class="invoice-container">
    <div class="invoice-header">
        <h1>🛒 HOA DON BAN HANG</h1>
        <div class="subtitle">Sieu Thi - Cam on quy khach!</div>
    </div>

    <div class="invoice-info">
        <div class="info-row">
            <span class="info-label">Ma hoa don:</span>
            <span class="info-value">)";

    htmlContent += QString::fromStdString(hd.maHoaDon);
    htmlContent += R"(</span>
        </div>
        <div class="info-row">
            <span class="info-label">Thoi gian:</span>
            <span class="info-value">)";

    htmlContent += QString::fromStdString(hd.thoiGian).trimmed();
    htmlContent += R"(</span>
        </div>
        <div class="info-row">
            <span class="info-label">Khach hang:</span>
            <span class="info-value">)";

    if (!hd.tenKhachHang.empty()) {
        htmlContent += QString::fromStdString(hd.tenKhachHang) + " (" +
                      QString::fromStdString(hd.sdtKhachHang) + ")";
    } else {
        htmlContent += "Khach vang lai";
    }

    htmlContent += R"(</span>
        </div>
    </div>

    <div class="items-section">
        <div class="items-header">Chi tiet san pham</div>)";

    // Dùng dữ liệu đã lưu trong hóa đơn để không bị lệch khi giá hiện tại thay đổi
    long long tongTien = 0;
    for (const ChiTietHoaDon& ct : hd.chiTiet) {
        long long thanhTienDong = (long long)ct.thanhTien;
        tongTien += (long long)(ct.giaGoc * ct.soLuong);

        htmlContent += R"(
        <div class="item">
            <div class="item-header">
                <span class="item-name">)" + QString::fromStdString(ct.tenHang) + R"(</span>
                <span class="item-qty">x)" + QString::number(ct.soLuong) + R"(</span>
            </div>
            <div class="item-details">
                <span>Don gia: )" + formatTien((long long)ct.giaGoc) + R"(</span>
                <span style="font-weight:600; color:#212121;">)" + formatTien(thanhTienDong) + R"(</span>
            </div>
        </div>)";
    }

    long long tongGiam = tongTien - (long long)hd.tongThanhToan;

    htmlContent += R"(
    </div>

    <div class="totals-section">
        <div class="total-row">
            <span class="total-label">Tong tien:</span>
            <span class="total-value">)" + formatTien(tongTien) + R"(</span>
        </div>
        <div class="total-row discount-row">
            <span class="total-label">Giam gia:</span>
            <span class="total-value">- )" + formatTien(tongGiam) + R"(</span>
        </div>
        <div class="total-row final-total">
            <span class="total-label">THANH TOAN:</span>
            <span class="total-value">)" + formatTien((long long)hd.tongThanhToan) + R"(</span>
        </div>
    </div>

    <div class="invoice-footer">
        ✨ Cam on quy khach! Hen gap lai! ✨
    </div>
</div>
</body>
</html>)";

    QDialog* dlg = new QDialog(this);
    dlg->setWindowTitle("Hoa don - " + QString::fromStdString(hd.maHoaDon));
    dlg->resize(650, 700);

    auto* vl  = new QVBoxLayout(dlg);
    vl->setContentsMargins(0, 0, 0, 12);
    vl->setSpacing(12);

    auto* txt = new QTextEdit(dlg);
    txt->setReadOnly(true);
    txt->setHtml(htmlContent);
    txt->setStyleSheet("QTextEdit { border: none; background: #f8f9fa; }");

    auto* btnOk = new QPushButton("Dong", dlg);
    btnOk->setMinimumHeight(40);
    btnOk->setMinimumWidth(120);
    btnOk->setStyleSheet(
        "QPushButton{background:#1A237E;color:white;border-radius:8px;font-weight:bold;padding:8px 24px;font-size:14px;}"
        "QPushButton:hover{background:#3F51B5;}");
    connect(btnOk, &QPushButton::clicked, dlg, &QDialog::accept);

    vl->addWidget(txt);

    auto* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    btnLayout->addWidget(btnOk);
    btnLayout->addStretch();
    vl->addLayout(btnLayout);

    dlg->exec();
    delete dlg;
}

void MainWindow::loadBaoCao()
{
    // Tính tổng vốn tồn kho (dùng giaNhap × soLuong)
    float tongVon = shop.ds.tinhTongVon();
    ui->labelCardVon_So->setText(formatTien((long long)tongVon));

    // Số hóa đơn
    int soHoaDon = (int)shop.lichSuHoaDon.size();
    ui->labelCardHD_So->setText(QString::number(soHoaDon));

    // Tổng doanh thu (tổng thanh toán từ tất cả hóa đơn)
    long long tongDoanhThu = 0;
    for (const HoaDonLichSu& hd : shop.lichSuHoaDon) {
        tongDoanhThu += (long long)hd.tongThanhToan;
    }
    ui->labelCardDT_So->setText(formatTien(tongDoanhThu));

    // Load lịch sử hóa đơn - wrap in try-catch để tránh crash
    try {
        loadLichSuHoaDon();
    } catch (const exception& e) {
        qDebug() << "Loi khi load lich su hoa don:" << e.what();
        ui->tableLichSuHD->setRowCount(0);
    }
}

void MainWindow::loadLichSuHoaDon()
{
    QTableWidget* tbl = ui->tableLichSuHD;

    // Sử dụng dữ liệu đã load từ shop.lichSuHoaDon
    const vector<HoaDonLichSu>& dsHoaDon = shop.lichSuHoaDon;

    // Hiển thị vào bảng
    tbl->setRowCount((int)dsHoaDon.size());
    tbl->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);

    for (int i = 0; i < (int)dsHoaDon.size(); i++) {
        const HoaDonLichSu& hd = dsHoaDon[i];

        QString thoiGian = QString::fromStdString(hd.thoiGian).trimmed();
        QStringList parts = thoiGian.split(' ', Qt::SkipEmptyParts);
        if (parts.size() >= 5) {
            QString day = parts[2];
            QString month = parts[1];
            QString time = parts[3];
            QString year = parts[4];
            QMap<QString, QString> monthMap;
            monthMap["Jan"] = "01"; monthMap["Feb"] = "02"; monthMap["Mar"] = "03";
            monthMap["Apr"] = "04"; monthMap["May"] = "05"; monthMap["Jun"] = "06";
            monthMap["Jul"] = "07"; monthMap["Aug"] = "08"; monthMap["Sep"] = "09";
            monthMap["Oct"] = "10"; monthMap["Nov"] = "11"; monthMap["Dec"] = "12";
            QString monthNum = monthMap.value(month, "01");
            thoiGian = day + "/" + monthNum + "/" + year + " " + time;
        }

        auto cell = [&](int col, const QString& text, Qt::Alignment align) {
            auto* it = new QTableWidgetItem(text);
            it->setTextAlignment(align);
            tbl->setItem(i, col, it);
        };

        cell(0, QString::fromStdString(hd.maHoaDon), Qt::AlignCenter);
        cell(1, thoiGian, Qt::AlignCenter);
        cell(2, hd.tenKhachHang.empty() ? "Khach vang lai" : QString::fromStdString(hd.tenKhachHang), Qt::AlignVCenter | Qt::AlignLeft);
        cell(3, QString::fromStdString(hd.sdtKhachHang), Qt::AlignCenter);
        cell(4, formatTien((long long)hd.tongGoc), Qt::AlignVCenter | Qt::AlignRight);
        cell(5, formatTien((long long)hd.tongGiam), Qt::AlignVCenter | Qt::AlignRight);
        cell(6, formatTien((long long)hd.tongThanhToan), Qt::AlignVCenter | Qt::AlignRight);
    }

    tbl->setColumnWidth(0, 110);
    tbl->setColumnWidth(1, 170);
    tbl->setColumnWidth(2, 220);
    tbl->setColumnWidth(3, 120);
    tbl->setColumnWidth(4, 130);
    tbl->setColumnWidth(5, 130);

    // Thêm double-click handler để xem chi tiết hóa đơn
    connect(tbl, &QTableWidget::cellDoubleClicked, this, [this](int row, int col) {
        Q_UNUSED(col);
        if (row < 0 || row >= (int)shop.lichSuHoaDon.size()) return;

        const HoaDonLichSu& hd = shop.lichSuHoaDon[row];

        // Wrap HoaDonLichSu vào KetQuaBanHang để truyền vào hienThiHoaDon
        CuaHang::KetQuaBanHang kq;
        kq.thanhCong = true;
        kq.hoaDon = hd;
        hienThiHoaDon(kq);
    });
    tbl->setColumnWidth(6, 130);
    tbl->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tbl->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tbl->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tbl->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    tbl->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    tbl->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    tbl->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
}

QString MainWindow::formatTien(long long tien) const
{
    return QLocale(QLocale::Vietnamese).toString(tien) + " d";
}

float MainWindow::safeStof(const string& str) const
{
    if (str.empty()) return 0.0f;
    try {
        return stof(str);
    } catch (...) {
        return 0.0f;
    }
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            QString action = obj->property("cardAction").toString();

            if (action == "kho") {
                onQuanLyKho();
                return true;
            }
            else if (action == "sapHetHan") {
                auto dsH = shop.ds.toVector();
                vector<HangHoa*> dsSapHetHan;
                for (HangHoa* h : dsH) {
                    int n = h->tinhSoNgayConLai();
                    if (n >= 0 && n <= 7) {
                        dsSapHetHan.push_back(h);
                    }
                }

                QDialog* dlg = new QDialog(this);
                dlg->setWindowTitle("Hang sap het han");
                dlg->resize(900, 500);

                QVBoxLayout* layout = new QVBoxLayout(dlg);
                QLabel* lblTitle = new QLabel(QString("Co %1 hang sap het han (con lai <= 7 ngay)").arg(dsSapHetHan.size()), dlg);
                lblTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #FF9800;");
                layout->addWidget(lblTitle);

                QTableWidget* table = new QTableWidget(dlg);
                table->setColumnCount(6);
                table->setHorizontalHeaderLabels({"Ma", "Ten hang", "Loai", "Ton kho", "Ngay het han", "Trang thai"});
                table->setRowCount(dsSapHetHan.size());
                table->setAlternatingRowColors(true);
                table->setSelectionBehavior(QAbstractItemView::SelectRows);
                table->setEditTriggers(QAbstractItemView::NoEditTriggers);

                for (int i = 0; i < (int)dsSapHetHan.size(); i++) {
                    HangHoa* h = dsSapHetHan[i];
                    table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(h->getMa())));
                    table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(h->getTen())));
                    table->setItem(i, 2, new QTableWidgetItem(loaiHienThi(h->getLoai())));
                    table->setItem(i, 3, new QTableWidgetItem(QString::number(h->getSoLuong())));
                    table->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(h->getNgayHetHan())));

                    auto* itTT = new QTableWidgetItem(trangThaiHang(h));
                    QColor mau = mauTrangThai(h);
                    itTT->setForeground(mau);
                    QFont f = itTT->font(); f.setBold(true); itTT->setFont(f);
                    table->setItem(i, 5, itTT);
                }

                table->resizeColumnsToContents();
                table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
                layout->addWidget(table);

                QPushButton* btnClose = new QPushButton("Dong", dlg);
                btnClose->setStyleSheet("QPushButton{background:#757575;color:white;padding:8px 16px;border-radius:4px;}"
                                        "QPushButton:hover{background:#616161;}");
                connect(btnClose, &QPushButton::clicked, dlg, &QDialog::accept);
                layout->addWidget(btnClose, 0, Qt::AlignRight);

                dlg->exec();
                delete dlg;
                return true;
            }
            else if (action == "khachHang") {
                onBaoCao();
                onThongTinKH();
                return true;
            }
            else if (action == "sapHetKho") {
                auto dsH = shop.ds.toVector();
                vector<HangHoa*> dsSapHetKho;
                for (HangHoa* h : dsH) {
                    if (h->getSoLuong() < 10) {
                        dsSapHetKho.push_back(h);
                    }
                }

                QDialog* dlg = new QDialog(this);
                dlg->setWindowTitle("Hang sap het kho");
                dlg->resize(900, 500);

                QVBoxLayout* layout = new QVBoxLayout(dlg);
                QLabel* lblTitle = new QLabel(QString("Co %1 hang sap het kho (ton kho < 10)").arg(dsSapHetKho.size()), dlg);
                lblTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #F44336;");
                layout->addWidget(lblTitle);

                QTableWidget* table = new QTableWidget(dlg);
                table->setColumnCount(6);
                table->setHorizontalHeaderLabels({"Ma", "Ten hang", "Loai", "Ton kho", "Gia nhap", "Gia ban"});
                table->setRowCount(dsSapHetKho.size());
                table->setAlternatingRowColors(true);
                table->setSelectionBehavior(QAbstractItemView::SelectRows);
                table->setEditTriggers(QAbstractItemView::NoEditTriggers);

                for (int i = 0; i < (int)dsSapHetKho.size(); i++) {
                    HangHoa* h = dsSapHetKho[i];
                    table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(h->getMa())));
                    table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(h->getTen())));
                    table->setItem(i, 2, new QTableWidgetItem(loaiHienThi(h->getLoai())));

                    auto* itSL = new QTableWidgetItem(QString::number(h->getSoLuong()));
                    itSL->setForeground(QColor("#F44336"));
                    QFont f = itSL->font(); f.setBold(true); itSL->setFont(f);
                    table->setItem(i, 3, itSL);

                    table->setItem(i, 4, new QTableWidgetItem(formatTien(h->getGiaNhap())));
                    table->setItem(i, 5, new QTableWidgetItem(formatTien(h->getGiaNiemYet())));
                }

                table->resizeColumnsToContents();
                table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
                layout->addWidget(table);

                QPushButton* btnClose = new QPushButton("Dong", dlg);
                btnClose->setStyleSheet("QPushButton{background:#757575;color:white;padding:8px 16px;border-radius:4px;}"
                                        "QPushButton:hover{background:#616161;}");
                connect(btnClose, &QPushButton::clicked, dlg, &QDialog::accept);
                layout->addWidget(btnClose, 0, Qt::AlignRight);

                dlg->exec();
                delete dlg;
                return true;
            }
        }
    }

    return QMainWindow::eventFilter(obj, event);
}

QString MainWindow::loaiHienThi(const string& loai) const
{
    if (loai=="ThucPhamCheBien") return "Thuc pham CB";
    if (loai=="DoUong")          return "Do uong";
    if (loai=="DoGiaDung")       return "Do gia dung";
    if (loai=="MyPham")          return "My pham";
    if (loai=="HangTuoiSong")    return "Hang tuoi song";
    if (loai=="TraiCay")         return "Trai cay";
    return QString::fromStdString(loai);
}

QString MainWindow::trangThaiHang(HangHoa* h) const
{
    int ngay=h->tinhSoNgayConLai(), sl=h->getSoLuong();
    if (ngay<0)  return "Da het han";
    if (ngay<=3) return "Sap het han!";
    if (ngay<=7) return "Canh bao han";
    if (sl==0)   return "Het hang";
    if (sl<10)   return "Sap het kho";
    return             "Con hang";
}

QColor MainWindow::mauTrangThai(HangHoa* h) const
{
    int ngay=h->tinhSoNgayConLai(), sl=h->getSoLuong();
    if (ngay<0||sl==0) return QColor("#E53935");
    if (ngay<=3)        return QColor("#E53935");
    if (ngay<=7)        return QColor("#FB8C00");
    if (sl<10)          return QColor("#F9A825");
    return              QColor("#43A047");
}

// =====================================================================
// PHẦN BỔ SUNG: CÁC CHỨC NĂNG BÁO CÁO MỚI
// =====================================================================

void MainWindow::onThongTinKH()
{
    QDialog* dlg = new QDialog(this);
    dlg->setWindowTitle("Quan ly khach hang");
    dlg->resize(900, 600);

    QVBoxLayout* layout = new QVBoxLayout(dlg);

    // Tạo bảng hiển thị
    QTableWidget* table = new QTableWidget(dlg);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"Ma KH", "Ten", "SDT", "Loai TV", "Tong tien", "Diem"});
    table->setAlternatingRowColors(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Hàm load dữ liệu vào bảng
    auto loadData = [&]() {
        auto dsKH = shop.dsKhachHang.toVector();
        table->setRowCount(dsKH.size());

        for (int i = 0; i < (int)dsKH.size(); i++) {
            KhachHang* kh = dsKH[i];
            table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(kh->getMa())));
            table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(kh->getTen())));
            table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(kh->getSDT())));
            table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(kh->getLoaiThanhVien())));
            table->setItem(i, 4, new QTableWidgetItem(formatTien(kh->getTongTien())));
            table->setItem(i, 5, new QTableWidgetItem(QString::number(kh->getDiem())));
        }

        table->resizeColumnsToContents();
        table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    };

    loadData();
    layout->addWidget(table);

    // Các nút chức năng
    QHBoxLayout* btnLayout = new QHBoxLayout();

    QPushButton* btnThem = new QPushButton("Them khach hang", dlg);
    QPushButton* btnSua = new QPushButton("Sua thong tin", dlg);
    QPushButton* btnXoa = new QPushButton("Xoa khach hang", dlg);
    QPushButton* btnDong = new QPushButton("Dong", dlg);

    btnThem->setStyleSheet("QPushButton{background:#4CAF50;color:white;padding:8px 16px;border-radius:4px;font-weight:bold;}"
                           "QPushButton:hover{background:#45a049;}");
    btnSua->setStyleSheet("QPushButton{background:#2196F3;color:white;padding:8px 16px;border-radius:4px;font-weight:bold;}"
                          "QPushButton:hover{background:#0b7dda;}");
    btnXoa->setStyleSheet("QPushButton{background:#f44336;color:white;padding:8px 16px;border-radius:4px;font-weight:bold;}"
                          "QPushButton:hover{background:#da190b;}");
    btnDong->setStyleSheet("QPushButton{background:#757575;color:white;padding:8px 16px;border-radius:4px;}"
                           "QPushButton:hover{background:#616161;}");

    btnLayout->addWidget(btnThem);
    btnLayout->addWidget(btnSua);
    btnLayout->addWidget(btnXoa);
    btnLayout->addStretch();
    btnLayout->addWidget(btnDong);
    layout->addLayout(btnLayout);

    // Xử lý nút Thêm
    connect(btnThem, &QPushButton::clicked, [&]() {
        QDialog* addDlg = new QDialog(dlg);
        addDlg->setWindowTitle("Them khach hang moi");
        addDlg->setMinimumWidth(450);

        QVBoxLayout* addLayout = new QVBoxLayout(addDlg);
        QFormLayout* formLayout = new QFormLayout();

        QLineEdit* editTen = new QLineEdit(addDlg);
        QLineEdit* editSDT = new QLineEdit(addDlg);

        formLayout->addRow("Ten khach hang:", editTen);
        formLayout->addRow("So dien thoai:", editSDT);

        addLayout->addLayout(formLayout);

        QHBoxLayout* btnAddLayout = new QHBoxLayout();
        QPushButton* btnOK = new QPushButton("Them", addDlg);
        QPushButton* btnCancel = new QPushButton("Huy", addDlg);

        btnOK->setStyleSheet("QPushButton{background:#4CAF50;color:white;padding:8px 16px;border-radius:4px;}"
                             "QPushButton:hover{background:#45a049;}");
        btnCancel->setStyleSheet("QPushButton{background:#757575;color:white;padding:8px 16px;border-radius:4px;}"
                                 "QPushButton:hover{background:#616161;}");

        connect(btnOK, &QPushButton::clicked, [=]() {
            QString ten = editTen->text().trimmed();
            QString sdt = editSDT->text().trimmed();

            if (ten.isEmpty() || sdt.isEmpty()) {
                QMessageBox::warning(addDlg, "Canh bao", "Vui long nhap day du thong tin!");
                return;
            }

            // Kiểm tra SDT đã tồn tại chưa
            if (shop.dsKhachHang.timTheoSDT(sdt.toStdString())) {
                QMessageBox::warning(addDlg, "Canh bao", "So dien thoai nay da ton tai!");
                return;
            }

            // Tạo mã khách hàng tự động
            time_t now = time(nullptr);
            string maKH = "KH" + to_string(now % 100000000);

            // Tạo khách hàng mới
            KhachHang* kh = new KhachHang();
            kh->setMa(maKH);
            kh->setTen(ten.toStdString());
            kh->setSDT(sdt.toStdString());
            kh->setLoaiThanhVien("Thuong");
            kh->setTongTien(0);
            kh->setDiem(0);

            shop.dsKhachHang.themCuoi(kh);
            shop.ghiKhachHang("khachhang.txt");

            loadData();
            QMessageBox::information(addDlg, "Thanh cong", "Da them khach hang moi!");
            addDlg->accept();
        });

        connect(btnCancel, &QPushButton::clicked, addDlg, &QDialog::reject);

        btnAddLayout->addWidget(btnOK);
        btnAddLayout->addWidget(btnCancel);
        addLayout->addLayout(btnAddLayout);

        addDlg->exec();
        delete addDlg;
    });

    // Xử lý nút Sửa
    connect(btnSua, &QPushButton::clicked, [&]() {
        int row = table->currentRow();
        if (row < 0) {
            QMessageBox::warning(dlg, "Canh bao", "Vui long chon khach hang can sua!");
            return;
        }

        QString sdtCu = table->item(row, 2)->text();
        KhachHang* kh = shop.dsKhachHang.timTheoSDT(sdtCu.toStdString());
        if (!kh) {
            QMessageBox::warning(dlg, "Loi", "Khong tim thay khach hang!");
            return;
        }

        QDialog* editDlg = new QDialog(dlg);
        editDlg->setWindowTitle("Sua thong tin khach hang");
        editDlg->setMinimumWidth(450);

        QVBoxLayout* editLayout = new QVBoxLayout(editDlg);
        QFormLayout* formLayout = new QFormLayout();

        QLineEdit* editTen = new QLineEdit(QString::fromStdString(kh->getTen()), editDlg);
        QLineEdit* editSDT = new QLineEdit(QString::fromStdString(kh->getSDT()), editDlg);

        formLayout->addRow("Ten khach hang:", editTen);
        formLayout->addRow("So dien thoai:", editSDT);

        editLayout->addLayout(formLayout);

        QHBoxLayout* btnEditLayout = new QHBoxLayout();
        QPushButton* btnOK = new QPushButton("Luu", editDlg);
        QPushButton* btnCancel = new QPushButton("Huy", editDlg);

        btnOK->setStyleSheet("QPushButton{background:#2196F3;color:white;padding:8px 16px;border-radius:4px;}"
                             "QPushButton:hover{background:#0b7dda;}");
        btnCancel->setStyleSheet("QPushButton{background:#757575;color:white;padding:8px 16px;border-radius:4px;}"
                                 "QPushButton:hover{background:#616161;}");

        connect(btnOK, &QPushButton::clicked, [=]() {
            QString ten = editTen->text().trimmed();
            QString sdt = editSDT->text().trimmed();

            if (ten.isEmpty() || sdt.isEmpty()) {
                QMessageBox::warning(editDlg, "Canh bao", "Vui long nhap day du thong tin!");
                return;
            }

            // Kiểm tra SDT mới có trùng với khách hàng khác không
            if (sdt.toStdString() != sdtCu.toStdString()) {
                if (shop.dsKhachHang.timTheoSDT(sdt.toStdString())) {
                    QMessageBox::warning(editDlg, "Canh bao", "So dien thoai nay da ton tai!");
                    return;
                }
            }

            // Cập nhật thông tin
            kh->setTen(ten.toStdString());
            kh->setSDT(sdt.toStdString());

            shop.ghiKhachHang("khachhang.txt");
            loadData();
            QMessageBox::information(editDlg, "Thanh cong", "Da cap nhat thong tin khach hang!");
            editDlg->accept();
        });

        connect(btnCancel, &QPushButton::clicked, editDlg, &QDialog::reject);

        btnEditLayout->addWidget(btnOK);
        btnEditLayout->addWidget(btnCancel);
        editLayout->addLayout(btnEditLayout);

        editDlg->exec();
        delete editDlg;
    });

    // Xử lý nút Xóa
    connect(btnXoa, &QPushButton::clicked, [&]() {
        int row = table->currentRow();
        if (row < 0) {
            QMessageBox::warning(dlg, "Canh bao", "Vui long chon khach hang can xoa!");
            return;
        }

        QString sdt = table->item(row, 2)->text();
        QString ten = table->item(row, 1)->text();

        auto reply = QMessageBox::question(dlg, "Xac nhan xoa",
                                          QString("Ban co chac muon xoa khach hang '%1' (SDT: %2)?").arg(ten).arg(sdt),
                                          QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            KhachHang* kh = shop.dsKhachHang.timTheoSDT(sdt.toStdString());
            if (kh) {
                string maKH = kh->getMa();
                shop.dsKhachHang.xoaTheoMa(maKH);
                shop.ghiKhachHang("khachhang.txt");
                loadData();
                QMessageBox::information(dlg, "Thanh cong", "Da xoa khach hang!");
            }
        }
    });

    connect(btnDong, &QPushButton::clicked, dlg, &QDialog::accept);

    dlg->exec();
    delete dlg;
}

void MainWindow::onLichSuHuy()
{
    // Tạo dialog hiển thị lịch sử hủy hàng
    QDialog* dlg = new QDialog(this);
    dlg->setWindowTitle("Lich su huy hang");
    dlg->resize(900, 500);

    QVBoxLayout* layout = new QVBoxLayout(dlg);

    // Tạo bảng hiển thị
    QTableWidget* table = new QTableWidget(dlg);
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"Thoi gian", "Ma hang", "Ten hang", "So luong", "Ly do", "Loai"});
    table->setAlternatingRowColors(true);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Đọc file lichsuhuy.txt
    ifstream file;
    QString filePath1 = QCoreApplication::applicationDirPath() + "/lichsuhuy.txt";
    file.open(filePath1.toStdString());

    if (!file.is_open()) {
        QString filePath2 = QCoreApplication::applicationDirPath() + "/../../lichsuhuy.txt";
        file.open(filePath2.toStdString());
    }

    if (!file.is_open()) {
        file.open("lichsuhuy.txt");
    }

    vector<vector<string>> dsHuy;
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            // Format: thoiGian|maHang|tenHang|keHang|ngayHetHan|soLuong|lyDo|loaiHang
            stringstream ss(line);
            vector<string> row;
            string temp;
            while (getline(ss, temp, '|')) {
                row.push_back(temp);
            }
            if (row.size() >= 8) {
                dsHuy.push_back(row);
            }
        }
        file.close();
    }

    table->setRowCount(dsHuy.size());
    for (int i = 0; i < (int)dsHuy.size(); i++) {
        table->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(dsHuy[i][0])));
        table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(dsHuy[i][1])));
        table->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(dsHuy[i][2])));
        table->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(dsHuy[i][5])));
        table->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(dsHuy[i][6])));
        table->setItem(i, 5, new QTableWidgetItem(QString::fromStdString(dsHuy[i][7])));
    }

    table->setColumnWidth(0, 170);
    table->setColumnWidth(1, 110);
    table->setColumnWidth(2, 260);
    table->setColumnWidth(3, 90);
    table->setColumnWidth(4, 160);
    table->setColumnWidth(5, 110);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);

    layout->addWidget(table);

    // Nút đóng
    QPushButton* btnClose = new QPushButton("Dong", dlg);
    connect(btnClose, &QPushButton::clicked, dlg, &QDialog::accept);
    layout->addWidget(btnClose);

    dlg->exec();
    delete dlg;
}

void MainWindow::onBaoCaoDoanhThu()
{
    QDialog* dlg = new QDialog(this);
    dlg->setWindowTitle("Bao cao doanh thu chi tiet");
    dlg->resize(750, 600);

    QVBoxLayout* layout = new QVBoxLayout(dlg);

    // Thêm bộ lọc thời gian
    QGroupBox* filterGroup = new QGroupBox("Loc theo thoi gian", dlg);
    QVBoxLayout* filterLayout = new QVBoxLayout(filterGroup);

    QRadioButton* radioAll = new QRadioButton("Tat ca", filterGroup);
    QRadioButton* radioDay = new QRadioButton("Theo ngay", filterGroup);
    QRadioButton* radioMonth = new QRadioButton("Theo thang", filterGroup);
    radioAll->setChecked(true);

    // Style cho radio buttons
    QString radioStyle = R"(
        QRadioButton {
            padding: 8px;
            font-size: 13px;
        }
        QRadioButton::indicator {
            width: 18px;
            height: 18px;
        }
        QRadioButton::indicator:checked {
            background-color: #1A237E;
            border: 2px solid #1A237E;
            border-radius: 9px;
        }
        QRadioButton::indicator:unchecked {
            background-color: white;
            border: 2px solid #BDBDBD;
            border-radius: 9px;
        }
        QRadioButton:checked {
            color: #1A237E;
            font-weight: bold;
        }
    )";
    radioAll->setStyleSheet(radioStyle);
    radioDay->setStyleSheet(radioStyle);
    radioMonth->setStyleSheet(radioStyle);

    QDateEdit* dateEdit = new QDateEdit(QDate::currentDate(), filterGroup);
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");
    dateEdit->setEnabled(false);

    QDateEdit* monthEdit = new QDateEdit(QDate::currentDate(), filterGroup);
    monthEdit->setCalendarPopup(true);
    monthEdit->setDisplayFormat("MM/yyyy");
    monthEdit->setEnabled(false);

    connect(radioAll, &QRadioButton::toggled, [=](bool) {
        dateEdit->setEnabled(false);
        monthEdit->setEnabled(false);
    });
    connect(radioDay, &QRadioButton::toggled, [=](bool checked) {
        dateEdit->setEnabled(checked);
        monthEdit->setEnabled(false);
    });
    connect(radioMonth, &QRadioButton::toggled, [=](bool checked) {
        dateEdit->setEnabled(false);
        monthEdit->setEnabled(checked);
    });

    filterLayout->addWidget(radioAll);

    QHBoxLayout* dayLayout = new QHBoxLayout();
    dayLayout->addWidget(radioDay);
    dayLayout->addWidget(dateEdit);
    dayLayout->addStretch();
    filterLayout->addLayout(dayLayout);

    QHBoxLayout* monthLayout = new QHBoxLayout();
    monthLayout->addWidget(radioMonth);
    monthLayout->addWidget(monthEdit);
    monthLayout->addStretch();
    filterLayout->addLayout(monthLayout);

    layout->addWidget(filterGroup);

    // Text hiển thị báo cáo
    QTextEdit* textEdit = new QTextEdit(dlg);
    textEdit->setReadOnly(true);
    QFont monoFont("Courier New", 10);
    textEdit->setFont(monoFont);
    layout->addWidget(textEdit);

    // Biến lưu báo cáo hiện tại
    QString* currentReport = new QString();

    // Hàm tính báo cáo
    auto tinhBaoCao = [=]() {
        // Lấy loại lọc
        QString loaiLoc = "all";
        QDate ngayLoc, thangLoc;

        // day la code co ban quyen - nguyen huu tri
        if (radioDay->isChecked()) {
            loaiLoc = "day";
            ngayLoc = dateEdit->date();
        } else if (radioMonth->isChecked()) {
            loaiLoc = "month";

            thangLoc = monthEdit->date();
        }

        // Tính toán doanh thu từ hóa đơn
        float tongTienBan = 0;
        float tongGiam = 0;
        float tongThanhToan = 0;
        float tongGiaNhap = 0;
        int soHoaDon = 0;

        for (const HoaDonLichSu& hd : shop.lichSuHoaDon) {
            // Parse thời gian hóa đơn: "Mon May 04 23:42:58 2026"
            QString thoiGian = QString::fromStdString(hd.thoiGian).trimmed();
            QStringList parts = thoiGian.split(' ', Qt::SkipEmptyParts);

            bool khopLoc = false;
            if (loaiLoc == "all") {
                khopLoc = true;
            } else if (parts.size() >= 5 && loaiLoc == "day") {
                // Parse ngày từ hóa đơn
                QString day = parts[2];
                QString month = parts[1];
                QString year = parts[4];
                QMap<QString, int> monthMap;
                monthMap["Jan"] = 1; monthMap["Feb"] = 2; monthMap["Mar"] = 3;
                monthMap["Apr"] = 4; monthMap["May"] = 5; monthMap["Jun"] = 6;
                monthMap["Jul"] = 7; monthMap["Aug"] = 8; monthMap["Sep"] = 9;
                monthMap["Oct"] = 10; monthMap["Nov"] = 11; monthMap["Dec"] = 12;

                QDate ngayHD(year.toInt(), monthMap.value(month, 1), day.toInt());
                khopLoc = (ngayHD == ngayLoc);
            } else if (parts.size() >= 5 && loaiLoc == "month") {
                // Parse tháng/năm từ hóa đơn
                QString month = parts[1];
                QString year = parts[4];
                QMap<QString, int> monthMap;
                monthMap["Jan"] = 1; monthMap["Feb"] = 2; monthMap["Mar"] = 3;
                monthMap["Apr"] = 4; monthMap["May"] = 5; monthMap["Jun"] = 6;
                monthMap["Jul"] = 7; monthMap["Aug"] = 8; monthMap["Sep"] = 9;
                monthMap["Oct"] = 10; monthMap["Nov"] = 11; monthMap["Dec"] = 12;

                int thangHD = monthMap.value(month, 1);
                int namHD = year.toInt();
                khopLoc = (thangHD == thangLoc.month() && namHD == thangLoc.year());
            }

            if (khopLoc) {
                tongTienBan += hd.tongGoc;
                tongGiam += hd.tongGiam;
                tongThanhToan += hd.tongThanhToan;
                soHoaDon++;

                for (const ChiTietHoaDon& item : hd.chiTiet) {
                    tongGiaNhap += item.giaNhap * item.soLuong;
                }
            }
        }

        // Tính tổng giá hàng đã hủy (chỉ tính khi lọc "Tất cả")
        float tongGiaHangHuy = 0;
        if (loaiLoc == "all") {
            ifstream fileHuy("lichsuhuy.txt");
            if (fileHuy.is_open()) {
                string line;
                while (getline(fileHuy, line)) {
                    if (line.empty()) continue;
                    QStringList parts = QString::fromStdString(line).split('|');
                    if (parts.size() >= 6) {
                        QString maHang = parts[1];
                        int soLuong = parts[5].toInt();
                        HangHoa* h = shop.ds.timTheoMa(maHang.toStdString());
                        if (h) {
                            tongGiaHangHuy += h->getGiaNhap() * soLuong;
                        }
                    }
                }
                fileHuy.close();
            }
        }

        float doanhThuRong = tongThanhToan - tongGiaNhap - tongGiaHangHuy;

        // Tạo tiêu đề báo cáo
        QString tieuDe = "========== BAO CAO DOANH THU ==========\n";
        if (loaiLoc == "day") {
            tieuDe += QString("Ngay: %1\n").arg(ngayLoc.toString("dd/MM/yyyy"));
        } else if (loaiLoc == "month") {
            tieuDe += QString("Thang: %1\n").arg(thangLoc.toString("MM/yyyy"));
        } else {
            tieuDe += "Tat ca\n";
        }
        tieuDe += "\n";

        QString report = tieuDe;
        report += QString("Tong tien:                   %1\n").arg(formatTien((long long)tongTienBan));
        report += QString("Giam gia:                    %1\n").arg(formatTien((long long)tongGiam));
        report += QString("Thanh toan:                  %1\n").arg(formatTien((long long)tongThanhToan));
        report += QString("Tong gia goc hang da ban:    %1\n").arg(formatTien((long long)tongGiaNhap));
        if (loaiLoc == "all") {
            report += QString("Tong gia hang da huy:        %1\n").arg(formatTien((long long)tongGiaHangHuy));
        }
        report += QString("So hoa don:                  %1\n").arg(soHoaDon);
        report += QString("\n");
        report += QString("========================================\n");
        report += QString("DOANH THU RONG:              %1\n").arg(formatTien((long long)doanhThuRong));
        report += QString("========================================\n");

        textEdit->setPlainText(report);
        *currentReport = report;  // Lưu vào biến để dùng khi save file
    };

    // Tính báo cáo ban đầu
    tinhBaoCao();

    // Kết nối sự kiện thay đổi bộ lọc
    connect(radioAll, &QRadioButton::clicked, tinhBaoCao);
    connect(radioDay, &QRadioButton::clicked, tinhBaoCao);
    connect(radioMonth, &QRadioButton::clicked, tinhBaoCao);
    connect(dateEdit, &QDateEdit::dateChanged, [=]() { if (radioDay->isChecked()) tinhBaoCao(); });
    connect(monthEdit, &QDateEdit::dateChanged, [=]() { if (radioMonth->isChecked()) tinhBaoCao(); });

    // Nút lưu file và đóng
    QHBoxLayout* btnLayout = new QHBoxLayout();
    QPushButton* btnSave = new QPushButton("Luu ra file", dlg);
    QPushButton* btnClose = new QPushButton("Dong", dlg);

    connect(btnSave, &QPushButton::clicked, [=]() {
        QString fileName = QFileDialog::getSaveFileName(dlg, "Luu bao cao", "", "Text Files (*.txt)");
        if (!fileName.isEmpty()) {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                QTextStream out(&file);
                out << *currentReport;
                file.close();
                QMessageBox::information(dlg, "Thanh cong", "Da luu bao cao vao file!");
            }
        }
    });

    connect(btnClose, &QPushButton::clicked, dlg, &QDialog::accept);

    btnLayout->addWidget(btnSave);
    btnLayout->addWidget(btnClose);
    layout->addLayout(btnLayout);

    dlg->exec();
    delete currentReport;
    delete dlg;
}

void MainWindow::applyStyleSheet()
{
    setStyleSheet(R"(
/* ═══════════════════════════════════════════════════════════════
   GREEN RETAIL THEME - Fresh & Professional
   Inspired by modern retail design
   ═══════════════════════════════════════════════════════════════ */

/* ─────────────────────────────────────────────────────────────── */
/* BASE STYLES */
/* ─────────────────────────────────────────────────────────────── */
QMainWindow, QWidget {
    background-color: #F7F9F7;
    color: #1F2937;
    font-family: "Segoe UI", "SF Pro Display", "Roboto", sans-serif;
    font-size: 13px;
}

/* ─────────────────────────────────────────────────────────────── */
/* SIDEBAR - Soft pastel green gradient */
/* ─────────────────────────────────────────────────────────────── */
QFrame#frameSidebar {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                stop:0 #A7F3D0, stop:1 #6EE7B7);
    border-right: 2px solid #34D399;
}

QFrame#frameSidebar QLabel {
    color: #065F46;
}

QLabel#labelAppName {
    font-size: 17px;
    font-weight: bold;
    color: #065F46;
    letter-spacing: 0.5px;
    background-color: #FFFFFF;
    padding: 8px 16px;
    border-radius: 8px;
}

QLabel#labelNhanVien {
    font-size: 11px;
    color: #065F46;
    font-weight: 600;
    background-color: rgba(255, 255, 255, 0.6);
    padding: 6px 10px;
    border-radius: 6px;
}

QFrame#frameSidebar QPushButton {
    color: #065F46;
    background: transparent;
    border: none;
    border-radius: 10px;
    text-align: left;
    padding: 14px 18px;
    font-size: 13px;
    font-weight: 600;
}

QFrame#frameSidebar QPushButton:hover {
    background-color: rgba(255,255,255,0.5);
    color: #064E3B;
}

QFrame#frameSidebar QPushButton:checked {
    background-color: #FFFFFF;
    color: #047857;
    font-weight: 700;
    border-left: 4px solid #F59E0B;
}

QPushButton#btnDangXuat {
    color: #FEF3C7;
}

QPushButton#btnDangXuat:hover {
    background-color: rgba(239,68,68,0.25);
    color: #FEE2E2;
}

QFrame#line1, QFrame#line2 {
    color: rgba(255,255,255,0.2);
}

/* ─────────────────────────────────────────────────────────────── */
/* PAGE TITLES - Bold and modern */
/* ─────────────────────────────────────────────────────────────── */
QLabel#labelTieuDeTQ, QLabel#labelTieuDeKho,
QLabel#labelTieuDeKH, QLabel#labelBH_TieuDe {
    font-size: 24px;
    font-weight: 700;
    color: #1E293B;
}

QLabel#labelGio_TieuDe {
    font-size: 17px;
    font-weight: 700;
    color: #1E293B;
}

/* ─────────────────────────────────────────────────────────────── */
/* DASHBOARD CARDS - Elevated with shadows */
/* ─────────────────────────────────────────────────────────────── */
QFrame#cardHangHoa, QFrame#cardHetHan,
QFrame#cardKhachHang, QFrame#cardSapHet {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                stop:0 #FFFFFF, stop:1 #F8FAFC);
    border-radius: 14px;
    border: 2px solid #E2E8F0;
}

QLabel#labelCardHH_So, QLabel#labelCardHan_So,
QLabel#labelCardKH_So, QLabel#labelCardSH_So {
    font-size: 32px;
    font-weight: 800;
    color: #10B981;
}

/* ─────────────────────────────────────────────────────────────── */
/* TABLES - Clean with alternating rows */
/* ─────────────────────────────────────────────────────────────── */
QTableWidget {
    background: #FFFFFF;
    border: 2px solid #E2E8F0;
    border-radius: 10px;
    gridline-color: #F1F5F9;
    selection-background-color: #DBEAFE;
    selection-color: #1E40AF;
    alternate-background-color: #F8FAFC;
}

QTableWidget::item {
    padding: 8px 12px;
}

QTableWidget::item:hover {
    background-color: #F0F9FF;
}

QHeaderView::section {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                stop:0 #10B981, stop:1 #059669);
    color: #FFFFFF;
    font-weight: 600;
    padding: 10px 12px;
    border: none;
    border-right: 1px solid #047857;
    border-bottom: 2px solid #047857;
    font-size: 13px;
}

/* ─────────────────────────────────────────────────────────────── */
/* INPUT FIELDS - Modern with focus states */
/* ─────────────────────────────────────────────────────────────── */
QLineEdit, QComboBox, QSpinBox {
    background: #FFFFFF;
    border: 2px solid #CBD5E1;
    border-radius: 8px;
    padding: 6px 12px;
    color: #1E293B;
    font-size: 13px;
}

QLineEdit:focus, QComboBox:focus, QSpinBox:focus {
    border: 2px solid #10B981;
    background: #F0FDF4;
}

QComboBox::drop-down {
    border: none;
    width: 24px;
}

QComboBox::down-arrow {
    image: none;
    border-left: 4px solid transparent;
    border-right: 4px solid transparent;
    border-top: 5px solid #64748B;
    margin-right: 8px;
}

/* ─────────────────────────────────────────────────────────────── */
/* BUTTONS - Vibrant with hover effects */
/* ─────────────────────────────────────────────────────────────── */
QPushButton#btnThemVaoGio {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                stop:0 #10B981, stop:1 #059669);
    color: #FFFFFF;
    border-radius: 8px;
    font-weight: 600;
    padding: 8px 16px;
    border: none;
}

QPushButton#btnThemVaoGio:hover {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                stop:0 #34D399, stop:1 #10B981);
}

QPushButton#btnThemVaoGio:pressed {
    background: #047857;
}

QPushButton#btnThanhToan {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                stop:0 #10B981, stop:1 #059669);
    color: #FFFFFF;
    border-radius: 10px;
    font-size: 16px;
    font-weight: 700;
    padding: 12px 24px;
    border: none;
}

QPushButton#btnThanhToan:hover {
    background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                                stop:0 #34D399, stop:1 #10B981);
}

QPushButton#btnThanhToan:pressed {
    background: #047857;
}

QPushButton#btnXoaGio {
    background: #FEF2F2;
    color: #DC2626;
    border-radius: 8px;
    border: 2px solid #FEE2E2;
    font-weight: 600;
    padding: 8px 16px;
}

QPushButton#btnXoaGio:hover {
    background: #FEE2E2;
    border: 2px solid #FCA5A5;
}

QPushButton#btnXoaGio:pressed {
    background: #FECACA;
}

QPushButton#btnTimKhach {
    background: #F0FDF4;
    color: #059669;
    border-radius: 8px;
    border: 2px solid #A7F3D0;
    font-weight: 600;
    padding: 8px 16px;
}

QPushButton#btnTimKhach:hover {
    background: #DCFCE7;
    border: 2px solid #6EE7B7;
}

/* ─────────────────────────────────────────────────────────────── */
/* FRAMES & CONTAINERS */
/* ─────────────────────────────────────────────────────────────── */
QFrame#frameKhachHangBH, QFrame#frameTongTien {
    background: #FFFFFF;
    border: 2px solid #E2E8F0;
    border-radius: 12px;
}

QFrame#frameBH_Trai {
    background: #F8FAFC;
    border-right: 2px solid #E2E8F0;
}

/* ─────────────────────────────────────────────────────────────── */
/* LABELS - Typography hierarchy */
/* ─────────────────────────────────────────────────────────────── */
QLabel#labelThongTinKhach {
    color: #64748B;
    font-size: 12px;
    font-weight: 500;
}

QLabel#labelTongThanhToan {
    font-size: 22px;
    font-weight: 800;
    color: #10B981;
}

QLabel#labelThanhToanTitle {
    font-size: 15px;
    font-weight: 700;
    color: #1E293B;
}

QLabel#labelTongGiam {
    color: #EF4444;
    font-weight: 700;
    font-size: 14px;
}

QLabel#labelSoDong, QLabel#labelSoKH {
    color: #64748B;
    font-size: 12px;
    font-weight: 500;
}

QLabel#labelCanhBao {
    font-size: 15px;
    font-weight: 700;
    color: #DC2626;
}

/* ─────────────────────────────────────────────────────────────── */
/* SCROLLBARS - Subtle and modern */
/* ─────────────────────────────────────────────────────────────── */
QScrollBar:vertical {
    background: #F1F5F9;
    width: 12px;
    border-radius: 6px;
}

QScrollBar::handle:vertical {
    background: #CBD5E1;
    border-radius: 6px;
    min-height: 30px;
}

QScrollBar::handle:vertical:hover {
    background: #94A3B8;
}

QScrollBar:horizontal {
    background: #F1F5F9;
    height: 12px;
    border-radius: 6px;
}

QScrollBar::handle:horizontal {
    background: #CBD5E1;
    border-radius: 6px;
    min-width: 30px;
}

QScrollBar::handle:horizontal:hover {
    background: #94A3B8;
}

QScrollBar::add-line, QScrollBar::sub-line {
    border: none;
    background: none;
}
    )");
}
