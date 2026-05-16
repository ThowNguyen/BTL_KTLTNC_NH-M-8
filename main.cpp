#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("Quan Ly Sieu Thi");
// DEBUG: kiểm tra thư mục hiện tại và file
    qDebug() << "Working dir:" << QDir::currentPath();
    qDebug() << "data.txt exists:" << QFile::exists("data.txt");
    qDebug() << "nhanvien.txt exists:" << QFile::exists("nhanvien.txt");
    qDebug() << "khachhang.txt exists:" << QFile::exists("khachhang.txt");

    // Test đọc trực tiếp
    try {
        CuaHang testShop;
        qDebug() << "=== Doc hang hoa ===";
        testShop.docFile("data.txt");
        qDebug() << "Hang hoa OK:" << testShop.ds.toVector().size();

        qDebug() << "=== Doc khach hang ===";
        testShop.docKhachHang("khachhang.txt");
        qDebug() << "Khach hang OK:" << testShop.dsKhachHang.toVector().size();
    } catch (const exception& e) {
        qDebug() << "EXCEPTION:" << e.what();
    } catch (...) {
        qDebug() << "UNKNOWN EXCEPTION!";
    }

    MainWindow w;
    w.khoiDong();  // Gọi khoiDong() trước - sẽ hiển thị dialog đăng nhập và sau đó mới show() cửa sổ chính
    return a.exec();
}
