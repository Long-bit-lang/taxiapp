#ifndef QUANLYTAIXE_H
#define QUANLYTAIXE_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "TaiXe.h"

using namespace std;


class QuanLyTaiXe {
private:
    vector<TaiXe> dsTaiXe;
    string duongDanFile;

public:
    explicit QuanLyTaiXe(string file = "data/taixe.txt") : duongDanFile(file) {}

    const vector<TaiXe>& layDanhSach() const { return dsTaiXe; }

    void them(const TaiXe& tx) { dsTaiXe.push_back(tx); ghiFile(); }

    bool sua(const string& maTX, const TaiXe& moi) {
        for (auto& t : dsTaiXe)
            if (t.getMaTX() == maTX) { t = moi; ghiFile(); return true; }
        return false;
    }

    bool xoa(const string& maTX) {
        auto it = remove_if(dsTaiXe.begin(), dsTaiXe.end(),
                                  [&](const TaiXe& t) { return t.getMaTX() == maTX; });
        if (it == dsTaiXe.end()) return false;
        dsTaiXe.erase(it, dsTaiXe.end());
        ghiFile();
        return true;
    }

    bool coSoGPLX(const string& gplx, const string& boQuaMa = "") const {
        for (auto& t : dsTaiXe)
            if (t.getSoGPLX() == gplx && t.getMaTX() != boQuaMa) return true;
        return false;
    }

    bool coMaTX(const string& maTX) const {
        for (auto& t : dsTaiXe) if (t.getMaTX() == maTX) return true;
        return false;
    }

    vector<TaiXe> timKiem(const string& tuKhoa) const {
        vector<TaiXe> ketQua;
        string tk = tuKhoa;
        transform(tk.begin(), tk.end(), tk.begin(), ::tolower);
        for (auto& t : dsTaiXe) {
            string hoTen = t.getHoTen(), gplx = t.getSoGPLX(), bl = t.getBangLai();
            transform(hoTen.begin(), hoTen.end(), hoTen.begin(), ::tolower);
            string tuoiStr = to_string(tinhTuoi(t.getNgaySinh()));
            if (tk.empty() || hoTen.find(tk) != string::npos ||
                gplx.find(tuKhoa) != string::npos ||
                bl.find(tuKhoa) != string::npos ||
                tuoiStr.find(tuKhoa) != string::npos)
                ketQua.push_back(t);
        }
        return ketQua;
    }

    string sinhMaMoi() const {
        int maxx = 0;
        for (auto& t : dsTaiXe) {
            string ma = t.getMaTX();
            if (ma.size() > 2) maxx = max(maxx, atoi(ma.substr(2).c_str()));
        }
        char buf[16];
        snprintf(buf, sizeof(buf), "TX%03d", maxx + 1);
        return buf;
    }

    void docFile() {
        dsTaiXe.clear();
        ifstream fin(duongDanFile);
        if (!fin.is_open()) return;
        string line;
        while (getline(fin, line)) {
            if (line.empty()) continue;
            auto p = split(line, '|');
            if (p.size() < 9) continue;
            Date d{}; sscanf(p[3].c_str(), "%d/%d/%d", &d.day, &d.month, &d.year);
            TaiXe t(p[0], p[1], p[2], d, p[4], p[5], p[6], p[7], (TrangThai)stoi(p[8]));
            dsTaiXe.push_back(t);
        }
    }

    void ghiFile() const {
        ofstream fout(duongDanFile, ios::trunc);
        for (auto& t : dsTaiXe) {
            fout << t.getMaTX() << "|" << t.getTen() << "|" << t.getHoDem() << "|"
                 << t.getNgaySinh().toString() << "|" << t.getDiaChi() << "|"
                 << t.getSoDienThoai() << "|" << t.getBangLai() << "|" << t.getSoGPLX()
                 << "|" << (int)t.getTrangThai() << "\n";
        }
    }
};

#endif // QUANLYTAIXE_H
