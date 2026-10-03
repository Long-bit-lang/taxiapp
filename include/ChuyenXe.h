#ifndef CHUYENXE_H
#define CHUYENXE_H

#include <string>

<<<<<<< HEAD
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
=======
class ChuyenXe {
private:
    std::string maChuyen;
    std::string tenKhachHang;
    std::string sdtKhachHang;
    std::string bienSoTaxi;
    std::string thoiDiem;   // dd/MM/yyyy HH:mm
    std::string diemDon;
    std::string diemDen;
    double khoangCach = 0;
    double cuocPhi = 0;
    TrangThaiChuyen trangThai = DANG_DI;
    std::string maTaiXe;

public:
    ChuyenXe() {}
    ChuyenXe(std::string ma, std::string ten, std::string sdt, std::string bienSo,
              std::string thoiDiemChuyen, std::string don, std::string den, double kc, std::string maTX = "")
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
        : maChuyen(ma), tenKhachHang(ten), sdtKhachHang(sdt), bienSoTaxi(bienSo),
          thoiDiem(thoiDiemChuyen), diemDon(don), diemDen(den), khoangCach(kc), maTaiXe(maTX) {
        cuocPhi = tinhCuocPhi();
    }

    double tinhCuocPhi() const {
        const double GIA_MO_CUA = 11000.0;
        const double DON_GIA_KM = 15000.0;
        return GIA_MO_CUA + khoangCach * DON_GIA_KM;
    }

<<<<<<< HEAD
    string getMaChuyen() const { return maChuyen; }
    string getTenKhachHang() const { return tenKhachHang; }
    string getSdtKhachHang() const { return sdtKhachHang; }
    string getBienSoTaxi() const { return bienSoTaxi; }
    string getThoiDiem() const { return thoiDiem; }
    string getDiemDon() const { return diemDon; }
    string getDiemDen() const { return diemDen; }
=======
    std::string getMaChuyen() const { return maChuyen; }
    std::string getTenKhachHang() const { return tenKhachHang; }
    std::string getSdtKhachHang() const { return sdtKhachHang; }
    std::string getBienSoTaxi() const { return bienSoTaxi; }
    std::string getThoiDiem() const { return thoiDiem; }
    std::string getDiemDon() const { return diemDon; }
    std::string getDiemDen() const { return diemDen; }
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
    double getKhoangCach() const { return khoangCach; }
    double getCuocPhi() const { return cuocPhi; }
    void setCuocPhi(double c) { cuocPhi = c; }
    TrangThaiChuyen getTrangThai() const { return trangThai; }
    void setTrangThai(TrangThaiChuyen t) { trangThai = t; }
<<<<<<< HEAD
    string getMaTaiXe() const { return maTaiXe; }
    void setMaTaiXe(const string& m) { maTaiXe = m; }
=======
    std::string getMaTaiXe() const { return maTaiXe; }
    void setMaTaiXe(const std::string& m) { maTaiXe = m; }
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
};


#endif // CHUYENXE_H
