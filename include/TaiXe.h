#ifndef TAIXE_H
#define TAIXE_H

#include <string>
#include "Common.h"

<<<<<<< HEAD
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
=======
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
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
    TrangThai trangThai;

public:
    TaiXe() : trangThai(DANG_HOAT_DONG) {}
<<<<<<< HEAD
    TaiXe(string ma, string t, string hd, Date ns, string dc,
          string sdt, string bl, string gplx, TrangThai tt)
        : maTX(ma), ten(t), hoDem(hd), ngaySinh(ns), diaChi(dc),
          soDienThoai(sdt), bangLai(bl), soGPLX(gplx), trangThai(tt) {}

    string getMaTX() const { return maTX; }
    string getTen() const { return ten; }
    string getHoDem() const { return hoDem; }
    string getHoTen() const { return hoDem + " " + ten; }
    Date getNgaySinh() const { return ngaySinh; }
    string getDiaChi() const { return diaChi; }
    string getSoDienThoai() const { return soDienThoai; }
    string getBangLai() const { return bangLai; }
    string getSoGPLX() const { return soGPLX; }
    TrangThai getTrangThai() const { return trangThai; }

    void setTen(string t) { ten = t; }
    void setHoDem(string hd) { hoDem = hd; }
    void setNgaySinh(Date d) { ngaySinh = d; }
    void setDiaChi(string dc) { diaChi = dc; }
    void setSoDienThoai(string sdt) { soDienThoai = sdt; }
    void setBangLai(string bl) { bangLai = bl; }
    void setSoGPLX(string gplx) { soGPLX = gplx; }
=======
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
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
};

#endif // TAIXE_H
