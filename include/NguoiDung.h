#ifndef NGUOIDUNG_H
#define NGUOIDUNG_H

#include <string>
#include <vector>
#include "Common.h"

class TaiKhoan {
public:
    std::string tenDangNhap;
    std::string matKhau;
    VaiTro vaiTro;
    TaiKhoan() : vaiTro(DIEU_HANH) {}
    TaiKhoan(std::string tk, std::string mk, VaiTro vt) : tenDangNhap(tk), matKhau(mk), vaiTro(vt) {}
};

// Lop co so - the hien tinh dong goi + da hinh (phuong thuc thuan virtual)
class NguoiDung {
protected:
    std::string tenDangNhap;
    VaiTro vaiTro;

public:
    NguoiDung(std::string tk, VaiTro vt) : tenDangNhap(tk), vaiTro(vt) {}
    virtual ~NguoiDung() {}

    std::string getTenDangNhap() const { return tenDangNhap; }
    VaiTro getVaiTro() const { return vaiTro; }

    virtual std::vector<ChucNang> danhSachChucNang() const = 0;
    virtual std::string tenVaiTro() const = 0;
};

class QuanLy : public NguoiDung {
public:
    QuanLy(std::string tk) : NguoiDung(tk, QUAN_LY) {}
    std::vector<ChucNang> danhSachChucNang() const override {
        return {CN_QUAN_LY_TAI_XE, CN_QUAN_LY_TAXI, CN_DAT_CHUYEN_XE, CN_THONG_KE};
    }
    std::string tenVaiTro() const override { return "Quan ly"; }
};

class DieuHanh : public NguoiDung {
public:
    DieuHanh(std::string tk) : NguoiDung(tk, DIEU_HANH) {}
    std::vector<ChucNang> danhSachChucNang() const override {
        return {CN_DAT_CHUYEN_XE};
    }
    std::string tenVaiTro() const override { return "Dieu hanh"; }
};

#endif // NGUOIDUNG_H
