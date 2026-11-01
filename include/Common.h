#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <ctime>
#include "CauTruc.h"

using namespace std;

struct Date {
    int day = 1, month = 1, year = 2000;
    string toString() const {
        auto pad = [](int v) { return (v < 10 ? "0" : "") + to_string(v); };
        return pad(day) + "/" + pad(month) + "/" + to_string(year);
    }
};

enum TrangThai { DANG_HOAT_DONG = 0, NGHI = 1, NGUNG_HOAT_DONG = 2 };
enum ChucNang { CN_QUAN_LY_TAI_XE, CN_QUAN_LY_TAXI, CN_DAT_CHUYEN_XE, CN_CHUYEN_HOM_NAY, CN_THONG_KE };

inline int tinhTuoi(const Date& ngaySinh) {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    int namNay = now->tm_year + 1900;
    int thangNay = now->tm_mon + 1;
    int ngayNay = now->tm_mday;
    int tuoi = namNay - ngaySinh.year;
    if (thangNay < ngaySinh.month || (thangNay == ngaySinh.month && ngayNay < ngaySinh.day))
        tuoi--;
    return tuoi;
}

inline string trangThaiToStr(TrangThai t) {
    switch (t) {
        case DANG_HOAT_DONG: return "Đang hoạt động";
        case NGHI: return "Nghỉ";
        default: return "Ngừng hoạt động";
    }
}

inline bool hangBangLaiPhuHopSucChua(const string& bangLai, int sucChua) {
    if (bangLai == "B2") return sucChua <= 8;
    if (bangLai == "D")  return sucChua <= 29;
    if (bangLai == "E")  return true;
    if (bangLai == "C")  return true;
    return false;
}

enum VaiTro { QUAN_LY = 0, DIEU_HANH = 1 };

enum TrangThaiChuyen { DANG_DI = 0, HOAN_THANH = 1, DA_HUY = 2 };

inline string trangThaiChuyenToStr(TrangThaiChuyen t) {
    switch (t) {
        case DANG_DI: return "Đang đi";
        case HOAN_THANH: return "Hoàn thành";
        default: return "Đã hủy";
    }
}

inline DanhSach<string> split(const string& s, char delim) {
    DanhSach<string> out;
    string item;
    for (size_t i = 0; i <= s.size(); ++i) {
        if (i == s.size() || s[i] == delim) {
            if (!(i == s.size() && item.empty()))   // giống getline: không thêm mục rỗng ở cuối
                out.themCuoi(item);
            item.clear();
        } else item += s[i];
    }
    return out;
}

inline bool hangBangLaiHopLeTaxi(const string& bangLai) {
    static const DanhSach<string> hopLe = {"B2", "C", "D", "E"};
    return hopLe.chua(bangLai);
}

#endif // COMMON_H
