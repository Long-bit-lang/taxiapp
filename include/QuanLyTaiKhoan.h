#ifndef QUANLYTAIKHOAN_H
#define QUANLYTAIKHOAN_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <memory>
#include "NguoiDung.h"

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
            }
        }
        return nullptr;
    }

    void docFile() {
        dsTaiKhoan.clear();
        std::ifstream fin(duongDanFile);
        if (!fin.is_open()) return;
        std::string line;
        while (std::getline(fin, line)) {
            if (line.empty()) continue;
            auto p = split(line, '|');
            if (p.size() < 3) continue;
            dsTaiKhoan.emplace_back(p[0], p[1], (VaiTro)std::stoi(p[2]));
        }
    }
};

#endif // QUANLYTAIKHOAN_H
