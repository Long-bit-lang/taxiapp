#ifndef TAXI_H
#define TAXI_H

#include <string>
#include <algorithm>
#include "CauTruc.h"

using namespace std;


class Taxi {
private:
    string bienSo;
    int sucChua;
    DanhSach<string> dsMaTaiXe;
    bool dangSanSang;

public:
    Taxi() : sucChua(4), dangSanSang(true) {}
    Taxi(string bs, int sc) : bienSo(bs), sucChua(sc), dangSanSang(true) {}

    string getBienSo() const { return bienSo; }
    int getSucChua() const { return sucChua; }
    const DanhSach<string>& getDsMaTaiXe() const { return dsMaTaiXe; }
    bool kiemTraSanSang() const { return dangSanSang; }
    string dsMaTaiXeStr() const {
        string s;
        for (int i = 0; i < dsMaTaiXe.size(); ++i) {
            s += dsMaTaiXe[i];
            if (i + 1 < dsMaTaiXe.size()) s += ",";
        }
        return s;
    }

    void setSanSang(bool ss) { dangSanSang = ss; }

    void themTaiXe(const string& maTX) {
        if (!dsMaTaiXe.chua(maTX)) dsMaTaiXe.themCuoi(maTX);
    }
    void xoaTaiXe(const string& maTX) {
        dsMaTaiXe.xoaGiaTri(maTX);
    }
};

#endif // TAXI_H
