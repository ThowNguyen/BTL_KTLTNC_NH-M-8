#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle("Đăng nhập hệ thống");
    ui->btnDangNhap->setDefault(true); // Enter = nhấn nút

    connect(ui->btnDangNhap, &QPushButton::clicked,
            this, &LoginDialog::onDangNhapClicked);
}

LoginDialog::~LoginDialog() { delete ui; }

QString LoginDialog::getMaNV()    const { return ui->lineEditMaNV->text().trimmed(); }
QString LoginDialog::getMatKhau() const { return ui->lineEditMatKhau->text(); }

void LoginDialog::onDangNhapClicked() {
    if (getMaNV().isEmpty() || getMatKhau().isEmpty()) {
        ui->labelLoi->setText("Vui lòng nhập đầy đủ thông tin!");
        return;
    }
    accept(); // đóng dialog, trả về QDialog::Accepted
}