#ifndef QUANLYTAXI_H
#define QUANLYTAXI_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Taxi.h"
#include "Common.h"

using namespace std;


class QuanLyTaxi {
private:
    vector<Taxi> dsTaxi;
    string duongDanFile;

public:
    explicit QuanLyTaxi(string file = "data/taxi.txt") : duongDanFile(file) {}

    vector<Taxi>& layDanhSach() { return dsTaxi; }
    const vector<Taxi>& layDanhSach() const { return dsTaxi; }

    void them(const Taxi& t) { dsTaxi.push_back(t); ghiFile(); }

    bool xoa(const string& bienSo) {
        auto it = remove_if(dsTaxi.begin(), dsTaxi.end(),
                                  [&](const Taxi& t) { return t.getBienSo() == bienSo; });
        if (it == dsTaxi.end()) return false;
        dsTaxi.erase(it, dsTaxi.end());
        ghiFile();
        return true;
    }

    void goBoTaiXeKhoiTatCaTaxi(const string& maTX) {
        for (auto& t : dsTaxi) t.xoaTaiXe(maTX);
        ghiFile();
    }
    
    bool coBienSo(const string& bienSo) const {
        for (auto& t : dsTaxi) if (t.getBienSo() == bienSo) return true;
        return false;
    }

    Taxi* timTheoBienSo(const string& bienSo) {
        for (auto& t : dsTaxi) if (t.getBienSo() == bienSo) return &t;
        return nullptr;
    }

    // Loc taxi dang san sang, uu tien... (o day minh hoa: sap xep theo bien so)
    vector<Taxi*> timTaxiPhuHop(int soKhachToiThieu = 1) {
        vector<Taxi*> ketQua;
        for (auto& t : dsTaxi)
            if (t.kiemTraSanSang() && t.getSucChua() >= soKhachToiThieu)
                ketQua.push_back(&t);
        sort(ketQua.begin(), ketQua.end(),
                  [](Taxi* a, Taxi* b) { return a->getBienSo() < b->getBienSo(); });
        return ketQua;
    }

    void docFile() {
        dsTaxi.clear();
        ifstream fin(duongDanFile);
        if (!fin.is_open()) return;
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            auto p = split(line, '|');
            if (p.size() < 2) continue;
            Taxi t(p[0], stoi(p[1]));
            if (p.size() >= 3 && !p[2].empty()) {
                for (auto& ma : split(p[2], ',')) t.themTaiXe(ma);
            }
            if (p.size() >= 4) t.setSanSang(p[3] == "1");
            dsTaxi.push_back(t);
        }
    }

    void ghiFile() const {
        ofstream fout(duongDanFile, ios::trunc);
        for (auto& t : dsTaxi) {
            fout << t.getBienSo() << "|" << t.getSucChua() << "|" << t.dsMaTaiXeStr()
                 << "|" << (t.kiemTraSanSang() ? 1 : 0) << "\n";
        }
    }
};

#endif // QUANLYTAXI_H
