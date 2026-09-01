#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

struct Date {
    int day = 1, month = 1, year = 2000;
    std::string toString() const {
        auto pad = [](int v) { return (v < 10 ? "0" : "") + std::to_string(v); };
        return pad(day) + "/" + pad(month) + "/" + std::to_string(year);
    }
};

enum TrangThai { DANG_HOAT_DONG = 0, NGHI = 1, NGUNG_HOAT_DONG = 2 };
enum ChucNang { CN_QUAN_LY_TAI_XE, CN_QUAN_LY_TAXI, CN_DAT_CHUYEN_XE, CN_THONG_KE };


inline std::string trangThaiToStr(TrangThai t) {
    switch (t) {
        case DANG_HOAT_DONG: return "Đang hoạt động";
        case NGHI: return "Nghỉ";
        default: return "Ngừng hoạt động";
    }
}

inline bool hangBangLaiHopLeTaxi(const std::string& bangLai) {
    static const std::vector<std::string> hopLe = {"B2", "C", "D", "E"};
    return std::find(hopLe.begin(), hopLe.end(), bangLai) != hopLe.end();
}

inline bool hangBangLaiPhuHopSucChua(const std::string& bangLai, int sucChua) {
    if (bangLai == "B2") return sucChua <= 8;
    if (bangLai == "D")  return sucChua <= 29;
    if (bangLai == "E")  return true;
    if (bangLai == "C")  return true;
    return false;
}

enum VaiTro { QUAN_LY = 0, DIEU_HANH = 1 };

enum TrangThaiChuyen { DANG_DI = 0, HOAN_THANH = 1, DA_HUY = 2 };

inline std::string trangThaiChuyenToStr(TrangThaiChuyen t) {
    switch (t) {
        case DANG_DI: return "Đang đi";
        case HOAN_THANH: return "Hoàn thành";
        default: return "Đã hủy";
    }
}

inline std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) out.push_back(item);
    return out;
}
#endif // COMMON_H
