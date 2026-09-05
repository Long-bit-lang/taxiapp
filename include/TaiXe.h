#ifndef TAIXE_H
#define TAIXE_H

#include <string>
#include "Common.h"

class TaiXe {
private:
    std::string maTX;
    std::string ten;
    std::string hoDem;
    Date ngaySinh;
    std::string diaChi;
    std::string soDienThoai;
    std::string bangLai;
    std::string soGPLX;
    TrangThai trangThai;

public:
    TaiXe() : trangThai(DANG_HOAT_DONG) {}
    TaiXe(std::string ma, std::string t, std::string hd, Date ns, std::string dc,
          std::string sdt, std::string bl, std::string gplx, TrangThai tt)
        : maTX(ma), ten(t), hoDem(hd), ngaySinh(ns), diaChi(dc),
          soDienThoai(sdt), bangLai(bl), soGPLX(gplx), trangThai(tt) {}

    std::string getMaTX() const { return maTX; }
    std::string getTen() const { return ten; }
    std::string getHoDem() const { return hoDem; }
    std::string getHoTen() const { return hoDem + " " + ten; }
    Date getNgaySinh() const { return ngaySinh; }
    std::string getDiaChi() const { return diaChi; }
    std::string getSoDienThoai() const { return soDienThoai; }
    std::string getBangLai() const { return bangLai; }
    std::string getSoGPLX() const { return soGPLX; }
    TrangThai getTrangThai() const { return trangThai; }

    void setTen(std::string t) { ten = t; }
    void setHoDem(std::string hd) { hoDem = hd; }
    void setNgaySinh(Date d) { ngaySinh = d; }
    void setDiaChi(std::string dc) { diaChi = dc; }
    void setSoDienThoai(std::string sdt) { soDienThoai = sdt; }
    void setBangLai(std::string bl) { bangLai = bl; }
    void setSoGPLX(std::string gplx) { soGPLX = gplx; }
};

#endif // TAIXE_H
