#ifndef NGUOIDUNG_H
#define NGUOIDUNG_H

#include <string>
#include <vector>
#include "Common.h"

<<<<<<< HEAD
using namespace std;

class TaiKhoan {
public:
    string tenDangNhap;
    string matKhau;
    VaiTro vaiTro;
    TaiKhoan() : vaiTro(DIEU_HANH) {}
    TaiKhoan(string tk, string mk, VaiTro vt) : tenDangNhap(tk), matKhau(mk), vaiTro(vt) {}
=======
class TaiKhoan {
public:
    std::string tenDangNhap;
    std::string matKhau;
    VaiTro vaiTro;
    TaiKhoan() : vaiTro(DIEU_HANH) {}
    TaiKhoan(std::string tk, std::string mk, VaiTro vt) : tenDangNhap(tk), matKhau(mk), vaiTro(vt) {}
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
};

// Lop co so - the hien tinh dong goi + da hinh (phuong thuc thuan virtual)
class NguoiDung {
protected:
<<<<<<< HEAD
    string tenDangNhap;
    VaiTro vaiTro;

public:
    NguoiDung(string tk, VaiTro vt) : tenDangNhap(tk), vaiTro(vt) {}
    virtual ~NguoiDung() {}

    string getTenDangNhap() const { return tenDangNhap; }
    VaiTro getVaiTro() const { return vaiTro; }

    virtual vector<ChucNang> danhSachChucNang() const = 0;
    virtual string tenVaiTro() const = 0;
=======
    std::string tenDangNhap;
    VaiTro vaiTro;

public:
    NguoiDung(std::string tk, VaiTro vt) : tenDangNhap(tk), vaiTro(vt) {}
    virtual ~NguoiDung() {}

    std::string getTenDangNhap() const { return tenDangNhap; }
    VaiTro getVaiTro() const { return vaiTro; }

    virtual std::vector<ChucNang> danhSachChucNang() const = 0;
    virtual std::string tenVaiTro() const = 0;
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
};

class QuanLy : public NguoiDung {
public:
<<<<<<< HEAD
    QuanLy(string tk) : NguoiDung(tk, QUAN_LY) {}
    vector<ChucNang> danhSachChucNang() const override {
        return {CN_QUAN_LY_TAI_XE, CN_QUAN_LY_TAXI, CN_DAT_CHUYEN_XE, CN_THONG_KE};
    }
    string tenVaiTro() const override { return "Quản lý"; }
=======
    QuanLy(std::string tk) : NguoiDung(tk, QUAN_LY) {}
    std::vector<ChucNang> danhSachChucNang() const override {
        return {CN_QUAN_LY_TAI_XE, CN_QUAN_LY_TAXI, CN_DAT_CHUYEN_XE, CN_THONG_KE};
    }
    std::string tenVaiTro() const override { return "Quản lý"; }
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
};

class DieuHanh : public NguoiDung {
public:
<<<<<<< HEAD
    DieuHanh(string tk) : NguoiDung(tk, DIEU_HANH) {}
    vector<ChucNang> danhSachChucNang() const override {
        return {CN_DAT_CHUYEN_XE};
    }
    string tenVaiTro() const override { return "Điều hành"; }
=======
    DieuHanh(std::string tk) : NguoiDung(tk, DIEU_HANH) {}
    std::vector<ChucNang> danhSachChucNang() const override {
        return {CN_DAT_CHUYEN_XE};
    }
    std::string tenVaiTro() const override { return "Điều hành"; }
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
};

#endif // NGUOIDUNG_H
