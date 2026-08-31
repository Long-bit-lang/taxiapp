#ifndef CHUYENXE_H
#define CHUYENXE_H

#include <string>

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
        : maChuyen(ma), tenKhachHang(ten), sdtKhachHang(sdt), bienSoTaxi(bienSo),
          thoiDiem(thoiDiemChuyen), diemDon(don), diemDen(den), khoangCach(kc), maTaiXe(maTX) {
        cuocPhi = tinhCuocPhi();
    }

    double tinhCuocPhi() const {
        const double GIA_MO_CUA = 11000.0;
        const double DON_GIA_KM = 15000.0;
        return GIA_MO_CUA + khoangCach * DON_GIA_KM;
    }

    std::string getMaChuyen() const { return maChuyen; }
    std::string getTenKhachHang() const { return tenKhachHang; }
    std::string getSdtKhachHang() const { return sdtKhachHang; }
    std::string getBienSoTaxi() const { return bienSoTaxi; }
    std::string getThoiDiem() const { return thoiDiem; }
    std::string getDiemDon() const { return diemDon; }
    std::string getDiemDen() const { return diemDen; }
    double getKhoangCach() const { return khoangCach; }
    double getCuocPhi() const { return cuocPhi; }
    void setCuocPhi(double c) { cuocPhi = c; }
    TrangThaiChuyen getTrangThai() const { return trangThai; }
    void setTrangThai(TrangThaiChuyen t) { trangThai = t; }
    std::string getMaTaiXe() const { return maTaiXe; }
    void setMaTaiXe(const std::string& m) { maTaiXe = m; }
};


#endif // CHUYENXE_H
