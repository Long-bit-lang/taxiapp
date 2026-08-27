#ifndef QUANLYTAXI_H
#define QUANLYTAXI_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Taxi.h"

class QuanLyTaxi {
private:
    std::vector<Taxi> dsTaxi;
    std::string duongDanFile;

    static std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> out;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) out.push_back(item);
        return out;
    }

public:
    explicit QuanLyTaxi(std::string file = "data/taxi.txt") : duongDanFile(file) {}

    std::vector<Taxi>& layDanhSach() { return dsTaxi; }
    const std::vector<Taxi>& layDanhSach() const { return dsTaxi; }

    void them(const Taxi& t) { dsTaxi.push_back(t); ghiFile(); }

    bool xoa(const std::string& bienSo) {
        auto it = std::remove_if(dsTaxi.begin(), dsTaxi.end(),
                                  [&](const Taxi& t) { return t.getBienSo() == bienSo; });
        if (it == dsTaxi.end()) return false;
        dsTaxi.erase(it, dsTaxi.end());
        ghiFile();
        return true;
    }
    
    void goBoTaiXeKhoiTatCaTaxi(const std::string& maTX) {
        for (auto& t : dsTaxi) t.xoaTaiXe(maTX);
        ghiFile();
    }
    
    bool coBienSo(const std::string& bienSo) const {
        for (auto& t : dsTaxi) if (t.getBienSo() == bienSo) return true;
        return false;
    }

    Taxi* timTheoBienSo(const std::string& bienSo) {
        for (auto& t : dsTaxi) if (t.getBienSo() == bienSo) return &t;
        return nullptr;
    }

    // Loc taxi dang san sang, uu tien... (o day minh hoa: sap xep theo bien so)
    std::vector<Taxi*> timTaxiPhuHop(int soKhachToiThieu = 1) {
        std::vector<Taxi*> ketQua;
        for (auto& t : dsTaxi)
            if (t.kiemTraSanSang() && t.getSucChua() >= soKhachToiThieu)
                ketQua.push_back(&t);
        std::sort(ketQua.begin(), ketQua.end(),
                  [](Taxi* a, Taxi* b) { return a->getBienSo() < b->getBienSo(); });
        return ketQua;
    }

    void capNhatSauKhiGhi() { ghiFile(); }

    void docFile() {
        dsTaxi.clear();
        std::ifstream fin(duongDanFile);
        if (!fin.is_open()) return;
        std::string line;
        while (std::getline(fin, line)) {
            if (line.empty()) continue;
            auto p = split(line, '|');
            if (p.size() < 2) continue;
            Taxi t(p[0], std::stoi(p[1]));
            if (p.size() >= 3 && !p[2].empty()) {
                for (auto& ma : split(p[2], ',')) t.themTaiXe(ma);
            }
            if (p.size() >= 4) t.setSanSang(p[3] == "1");
            dsTaxi.push_back(t);
        }
    }

    void ghiFile() const {
        std::ofstream fout(duongDanFile, std::ios::trunc);
        for (auto& t : dsTaxi) {
            fout << t.getBienSo() << "|" << t.getSucChua() << "|" << t.dsMaTaiXeStr()
                 << "|" << (t.kiemTraSanSang() ? 1 : 0) << "\n";
        }
    }
};

#endif // QUANLYTAXI_H
