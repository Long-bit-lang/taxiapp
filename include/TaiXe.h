#pragma once
#include <string>
#include "Common.h"

using namespace std;


class TaiXe {
private:
    string maTX;
    string ten;
    string hoDem;
    Date ngaySinh;
    string diaChi;
    string soDienThoai;
    string bangLai;
    string soGPLX;
    TrangThai trangThai;

public:
    TaiXe() : trangThai(DANG_HOAT_DONG) {}
    TaiXe(string ma, string t, string hd, Date ns, string dc,
          string sdt, string bl, string gplx, TrangThai tt)
        : maTX(ma), ten(t), hoDem(hd), ngaySinh(ns), diaChi(dc),
          soDienThoai(sdt), bangLai(bl), soGPLX(gplx), trangThai(tt) {}

    const string& getMaTX() const { return maTX; }
    string getTen() const { return ten; }
    string getHoDem() const { return hoDem; }
    string getHoTen() const { return hoDem + " " + ten; }
    Date getNgaySinh() const { return ngaySinh; }
    string getDiaChi() const { return diaChi; }
    const string& getSoDienThoai() const { return soDienThoai; }
    const string& getBangLai() const { return bangLai; }
    const string& getSoGPLX() const { return soGPLX; }
    TrangThai getTrangThai() const { return trangThai; }

    void setTen(string t) { ten = t; }
    void setHoDem(string hd) { hoDem = hd; }
    void setNgaySinh(Date d) { ngaySinh = d; }
    void setDiaChi(string dc) { diaChi = dc; }
    void setSoDienThoai(string sdt) { soDienThoai = sdt; }
    void setBangLai(string bl) { bangLai = bl; }
    void setSoGPLX(string gplx) { soGPLX = gplx; }
};