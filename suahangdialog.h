#ifndef SUAHANGDIALOG_H
#define SUAHANGDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QString>

class SuaHangDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SuaHangDialog(const QString& maHang, const QString& tenHang,
                           float giaNiemYetHienTai, int soLuongHienTai,
                           const QString& ngayHetHanHienTai,
                           QWidget *parent = nullptr);

    float getGiaNiemYetMoi() const;
    int getSoLuongMoi() const;
    QString getNgayHetHanMoi() const;

private:
    QDoubleSpinBox* spinGiaNiemYet;
    QSpinBox* spinSoLuong;
    QLineEdit* editNgayHetHan;

    float giaNiemYetMoi;
    int soLuongMoi;
    QString ngayHetHanMoi;
};

#endif // SUAHANGDIALOG_H
