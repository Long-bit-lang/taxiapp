#ifndef CHUYENXE_H
#define CHUYENXE_H

#include <string>
#include "Common.h"

using namespace std;

class ChuyenXe {
private:
    string maChuyen;
    string tenKhachHang;
    string sdtKhachHang;
    string bienSoTaxi;
    string thoiDiem;   // dd/MM/yyyy HH:mm
    string diemDon;
    string diemDen;
    double khoangCach = 0;
    double cuocPhi = 0;
    TrangThaiChuyen trangThai = DANG_DI;
    string maTaiXe;

public:
    ChuyenXe() {}
    ChuyenXe(string ma, string ten, string sdt, string bienSo,
              string thoiDiemChuyen, string don, string den, double kc, string maTX = "")
        : maChuyen(ma), tenKhachHang(ten), sdtKhachHang(sdt), bienSoTaxi(bienSo),
          thoiDiem(thoiDiemChuyen), diemDon(don), diemDen(den), khoangCach(kc), maTaiXe(maTX) {
        cuocPhi = tinhCuocPhi();
    }

    double tinhCuocPhi() const {
        const double GIA_MO_CUA = 11000.0;
        const double DON_GIA_KM = 15000.0;
        return GIA_MO_CUA + khoangCach * DON_GIA_KM;
    }

    const string& getMaChuyen() const { return maChuyen; }
    string getTenKhachHang() const { return tenKhachHang; }
    string getSdtKhachHang() const { return sdtKhachHang; }
    const string& getBienSoTaxi() const { return bienSoTaxi; }
    const string& getThoiDiem() const { return thoiDiem; }
    string getDiemDon() const { return diemDon; }
    string getDiemDen() const { return diemDen; }
    double getKhoangCach() const { return khoangCach; }
    double getCuocPhi() const { return cuocPhi; }
    void setCuocPhi(double c) { cuocPhi = c; }
    TrangThaiChuyen getTrangThai() const { return trangThai; }
    void setTrangThai(TrangThaiChuyen t) { trangThai = t; }
    const string& getMaTaiXe() const { return maTaiXe; }
    void setMaTaiXe(const string& m) { maTaiXe = m; }
};


#endif // CHUYENXE_H
