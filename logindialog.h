#pragma once
#include <QDialog>

namespace Ui { class LoginDialog; }

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

    QString getMaNV()    const;
    QString getMatKhau() const;

private slots:
    void onDangNhapClicked();

private:
    Ui::LoginDialog *ui;
};