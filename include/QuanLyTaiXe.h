#ifndef QUANLYTAIXE_H
#define QUANLYTAIXE_H

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "TaiXe.h"

using namespace std;


class QuanLyTaiXe {
private:
    DanhSach<TaiXe> dsTaiXe;
    string duongDanFile;

public:
    explicit QuanLyTaiXe(string file = "data/taixe.txt") : duongDanFile(file) {}

    const DanhSach<TaiXe>& layDanhSach() const { return dsTaiXe; }

    void them(const TaiXe& tx) { dsTaiXe.push_back(tx); ghiFile(); }

    bool sua(const string& maTX, const TaiXe& moi) {
        for (auto& t : dsTaiXe)
            if (t.getMaTX() == maTX) { t = moi; ghiFile(); return true; }
        return false;
    }

    bool xoa(const string& bienSo) {
        bool coXoa = dsTaixe.xoaNeu([&](const Taixe& t) { return t.getMaTX() == maTX; });
        if (coXoa) ghiFile();
        return coXoa;
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

    DanhSach<TaiXe> timKiem(const string& tuKhoa) const {
        DanhSach<TaiXe> ketQua;
        string tk = tuKhoa;
        for (auto& ch : tk) ch = (char)tolower((unsigned char)ch);
        for (auto& t : dsTaiXe) {
            string hoTen = t.getHoTen(), gplx = t.getSoGPLX(), bl = t.getBangLai();
            for (auto& ch : hoTen) ch = (char)tolower((unsigned char)ch);
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
