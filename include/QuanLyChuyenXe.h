#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <locale>
#include "ChuyenXe.h"
#include "Common.h"

using namespace std;

class QuanLyChuyenXe {
private:
    DanhSach<ChuyenXe> dsChuyenXe;
    string duongDanFile;

    static string thoiDiemHienTai() {
        time_t t = time(nullptr);
        tm* now = localtime(&t);
        char buf[32];
        if (now && strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M", now) > 0)
            return string(buf);
        return "";
    }

    static double parseDoubleClassic(const string& s) {
        istringstream iss(s);
        iss.imbue(locale::classic());
        double val = 0;
        iss >> val;
        return val;
    }

public:
    explicit QuanLyChuyenXe(string file = "data/chuyenxe.txt") : duongDanFile(file) {}

    const DanhSach<ChuyenXe>& layDanhSach() const { return dsChuyenXe; }

    void themChuyen(const ChuyenXe& cx) { dsChuyenXe.push_back(cx); ghiFile(); }

    string sinhMaMoi() const {
        int maxv = 0;
        for (auto& c : dsChuyenXe) {
            string ma = c.getMaChuyen();
            if (ma.size() > 2) {
                maxv = max(maxv, stoi(ma.substr(2))); 
            }
        }

        ostringstream oss;
        oss << "CX" << setfill('0') << setw(4) << maxv + 1;
        return oss.str();
    }

    DanhSach<ChuyenXe> theoTaxi(const string& bienSo) const {
        DanhSach<ChuyenXe> ketQua;
        for (auto& c : dsChuyenXe) if (c.getBienSoTaxi() == bienSo) ketQua.push_back(c);
        return ketQua;
    }
    
    ChuyenXe* timChuyenDangDi(const string& bienSo) {
        for (int i = dsChuyenXe.size() - 1; i >= 0; --i) {
            if (dsChuyenXe[i].getBienSoTaxi() == bienSo && dsChuyenXe[i].getTrangThai() == DANG_DI)
                return &dsChuyenXe[i];
        }
        return nullptr;
    }
    
    bool taiXeDangChay(const string& maTX) const {
        for (int i = dsChuyenXe.size() - 1; i >= 0; --i)
            if (dsChuyenXe[i].getMaTaiXe() == maTX && dsChuyenXe[i].getTrangThai() == DANG_DI) return true;
        return false;
    }

    bool taxiDangChay(const string& bienSo) const {
        for (int i = dsChuyenXe.size() - 1; i >= 0; --i)
            if (dsChuyenXe[i].getBienSoTaxi() == bienSo && dsChuyenXe[i].getTrangThai() == DANG_DI) return true;
        return false;
    }
    
    bool hoanThanhChuyen(const string& maChuyen, const string& thoiDiemKT = "") {
        for (int i = dsChuyenXe.size() - 1; i >= 0; --i) {
            if (dsChuyenXe[i].getMaChuyen() == maChuyen) {
                dsChuyenXe[i].setTrangThai(HOAN_THANH);
                dsChuyenXe[i].setThoiDiemKetThuc(thoiDiemKT.empty() ? thoiDiemHienTai() : thoiDiemKT);
                ghiFile();
                return true;
            }
        }
        return false;
    }
  
    bool huyChuyen(const string& maChuyen, const string& thoiDiemKT = "") {
        for (int i = dsChuyenXe.size() - 1; i >= 0; --i) {
            if (dsChuyenXe[i].getMaChuyen() == maChuyen) {
                dsChuyenXe[i].setTrangThai(DA_HUY);
                dsChuyenXe[i].setThoiDiemKetThuc(thoiDiemKT.empty() ? thoiDiemHienTai() : thoiDiemKT);
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
            ChuyenXe c(p[0], p[1], p[2], p[3], p[4], p[5], p[6], parseDoubleClassic(p[7]));
            c.setCuocPhi(parseDoubleClassic(p[8]));
            if (p.size() >= 10) c.setTrangThai((TrangThaiChuyen)stoi(p[9]));
            if (p.size() >= 11) c.setMaTaiXe(p[10]);
            if (p.size() >= 12) c.setThoiDiemKetThuc(p[11]);
            dsChuyenXe.push_back(c);
        }
    }

    void ghiFile() const {
        ofstream fout(duongDanFile, ios::trunc);
        fout.imbue(locale::classic());
        for (auto& c : dsChuyenXe) {
            fout << c.getMaChuyen() << "|" << c.getTenKhachHang() << "|" << c.getSdtKhachHang()
                 << "|" << c.getBienSoTaxi() << "|" << c.getThoiDiem() << "|" << c.getDiemDon()
                 << "|" << c.getDiemDen() << "|" << c.getKhoangCach() << "|" << c.getCuocPhi() 
                 << "|" << (int)c.getTrangThai()  << "|" << c.getMaTaiXe()
                 << (c.getThoiDiemKetThuc().empty() ? "" : "|" + c.getThoiDiemKetThuc()) << "\n";
        }
    }
};