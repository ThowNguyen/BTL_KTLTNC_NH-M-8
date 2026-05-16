#ifndef NHAPKHODIALOG_H
#define NHAPKHODIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QPushButton>
#include <QString>

class NhapKhoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NhapKhoDialog(const QString& maHang, const QString& tenHang,
                           int tonKhoHienTai, float giaNhapCu, const QString& ngayHetHanCu,
                           QWidget *parent = nullptr);
    int getSoLuongNhap() const;
    float getGiaNhap() const;
    float getGiaBan() const;
    QString getNgayHetHan() const;

private:
    QSpinBox* spinSoLuong;
    QDoubleSpinBox* spinGiaNhap;
    QDoubleSpinBox* spinGiaBan;
    QDateEdit* dateNgayHetHan;
    int soLuongNhap;
    float giaNhap;
    float giaBan;
    QString ngayHetHan;
};

#endif // NHAPKHODIALOG_H
