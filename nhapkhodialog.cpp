#include "nhapkhodialog.h"
#include <QFormLayout>
#include <QDateEdit>
#include <QDate>

NhapKhoDialog::NhapKhoDialog(const QString& maHang, const QString& tenHang,
                             int tonKhoHienTai, float giaNhapCu, const QString& ngayHetHanCu,
                             QWidget *parent)
    : QDialog(parent), soLuongNhap(0), giaNhap(0), ngayHetHan("")
{
    setWindowTitle("Nhap them kho");
    setMinimumWidth(450);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(12);

    // Thông tin hàng hóa
    auto* infoLabel = new QLabel(
        QString("<b>Ma hang:</b> %1<br><b>Ten hang:</b> %2<br><b>Ton kho hien tai:</b> %3")
            .arg(maHang).arg(tenHang).arg(tonKhoHienTai));
    infoLabel->setStyleSheet("padding: 10px; background: #F5F7FA; border-radius: 6px;");
    mainLayout->addWidget(infoLabel);

    // Form nhập liệu
    auto* formLayout = new QFormLayout();
    formLayout->setSpacing(10);

    // Số lượng
    spinSoLuong = new QSpinBox();
    spinSoLuong->setMinimum(1);
    spinSoLuong->setMaximum(99999);
    spinSoLuong->setValue(10);
    spinSoLuong->setMinimumHeight(32);
    formLayout->addRow("So luong nhap:", spinSoLuong);

    // Giá nhập
    spinGiaNhap = new QDoubleSpinBox();
    spinGiaNhap->setMinimum(0);
    spinGiaNhap->setMaximum(999999999);
    spinGiaNhap->setDecimals(0);
    spinGiaNhap->setValue(giaNhapCu);
    spinGiaNhap->setMinimumHeight(32);
    formLayout->addRow("Gia nhap (VND):", spinGiaNhap);

    // Giá bán
    spinGiaBan = new QDoubleSpinBox();
    spinGiaBan->setMinimum(0);
    spinGiaBan->setMaximum(999999999);
    spinGiaBan->setDecimals(0);
    spinGiaBan->setValue(giaNhapCu * 1.5); // Mặc định giá bán = giá nhập * 1.5
    spinGiaBan->setMinimumHeight(32);
    formLayout->addRow("Gia ban (VND):", spinGiaBan);

    // Ngày hết hạn
    dateNgayHetHan = new QDateEdit();
    dateNgayHetHan->setCalendarPopup(true);
    dateNgayHetHan->setDisplayFormat("dd/MM/yyyy");
    dateNgayHetHan->setMinimumHeight(32);

    // Parse ngày hết hạn cũ hoặc set mặc định
    if (!ngayHetHanCu.isEmpty()) {
        QStringList parts = ngayHetHanCu.split('/');
        if (parts.size() == 3) {
            QDate date(parts[2].toInt(), parts[1].toInt(), parts[0].toInt());
            if (date.isValid()) {
                dateNgayHetHan->setDate(date);
            } else {
                dateNgayHetHan->setDate(QDate::currentDate().addMonths(6));
            }
        } else {
            dateNgayHetHan->setDate(QDate::currentDate().addMonths(6));
        }
    } else {
        dateNgayHetHan->setDate(QDate::currentDate().addMonths(6));
    }

    formLayout->addRow("Ngay het han:", dateNgayHetHan);

    mainLayout->addLayout(formLayout);

    // Ghi chú
    auto* noteLabel = new QLabel("Luu y: Moi lo hang co the co gia nhap va ngay het han khac nhau.");
    noteLabel->setStyleSheet("color: #666; font-style: italic; font-size: 11px;");
    mainLayout->addWidget(noteLabel);

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

    auto* btnXacNhan = new QPushButton("Xac nhan");
    btnXacNhan->setMinimumSize(100, 36);
    btnXacNhan->setStyleSheet(
        "QPushButton{background:#1A237E;color:white;border-radius:6px;font-weight:bold;}"
        "QPushButton:hover{background:#3F51B5;}");
    connect(btnXacNhan, &QPushButton::clicked, this, [this]() {
        soLuongNhap = spinSoLuong->value();
        giaNhap = spinGiaNhap->value();
        giaBan = spinGiaBan->value();
        QDate date = dateNgayHetHan->date();
        ngayHetHan = QString("%1/%2/%3")
            .arg(date.day(), 2, 10, QChar('0'))
            .arg(date.month(), 2, 10, QChar('0'))
            .arg(date.year());
        accept();
    });
    btnLayout->addWidget(btnXacNhan);

    mainLayout->addLayout(btnLayout);
}

int NhapKhoDialog::getSoLuongNhap() const
{
    return soLuongNhap;
}

float NhapKhoDialog::getGiaNhap() const
{
    return giaNhap;
}

float NhapKhoDialog::getGiaBan() const
{
    return giaBan;
}

QString NhapKhoDialog::getNgayHetHan() const
{
    return ngayHetHan;
}
