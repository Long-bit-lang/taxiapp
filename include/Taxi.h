#ifndef TAXI_H
#define TAXI_H

#include <string>
#include <vector>
#include <algorithm>

<<<<<<< HEAD
using namespace std;


class Taxi {
private:
    string bienSo;
    int sucChua;
    vector<string> dsMaTaiXe;
=======
class Taxi {
private:
    std::string bienSo;
    int sucChua;
    std::vector<std::string> dsMaTaiXe;
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
    bool dangSanSang;

public:
    Taxi() : sucChua(4), dangSanSang(true) {}
<<<<<<< HEAD
    Taxi(string bs, int sc) : bienSo(bs), sucChua(sc), dangSanSang(true) {}

    string getBienSo() const { return bienSo; }
    int getSucChua() const { return sucChua; }
    const vector<string>& getDsMaTaiXe() const { return dsMaTaiXe; }
    bool kiemTraSanSang() const { return dangSanSang; }
    string dsMaTaiXeStr() const {
        string s;
=======
    Taxi(std::string bs, int sc) : bienSo(bs), sucChua(sc), dangSanSang(true) {}

    std::string getBienSo() const { return bienSo; }
    int getSucChua() const { return sucChua; }
    const std::vector<std::string>& getDsMaTaiXe() const { return dsMaTaiXe; }
    bool kiemTraSanSang() const { return dangSanSang; }
    std::string dsMaTaiXeStr() const {
        std::string s;
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
        for (size_t i = 0; i < dsMaTaiXe.size(); ++i) {
            s += dsMaTaiXe[i];
            if (i + 1 < dsMaTaiXe.size()) s += ",";
        }
        return s;
    }

    void setSanSang(bool ss) { dangSanSang = ss; }

<<<<<<< HEAD
    void themTaiXe(const string& maTX) {
        if (find(dsMaTaiXe.begin(), dsMaTaiXe.end(), maTX) == dsMaTaiXe.end())
            dsMaTaiXe.push_back(maTX);
    }
    void xoaTaiXe(const string& maTX) {
        dsMaTaiXe.erase(remove(dsMaTaiXe.begin(), dsMaTaiXe.end(), maTX), dsMaTaiXe.end());
=======
    void themTaiXe(const std::string& maTX) {
        if (std::find(dsMaTaiXe.begin(), dsMaTaiXe.end(), maTX) == dsMaTaiXe.end())
            dsMaTaiXe.push_back(maTX);
    }
    void xoaTaiXe(const std::string& maTX) {
        dsMaTaiXe.erase(std::remove(dsMaTaiXe.begin(), dsMaTaiXe.end(), maTX), dsMaTaiXe.end());
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
    }
};

#endif // TAXI_H
