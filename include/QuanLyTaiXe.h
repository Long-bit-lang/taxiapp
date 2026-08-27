#ifndef QUANLYTAIXE_H
#define QUANLYTAIXE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "TaiXe.h"

class QuanLyTaiXe {
private:
    std::vector<TaiXe> dsTaiXe;
    std::string duongDanFile;

    static std::vector<std::string> split(const std::string& s, char delim) {
        std::vector<std::string> out;
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) out.push_back(item);
        return out;
    }

public:
    explicit QuanLyTaiXe(std::string file = "data/taixe.txt") : duongDanFile(file) {}

    const std::vector<TaiXe>& layDanhSach() const { return dsTaiXe; }

    void them(const TaiXe& tx) { dsTaiXe.push_back(tx); ghiFile(); }

    bool sua(const std::string& maTX, const TaiXe& moi) {
        for (auto& t : dsTaiXe)
            if (t.getMaTX() == maTX) { t = moi; ghiFile(); return true; }
        return false;
    }

    bool xoa(const std::string& maTX) {
        auto it = std::remove_if(dsTaiXe.begin(), dsTaiXe.end(),
                                  [&](const TaiXe& t) { return t.getMaTX() == maTX; });
        if (it == dsTaiXe.end()) return false;
        dsTaiXe.erase(it, dsTaiXe.end());
        ghiFile();
        return true;
    }

    bool coSoGPLX(const std::string& gplx, const std::string& boQuaMa = "") const {
        for (auto& t : dsTaiXe)
            if (t.getSoGPLX() == gplx && t.getMaTX() != boQuaMa) return true;
        return false;
    }

    bool coMaTX(const std::string& maTX) const {
        for (auto& t : dsTaiXe) if (t.getMaTX() == maTX) return true;
        return false;
    }

    std::vector<TaiXe> timKiem(const std::string& tuKhoa) const {
        std::vector<TaiXe> ketQua;
        std::string tk = tuKhoa;
        std::transform(tk.begin(), tk.end(), tk.begin(), ::tolower);
        for (auto& t : dsTaiXe) {
            std::string hoTen = t.getHoTen(), gplx = t.getSoGPLX(), bl = t.getBangLai();
            std::transform(hoTen.begin(), hoTen.end(), hoTen.begin(), ::tolower);
            if (tk.empty() || hoTen.find(tk) != std::string::npos ||
                gplx.find(tuKhoa) != std::string::npos ||
                bl.find(tuKhoa) != std::string::npos)
                ketQua.push_back(t);
        }
        return ketQua;
    }

    std::string sinhMaMoi() const {
        int max = 0;
        for (auto& t : dsTaiXe) {
            std::string ma = t.getMaTX();
            if (ma.size() > 2) max = std::max(max, atoi(ma.substr(2).c_str()));
        }
        char buf[16];
        snprintf(buf, sizeof(buf), "TX%03d", max + 1);
        return buf;
    }

    void docFile() {
        dsTaiXe.clear();
        std::ifstream fin(duongDanFile);
        if (!fin.is_open()) return;
        std::string line;
        while (std::getline(fin, line)) {
            if (line.empty()) continue;
            auto p = split(line, '|');
            if (p.size() < 9) continue;
            Date d{}; sscanf(p[3].c_str(), "%d/%d/%d", &d.day, &d.month, &d.year);
            TaiXe t(p[0], p[1], p[2], d, p[4], p[5], p[6], p[7], (TrangThai)std::stoi(p[8]));
            dsTaiXe.push_back(t);
        }
    }

    void ghiFile() const {
        std::ofstream fout(duongDanFile, std::ios::trunc);
        for (auto& t : dsTaiXe) {
            fout << t.getMaTX() << "|" << t.getTen() << "|" << t.getHoDem() << "|"
                 << t.getNgaySinh().toString() << "|" << t.getDiaChi() << "|"
                 << t.getSoDienThoai() << "|" << t.getBangLai() << "|" << t.getSoGPLX()
                 << "|" << (int)t.getTrangThai() << "\n";
        }
    }
};

#endif // QUANLYTAIXE_H
