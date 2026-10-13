#ifndef QUANLYTAIKHOAN_H
#define QUANLYTAIKHOAN_H

#include <string>
#include <fstream>
#include <memory>
#include "NguoiDung.h"

using namespace std;


class QuanLyTaiKhoan {
private:
    DanhSach<TaiKhoan> dsTaiKhoan;
    string duongDanFile;

public:
    explicit QuanLyTaiKhoan(string file = "data/taikhoan.txt") : duongDanFile(file) {}

    // Tra ve con tro NguoiDung* (QuanLy* hoac DieuHanh*) neu dang nhap thanh cong -> nullptr neu sai
    unique_ptr<NguoiDung> kiemTraDangNhap(const string& tk, const string& mk) const {
        for (auto& acc : dsTaiKhoan) {
            if (acc.tenDangNhap == tk && acc.matKhau == mk) {
                if (acc.vaiTro == QUAN_LY) return make_unique<QuanLy>(tk);
                else return make_unique<DieuHanh>(tk);
            }
        }
        return nullptr;
    }

    void docFile() {
        dsTaiKhoan.clear();
        ifstream fin(duongDanFile);
        if (!fin.is_open()) return;
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            auto p = split(line, '|');
            if (p.size() < 3) continue;
            dsTaiKhoan.themCuoi(TaiKhoan(p[0], p[1], (VaiTro)stoi(p[2])));
        }
    }
};

#endif // QUANLYTAIKHOAN_H
