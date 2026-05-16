#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QColor>
#include <QString>
#include <QMap>
#include "core/core.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void khoiDong();

private slots:
    // ── Sidebar ──
    void onTongQuan();
    void onQuanLyKho();
    void onBanHang();
    void onKhachHang();
    void onTraCuuHangHoa();  // Tra cứu thông tin hàng hóa
    void onBaoCao();
    void onDangXuat();

    // ── Kho ──
    void onTimKiemHangHoa(const QString& text);
    void onLocLoai(int index);
    void onThemHang();
    void onSuaHang();
    void onNhapKho();
    void onXoaHang();
    void onHuyHangHetHan();
    void onSapXepKho();
    void onTaoDanhSachTheoDoi();

    // ── Khách hàng ──
    void onTimKiemKhachHang(const QString& text);

    // ── Bán hàng ──
    void onTimHangBanHang(const QString& text);
    void onThemVaoGio();
    void onXoaGio();
    void onTimKhachHangBH();
    void onThanhToan();

    // ── Báo cáo ──
    void onBaoCaoDoanhThu();
    void onLichSuHuy();
    void onThongTinKH();

private:
    Ui::MainWindow* ui;
    CuaHang shop;
    QButtonGroup* menuGroup;

    // Giỏ hàng: maHang → soLuong
    QMap<QString, int> gioHang;

    // ── Setup ──
    void applyStyleSheet();
    void setupMenuGroup();
    void chuyenTrang(int index);

    // ── Dashboard ──
    void loadDashboard();
    void loadCanhBao();

    // ── Kho ──
    void loadHangHoa();
    void locHangHoa();
    void hienThiCanhBaoKho();

    // ── Khách hàng ──
    void loadKhachHang();
    void locKhachHang(const QString& tuKhoa);

    // ── Bán hàng ──
    void loadDanhSachHangBH(const QString& tuKhoa = "");
    void refreshGioHang();
    void capNhatTongTien();
    void hienThiHoaDon(const CuaHang::KetQuaBanHang& kq);

    // ── Báo cáo ──
    void loadBaoCao();
    void loadLichSuHoaDon();

    // ── Util ──
    QString loaiHienThi  (const string& loai) const;
    QString trangThaiHang(HangHoa* h)          const;
    QColor  mauTrangThai (HangHoa* h)          const;
    QString formatTien   (long long tien)       const;
    float   safeStof     (const string& str)    const;

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif // MAINWINDOW_H