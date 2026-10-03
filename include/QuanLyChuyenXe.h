#ifndef QUANLYCHUYENXE_H
#define QUANLYCHUYENXE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "ChuyenXe.h"
#include "Common.h"

using namespace std;

class QuanLyChuyenXe {
private:
    vector<ChuyenXe> dsChuyenXe;
    string duongDanFile;

public:
    explicit QuanLyChuyenXe(string file = "data/chuyenxe.txt") : duongDanFile(file) {}

    const vector<ChuyenXe>& layDanhSach() const { return dsChuyenXe; }

    void themChuyen(const ChuyenXe& cx) { dsChuyenXe.push_back(cx); ghiFile(); }

    string sinhMaMoi() const {
        int maxv = 0;
        for (auto& c : dsChuyenXe) {
            string ma = c.getMaChuyen();
            if (ma.size() > 2) maxv = max(maxv, atoi(ma.substr(2).c_str()));
        }
        char buf[16];
        snprintf(buf, sizeof(buf), "CX%04d", maxv + 1);
        return buf;
    }

    vector<ChuyenXe> theoTaxi(const string& bienSo) const {
        vector<ChuyenXe> ketQua;
        for (auto& c : dsChuyenXe) if (c.getBienSoTaxi() == bienSo) ketQua.push_back(c);
        return ketQua;
    }
    
    ChuyenXe* timChuyenDangDi(const string& bienSo) {
        for (auto& c : dsChuyenXe) {
            if (c.getBienSoTaxi() == bienSo && c.getTrangThai() == DANG_DI) return &c;
        }
        return nullptr;
    }
    
    bool taiXeDangChay(const string& maTX) const {
        for (auto& c : dsChuyenXe)
            if (c.getMaTaiXe() == maTX && c.getTrangThai() == DANG_DI) return true;
        return false;
    }

    bool taxiDangChay(const string& bienSo) const {
        for (auto& c : dsChuyenXe)
            if (c.getBienSoTaxi() == bienSo && c.getTrangThai() == DANG_DI) return true;
        return false;
    }
    
    bool hoanThanhChuyen(const string& maChuyen) {
        for (auto& c : dsChuyenXe) {
            if (c.getMaChuyen() == maChuyen) {
                c.setTrangThai(HOAN_THANH);
                ghiFile();
                return true;
            }
        }
        return false;
    }
  
    bool huyChuyen(const string& maChuyen) {
        for (auto& c : dsChuyenXe) {
            if (c.getMaChuyen() == maChuyen) {
                c.setTrangThai(DA_HUY);
                ghiFile();
                return true;
            }
        }
        return false;
    }

    double tongDoanhThu() const {
        double tong = 0;
        for (auto& c : dsChuyenXe)
            if (c.getTrangThai() != DA_HUY) tong += c.getCuocPhi();
        return tong;
    }

    void docFile() {
        dsChuyenXe.clear();
        ifstream fin(duongDanFile);
        if (!fin.is_open()) return;
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            auto p = split(line, '|');
            if (p.size() < 9) continue;
            ChuyenXe c(p[0], p[1], p[2], p[3], p[4], p[5], p[6], stod(p[7]));
            c.setCuocPhi(stod(p[8]));
            if (p.size() >= 10) c.setTrangThai((TrangThaiChuyen)stoi(p[9]));
            if (p.size() >= 11) c.setMaTaiXe(p[10]);
            dsChuyenXe.push_back(c);
        }
    }

    void ghiFile() const {
        ofstream fout(duongDanFile, ios::trunc);
        for (auto& c : dsChuyenXe) {
            fout << c.getMaChuyen() << "|" << c.getTenKhachHang() << "|" << c.getSdtKhachHang()
                 << "|" << c.getBienSoTaxi() << "|" << c.getThoiDiem() << "|" << c.getDiemDon()
                 << "|" << c.getDiemDen() << "|" << c.getKhoangCach() << "|" << c.getCuocPhi() 
                 << "|" << (int)c.getTrangThai()  << "|" << c.getMaTaiXe() << "\n";
        }
    }
};

#endif // QUANLYCHUYENXE_H
