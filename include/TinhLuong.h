#ifndef TINHLUONG_H
#define TINHLUONG_H

#include <map>
#include <vector>
#include <string>
#include <cstdio>
#include <algorithm>
#include "Common.h"
#include "ChuyenXe.h"
#include "QuanLyChuyenXe.h"
#include "QuanLyTaxi.h"

using namespace std;

struct CauHinhLuong {
    double tyLeChietKhau = 0.25;
    double phiNenTang4Cho = 5000;
    double phiNenTang7Cho = 8000;
    double tyLeThue = 0.0;
};

struct LuongTaiXe {
    string maTX;
    int soChuyen = 0;
    double tongKm = 0;
    double doanhThu = 0;
    double chietKhau = 0;
    double phiNenTang = 0;
    double thuong = 0;
    double thue = 0;
    double thucNhan = 0;
};

// Thuong theo so chuyen hoan thanh trong ngay (mo phong thuong ngay cua Grab)
inline double tyLeThuongNgay(int soChuyen) {
    if (soChuyen >= 8) return 0.06;
    if (soChuyen >= 5) return 0.04;
    if (soChuyen >= 3) return 0.02;
    return 0.0;
}

// thang = 0: ca nam
inline bool thuocThang(const ChuyenXe& c, int thang, int nam) {
    int d, m, y;
    if (sscanf(c.getThoiDiem().c_str(), "%d/%d/%d", &d, &m, &y) != 3) return false;
    if (y != nam) return false;
    return thang == 0 || m == thang;
}

inline vector<LuongTaiXe> tinhBangLuong(const QuanLyChuyenXe& qlCX, QuanLyTaxi& qlTaxi,
                                        int thang, int nam,
                                        const CauHinhLuong& ch = CauHinhLuong()) {
    map<string, LuongTaiXe> bang;
    map<pair<string, string>, pair<int, double>> theoNgay; // (maTX, ngay) -> (so chuyen, doanh thu)

    for (const auto& c : qlCX.layDanhSach()) {
        if (c.getTrangThai() != HOAN_THANH) continue;
        if (c.getMaTaiXe().empty() || c.getThoiDiem().size() < 10) continue;
        if (!thuocThang(c, thang, nam)) continue;

        Taxi* t = qlTaxi.timTheoBienSo(c.getBienSoTaxi());
        int sucChua = t ? t->getSucChua() : 4;      // taxi khong con trong taxi.txt -> coi nhu 4 cho
        double cuoc = c.getCuocPhi();

        LuongTaiXe& l = bang[c.getMaTaiXe()];
        l.maTX = c.getMaTaiXe();
        l.soChuyen++;
        l.tongKm += c.getKhoangCach();
        l.doanhThu += cuoc;
        l.chietKhau += cuoc * ch.tyLeChietKhau;
        l.phiNenTang += (sucChua >= 7) ? ch.phiNenTang7Cho : ch.phiNenTang4Cho;

        auto& n = theoNgay[{c.getMaTaiXe(), c.getThoiDiem().substr(0, 10)}];
        n.first++;
        n.second += cuoc;
    }

    for (const auto& kv : theoNgay)
        bang[kv.first.first].thuong += kv.second.second * tyLeThuongNgay(kv.second.first);

    vector<LuongTaiXe> kq;
    for (auto& kv : bang) {
        LuongTaiXe& l = kv.second;
        double truocThue = l.doanhThu - l.chietKhau - l.phiNenTang + l.thuong;
        l.thue = max(0.0, truocThue) * ch.tyLeThue;
        l.thucNhan = truocThue - l.thue;
        kq.push_back(l);
    }
    return kq;
}

#endif // TINHLUONG_H
