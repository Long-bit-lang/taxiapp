#ifndef TAXI_H
#define TAXI_H

#include <string>
#include <vector>
#include <algorithm>

class Taxi {
private:
    std::string bienSo;
    int sucChua;
    std::vector<std::string> dsMaTaiXe;
    bool dangSanSang;

public:
    Taxi() : sucChua(4), dangSanSang(true) {}
    Taxi(std::string bs, int sc) : bienSo(bs), sucChua(sc), dangSanSang(true) {}

    std::string getBienSo() const { return bienSo; }
    int getSucChua() const { return sucChua; }
    const std::vector<std::string>& getDsMaTaiXe() const { return dsMaTaiXe; }
    bool kiemTraSanSang() const { return dangSanSang; }
    std::string dsMaTaiXeStr() const {
        std::string s;
        for (size_t i = 0; i < dsMaTaiXe.size(); ++i) {
            s += dsMaTaiXe[i];
            if (i + 1 < dsMaTaiXe.size()) s += ",";
        }
        return s;
    }

    void setSucChua(int sc) { sucChua = sc; }
    void setSanSang(bool ss) { dangSanSang = ss; }

    void themTaiXe(const std::string& maTX) {
        if (std::find(dsMaTaiXe.begin(), dsMaTaiXe.end(), maTX) == dsMaTaiXe.end())
            dsMaTaiXe.push_back(maTX);
    }
    void xoaTaiXe(const std::string& maTX) {
        dsMaTaiXe.erase(std::remove(dsMaTaiXe.begin(), dsMaTaiXe.end(), maTX), dsMaTaiXe.end());
    }
};

#endif // TAXI_H
