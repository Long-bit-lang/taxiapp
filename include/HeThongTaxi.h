#ifndef HETHONGTAXI_H
#define HETHONGTAXI_H

#include "QuanLyTaiXe.h"
#include "QuanLyTaxi.h"
#include "QuanLyChuyenXe.h"
#include "QuanLyTaiKhoan.h"

class HeThongTaxi {
private:
    QuanLyTaiXe qlTaiXe;
    QuanLyTaxi qlTaxi;
    QuanLyChuyenXe qlChuyenXe;
    QuanLyTaiKhoan qlTaiKhoan;

public:
    HeThongTaxi() {}

    void khoiDong() {
        qlTaiKhoan.docFile();
        qlTaiXe.docFile();
        qlTaxi.docFile();
        qlChuyenXe.docFile();
    }

    QuanLyTaiXe& taiXe() { return qlTaiXe; }
    QuanLyTaxi& taxi() { return qlTaxi; }
    QuanLyChuyenXe& chuyenXe() { return qlChuyenXe; }
    QuanLyTaiKhoan& taiKhoan() { return qlTaiKhoan; }
};

#endif // HETHONGTAXI_H
