#ifndef QUANLYCHUYENXE_H
#define QUANLYCHUYENXE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "ChuyenXe.h"

class QuanLyChuyenXe {
private:
    std::vector<ChuyenXe> dsChuyenXe;
    std::string duongDanFile;

    static std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> out;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) out.push_back(item);
        return out;
    }

public:
    explicit QuanLyChuyenXe(std::string file = "data/chuyenxe.txt") : duongDanFile(file) {}

    const std::vector<ChuyenXe>& layDanhSach() const { return dsChuyenXe; }

    void themChuyen(const ChuyenXe& cx) { dsChuyenXe.push_back(cx); ghiFile(); }

    std::string sinhMaMoi() const {
        int maxv = 0;
        for (auto& c : dsChuyenXe) {
            std::string ma = c.getMaChuyen();
            if (ma.size() > 2) maxv = std::max(maxv, atoi(ma.substr(2).c_str()));
        }
        char buf[16];
        snprintf(buf, sizeof(buf), "CX%04d", maxv + 1);
        return buf;
    }

    std::vector<ChuyenXe> theoTaxi(const std::string& bienSo) const {
        std::vector<ChuyenXe> ketQua;
        for (auto& c : dsChuyenXe) if (c.getBienSoTaxi() == bienSo) ketQua.push_back(c);
        return ketQua;
    }
    
    ChuyenXe* timChuyenDangDi(const std::string& bienSo) {
        for (auto& c : dsChuyenXe) {
            if (c.getBienSoTaxi() == bienSo && c.getTrangThai() == DANG_DI) return &c;
        }
        return nullptr;
    }
    
    bool taiXeDangChay(const std::string& maTX) const {
        for (auto& c : dsChuyenXe)
            if (c.getMaTaiXe() == maTX && c.getTrangThai() == DANG_DI) return true;
        return false;
    }

    bool taxiDangChay(const std::string& bienSo) const {
        for (auto& c : dsChuyenXe)
            if (c.getBienSoTaxi() == bienSo && c.getTrangThai() == DANG_DI) return true;
        return false;
    }
    
    bool hoanThanhChuyen(const std::string& maChuyen) {
        for (auto& c : dsChuyenXe) {
            if (c.getMaChuyen() == maChuyen) {
                c.setTrangThai(HOAN_THANH);
                ghiFile();
                return true;
            }
        }
        return false;
    }
  
    bool huyChuyen(const std::string& maChuyen) {
        for (auto& c : dsChuyenXe) {
            if (c.getMaChuyen() == maChuyen) {
                c.setTrangThai(DA_HUY);
                ghiFile();
                return true;
            }
        }
        return false;
    }

    double tinhTongCuocPhi(const std::string& bienSo) const {
        double tong = 0;
        for (auto& c : theoTaxi(bienSo))
            if (c.getTrangThai() != DA_HUY) tong += c.getCuocPhi();
        return tong;
    }

    double tongDoanhThu() const {
        double tong = 0;
        for (auto& c : dsChuyenXe)
            if (c.getTrangThai() != DA_HUY) tong += c.getCuocPhi();
        return tong;
    }

    std::vector<ChuyenXe> sapXepTheoCuocPhiGiam() const {
        auto ds = dsChuyenXe;
        std::sort(ds.begin(), ds.end(),
                  [](const ChuyenXe& a, const ChuyenXe& b) { return a.getCuocPhi() > b.getCuocPhi(); });
        return ds;
    }

    void docFile() {
        dsChuyenXe.clear();
        std::ifstream fin(duongDanFile);
        if (!fin.is_open()) return;
        std::string line;
        while (std::getline(fin, line)) {
            if (line.empty()) continue;
            auto p = split(line, '|');
            if (p.size() < 9) continue;
            ChuyenXe c(p[0], p[1], p[2], p[3], p[4], p[5], p[6], std::stod(p[7]));
            c.setCuocPhi(std::stod(p[8]));
            if (p.size() >= 10) c.setTrangThai((TrangThaiChuyen)std::stoi(p[9]));
            if (p.size() >= 11) c.setMaTaiXe(p[10]);
            dsChuyenXe.push_back(c);
        }
    }

    void ghiFile() const {
        std::ofstream fout(duongDanFile, std::ios::trunc);
        for (auto& c : dsChuyenXe) {
            fout << c.getMaChuyen() << "|" << c.getTenKhachHang() << "|" << c.getSdtKhachHang()
                 << "|" << c.getBienSoTaxi() << "|" << c.getThoiDiem() << "|" << c.getDiemDon()
                 << "|" << c.getDiemDen() << "|" << c.getKhoangCach() << "|" << c.getCuocPhi() 
                 << "|" << (int)c.getTrangThai()  << "|" << c.getMaTaiXe() << "\n";
        }
    }
};

#endif // QUANLYCHUYENXE_H
