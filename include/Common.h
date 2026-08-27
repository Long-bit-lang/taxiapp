#ifndef COMMON_H
#define COMMON_H

#include <string>

struct Date {
    int day = 1, month = 1, year = 2000;
    std::string toString() const {
        auto pad = [](int v) { return (v < 10 ? "0" : "") + std::to_string(v); };
        return pad(day) + "/" + pad(month) + "/" + std::to_string(year);
    }
};

enum TrangThai { DANG_HOAT_DONG = 0, NGHI = 1, NGUNG_HOAT_DONG = 2 };

inline std::string trangThaiToStr(TrangThai t) {
    switch (t) {
        case DANG_HOAT_DONG: return "Dang hoat dong";
        case NGHI: return "Nghi";
        default: return "Ngung hoat dong";
    }
}

enum VaiTro { QUAN_LY = 0, DIEU_HANH = 1 };

enum TrangThaiChuyen { DANG_DI = 0, HOAN_THANH = 1, DA_HUY = 2 };

inline std::string trangThaiChuyenToStr(TrangThaiChuyen t) {
    switch (t) {
        case DANG_DI: return "Dang di";
        case HOAN_THANH: return "Hoan thanh";
        default: return "Da huy";
    }
}

#endif // COMMON_H
