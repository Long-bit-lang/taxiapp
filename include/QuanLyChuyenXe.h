#ifndef QUANLYCHUYENXE_H
#define QUANLYCHUYENXE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "ChuyenXe.h"
#include "Common.h"

<<<<<<< HEAD
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
=======
class QuanLyChuyenXe {
private:
    std::vector<ChuyenXe> dsChuyenXe;
    std::string duongDanFile;

public:
    explicit QuanLyChuyenXe(std::string file = "data/chuyenxe.txt") : duongDanFile(file) {}

    const std::vector<ChuyenXe>& layDanhSach() const { return dsChuyenXe; }

    void themChuyen(const ChuyenXe& cx) { dsChuyenXe.push_back(cx); ghiFile(); }

    std::string sinhMaMoi() const {
        int maxv = 0;
        for (auto& c : dsChuyenXe) {
            std::string ma = c.getMaChuyen();
            if (ma.size() > 2) maxv = std::max(maxv, atoi(ma.substr(2).c_str()));
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
        }
        char buf[16];
        snprintf(buf, sizeof(buf), "CX%04d", maxv + 1);
        return buf;
    }

<<<<<<< HEAD
    vector<ChuyenXe> theoTaxi(const string& bienSo) const {
        vector<ChuyenXe> ketQua;
=======
    std::vector<ChuyenXe> theoTaxi(const std::string& bienSo) const {
        std::vector<ChuyenXe> ketQua;
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
        for (auto& c : dsChuyenXe) if (c.getBienSoTaxi() == bienSo) ketQua.push_back(c);
        return ketQua;
    }
    
<<<<<<< HEAD
    ChuyenXe* timChuyenDangDi(const string& bienSo) {
=======
    ChuyenXe* timChuyenDangDi(const std::string& bienSo) {
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
        for (auto& c : dsChuyenXe) {
            if (c.getBienSoTaxi() == bienSo && c.getTrangThai() == DANG_DI) return &c;
        }
        return nullptr;
    }
    
<<<<<<< HEAD
    bool taiXeDangChay(const string& maTX) const {
=======
    bool taiXeDangChay(const std::string& maTX) const {
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
        for (auto& c : dsChuyenXe)
            if (c.getMaTaiXe() == maTX && c.getTrangThai() == DANG_DI) return true;
        return false;
    }

<<<<<<< HEAD
    bool taxiDangChay(const string& bienSo) const {
=======
    bool taxiDangChay(const std::string& bienSo) const {
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
        for (auto& c : dsChuyenXe)
            if (c.getBienSoTaxi() == bienSo && c.getTrangThai() == DANG_DI) return true;
        return false;
    }
    
<<<<<<< HEAD
    bool hoanThanhChuyen(const string& maChuyen) {
=======
    bool hoanThanhChuyen(const std::string& maChuyen) {
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
        for (auto& c : dsChuyenXe) {
            if (c.getMaChuyen() == maChuyen) {
                c.setTrangThai(HOAN_THANH);
                ghiFile();
                return true;
            }
        }
        return false;
    }
  
<<<<<<< HEAD
    bool huyChuyen(const string& maChuyen) {
=======
    bool huyChuyen(const std::string& maChuyen) {
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
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
<<<<<<< HEAD
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
=======
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
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
            if (p.size() >= 11) c.setMaTaiXe(p[10]);
            dsChuyenXe.push_back(c);
        }
    }

    void ghiFile() const {
<<<<<<< HEAD
        ofstream fout(duongDanFile, ios::trunc);
=======
        std::ofstream fout(duongDanFile, std::ios::trunc);
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
        for (auto& c : dsChuyenXe) {
            fout << c.getMaChuyen() << "|" << c.getTenKhachHang() << "|" << c.getSdtKhachHang()
                 << "|" << c.getBienSoTaxi() << "|" << c.getThoiDiem() << "|" << c.getDiemDon()
                 << "|" << c.getDiemDen() << "|" << c.getKhoangCach() << "|" << c.getCuocPhi() 
                 << "|" << (int)c.getTrangThai()  << "|" << c.getMaTaiXe() << "\n";
        }
    }
};

#endif // QUANLYCHUYENXE_H
