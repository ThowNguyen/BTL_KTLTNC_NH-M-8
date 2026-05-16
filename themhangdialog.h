#ifndef THEMHANGDIALOG_H
#define THEMHANGDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QStackedWidget>
#include <QString>
#include "core/core.h"

class ThemHangDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ThemHangDialog(CuaHang* shop, QWidget *parent = nullptr);
    HangHoa* getHangHoaMoi() const;

private slots:
    void onLoaiChanged(int index);
    void onXacNhan();

private:
    CuaHang* shop;
    HangHoa* hangHoaMoi;

    // Common fields
    QComboBox* comboLoai;
    QLineEdit* editMa;
    QLineEdit* editTen;
    QDoubleSpinBox* spinGiaNhap;
    QDoubleSpinBox* spinGiaNiemYet;
    QSpinBox* spinSoLuong;
    QLineEdit* editNgayHetHan;

    // Type-specific fields
    QStackedWidget* stackedSpecific;
    QSpinBox* spinBaoHanh;              // DoGiaDung
    QDoubleSpinBox* spinKhoiLuongHTS;   // HangTuoiSong
    QLineEdit* editTinhTrangHTS;        // HangTuoiSong
    QDoubleSpinBox* spinKhoiLuongTC;    // TraiCay
    QLineEdit* editTinhTrangTC;         // TraiCay
};

#endif // THEMHANGDIALOG_H
