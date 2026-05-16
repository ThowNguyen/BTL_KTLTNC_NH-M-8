#include "suahangdialog.h"
#include <QMessageBox>

SuaHangDialog::SuaHangDialog(const QString& maHang, const QString& tenHang,
                             float giaNiemYetHienTai, int soLuongHienTai,
                             const QString& ngayHetHanHienTai,
                             QWidget *parent)
    : QDialog(parent), giaNiemYetMoi(giaNiemYetHienTai),
      soLuongMoi(soLuongHienTai), ngayHetHanMoi(ngayHetHanHienTai)
{
    setWindowTitle("Sua thong tin hang hoa");
    setMinimumWidth(450);

    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);

    // Thông tin hàng hóa
    auto* infoLabel = new QLabel(
        QString("<b>Ma hang:</b> %1<br><b>Ten hang:</b> %2")
            .arg(maHang).arg(tenHang));
    infoLabel->setStyleSheet("padding: 10px; background: #F5F7FA; border-radius: 6px;");
    mainLayout->addWidget(infoLabel);

    // Form nhập liệu
    auto* formLayout = new QFormLayout();
    formLayout->setSpacing(10);

    // Giá niêm yết
    spinGiaNiemYet = new QDoubleSpinBox();
    spinGiaNiemYet->setMinimum(0);
    spinGiaNiemYet->setMaximum(999999999);
    spinGiaNiemYet->setValue(giaNiemYetHienTai);
    spinGiaNiemYet->setMinimumHeight(32);
    spinGiaNiemYet->setSuffix(" d");
    formLayout->addRow("Gia niem yet:", spinGiaNiemYet);

    // Số lượng
    spinSoLuong = new QSpinBox();
    spinSoLuong->setMinimum(0);
    spinSoLuong->setMaximum(99999);
    spinSoLuong->setValue(soLuongHienTai);
    spinSoLuong->setMinimumHeight(32);
    formLayout->addRow("So luong:", spinSoLuong);

    // Ngày hết hạn
    editNgayHetHan = new QLineEdit(ngayHetHanHienTai);
    editNgayHetHan->setPlaceholderText("dd/mm/yyyy");
    editNgayHetHan->setMinimumHeight(32);
    formLayout->addRow("Ngay het han:", editNgayHetHan);

    mainLayout->addLayout(formLayout);

    // Ghi chú
    auto* noteLabel = new QLabel("Luu y: Ngay het han phai co dinh dang dd/mm/yyyy");
    noteLabel->setStyleSheet("color: #666; font-size: 11px; font-style: italic;");
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

    auto* btnLuu = new QPushButton("Luu thay doi");
    btnLuu->setMinimumSize(110, 36);
    btnLuu->setStyleSheet(
        "QPushButton{background:#1A237E;color:white;border-radius:6px;font-weight:bold;}"
        "QPushButton:hover{background:#3F51B5;}");
    connect(btnLuu, &QPushButton::clicked, this, [this]() {
        // Validate ngày hết hạn
        QString ngay = editNgayHetHan->text().trimmed();
        if (!ngay.isEmpty()) {
            QStringList parts = ngay.split('/');
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

        giaNiemYetMoi = spinGiaNiemYet->value();
        soLuongMoi = spinSoLuong->value();
        ngayHetHanMoi = editNgayHetHan->text().trimmed();
        accept();
    });
    btnLayout->addWidget(btnLuu);

    mainLayout->addLayout(btnLayout);
}

float SuaHangDialog::getGiaNiemYetMoi() const
{
    return giaNiemYetMoi;
}

int SuaHangDialog::getSoLuongMoi() const
{
    return soLuongMoi;
}

QString SuaHangDialog::getNgayHetHanMoi() const
{
    return ngayHetHanMoi;
}
