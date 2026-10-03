#ifndef QUANLYTAIKHOAN_H
#define QUANLYTAIKHOAN_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include "NguoiDung.h"

<<<<<<< HEAD
using namespace std;


class QuanLyTaiKhoan {
private:
    vector<TaiKhoan> dsTaiKhoan;
    string duongDanFile;

public:
    explicit QuanLyTaiKhoan(string file = "data/taikhoan.txt") : duongDanFile(file) {}

    // Tra ve con tro NguoiDung* (QuanLy* hoac DieuHanh*) neu dang nhap thanh cong -> nullptr neu sai
    unique_ptr<NguoiDung> kiemTraDangNhap(const string& tk, const string& mk) const {
        for (auto& acc : dsTaiKhoan) {
            if (acc.tenDangNhap == tk && acc.matKhau == mk) {
                if (acc.vaiTro == QUAN_LY) return make_unique<QuanLy>(tk);
                else return make_unique<DieuHanh>(tk);
=======
class QuanLyTaiKhoan {
private:
    std::vector<TaiKhoan> dsTaiKhoan;
    std::string duongDanFile;

public:
    explicit QuanLyTaiKhoan(std::string file = "data/taikhoan.txt") : duongDanFile(file) {}

    // Tra ve con tro NguoiDung* (QuanLy* hoac DieuHanh*) neu dang nhap thanh cong -> nullptr neu sai
    std::unique_ptr<NguoiDung> kiemTraDangNhap(const std::string& tk, const std::string& mk) const {
        for (auto& acc : dsTaiKhoan) {
            if (acc.tenDangNhap == tk && acc.matKhau == mk) {
                if (acc.vaiTro == QUAN_LY) return std::make_unique<QuanLy>(tk);
                else return std::make_unique<DieuHanh>(tk);
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
            }
        }
        return nullptr;
    }

    void docFile() {
        dsTaiKhoan.clear();
<<<<<<< HEAD
        ifstream fin(duongDanFile);
        if (!fin.is_open()) return;
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            auto p = split(line, '|');
            if (p.size() < 3) continue;
            dsTaiKhoan.emplace_back(p[0], p[1], (VaiTro)stoi(p[2]));
=======
        std::ifstream fin(duongDanFile);
        if (!fin.is_open()) return;
        std::string line;
        while (std::getline(fin, line)) {
            if (line.empty()) continue;
            auto p = split(line, '|');
            if (p.size() < 3) continue;
            dsTaiKhoan.emplace_back(p[0], p[1], (VaiTro)std::stoi(p[2]));
>>>>>>> ac530f48aed7f31cf74186dddadf02ad8de568f1
        }
    }
};

#endif // QUANLYTAIKHOAN_H
