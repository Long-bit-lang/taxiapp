#ifndef COMMON_H
#define COMMON_H

#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
<<<<<<< HEAD
#include <ctime>

using namespace std;

struct Date {
    int day = 1, month = 1, year = 2000;
    string toString() const {
        auto pad = [](int v) { return (v < 10 ? "0" : "") + to_string(v); };
        return pad(day) + "/" + pad(month) + "/" + to_string(year);
=======

struct Date {
    int day = 1, month = 1, year = 2000;
    std::string toString() const {
        auto pad = [](int v) { return (v < 10 ? "0" : "") + std::to_string(v); };
        return pad(day) + "/" + pad(month) + "/" + std::to_string(year);
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
    }
};

enum TrangThai { DANG_HOAT_DONG = 0, NGHI = 1, NGUNG_HOAT_DONG = 2 };
enum ChucNang { CN_QUAN_LY_TAI_XE, CN_QUAN_LY_TAXI, CN_DAT_CHUYEN_XE, CN_THONG_KE };

<<<<<<< HEAD
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
=======

inline std::string trangThaiToStr(TrangThai t) {
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
    switch (t) {
        case DANG_HOAT_DONG: return "Đang hoạt động";
        case NGHI: return "Nghỉ";
        default: return "Ngừng hoạt động";
    }
}

<<<<<<< HEAD
inline bool hangBangLaiHopLeTaxi(const string& bangLai) {
    static const vector<string> hopLe = {"B2", "C", "D", "E"};
    return find(hopLe.begin(), hopLe.end(), bangLai) != hopLe.end();
}

inline bool hangBangLaiPhuHopSucChua(const string& bangLai, int sucChua) {
=======
inline bool hangBangLaiHopLeTaxi(const std::string& bangLai) {
    static const std::vector<std::string> hopLe = {"B2", "C", "D", "E"};
    return std::find(hopLe.begin(), hopLe.end(), bangLai) != hopLe.end();
}

inline bool hangBangLaiPhuHopSucChua(const std::string& bangLai, int sucChua) {
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
    if (bangLai == "B2") return sucChua <= 8;
    if (bangLai == "D")  return sucChua <= 29;
    if (bangLai == "E")  return true;
    if (bangLai == "C")  return true;
    return false;
}

enum VaiTro { QUAN_LY = 0, DIEU_HANH = 1 };

enum TrangThaiChuyen { DANG_DI = 0, HOAN_THANH = 1, DA_HUY = 2 };

<<<<<<< HEAD
inline string trangThaiChuyenToStr(TrangThaiChuyen t) {
=======
inline std::string trangThaiChuyenToStr(TrangThaiChuyen t) {
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
    switch (t) {
        case DANG_DI: return "Đang đi";
        case HOAN_THANH: return "Hoàn thành";
        default: return "Đã hủy";
    }
}

<<<<<<< HEAD
inline vector<string> split(const string& s, char delim) {
    vector<string> out;
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) out.push_back(item);
=======
inline std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) out.push_back(item);
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
    return out;
}
#endif // COMMON_H
