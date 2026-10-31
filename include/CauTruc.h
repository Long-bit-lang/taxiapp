#ifndef CAUTRUC_H
#define CAUTRUC_H

#include <utility>   // chỉ dùng std::move
#include <initializer_list>

// ========== DanhSach<T>: mảng động (thay vector) ==========
template <typename T>
class DanhSach {
private:
    T* data;
    int n;      // số phần tử
    int cap;    // sức chứa

    void tangSucChua(int capMoi) {
        T* moi = new T[capMoi];
        for (int i = 0; i < n; ++i) moi[i] = std::move(data[i]);
        delete[] data;
        data = moi;
        cap = capMoi;
    }

public:
    DanhSach() : data(nullptr), n(0), cap(0) {}
    DanhSach(std::initializer_list<T> ds) : data(nullptr), n(0), cap(0) {
        for (const T& x : ds) themCuoi(x);
    }
    DanhSach(const DanhSach& o) : data(nullptr), n(0), cap(0) {
        if (o.n > 0) {
            data = new T[o.n];
            cap = o.n;
            for (int i = 0; i < o.n; ++i) data[i] = o.data[i];
            n = o.n;
        }
    }
    DanhSach(DanhSach&& o) noexcept : data(o.data), n(o.n), cap(o.cap) {
        o.data = nullptr; o.n = o.cap = 0;
    }
    DanhSach& operator=(const DanhSach& o) {
        if (this != &o) {
            DanhSach tmp(o);
            hoanDoi(tmp);
        }
        return *this;
    }
    DanhSach& operator=(DanhSach&& o) noexcept {
        if (this != &o) {
            delete[] data;
            data = o.data; n = o.n; cap = o.cap;
            o.data = nullptr; o.n = o.cap = 0;
        }
        return *this;
    }
    ~DanhSach() { delete[] data; }

    void hoanDoi(DanhSach& o) {
        std::swap(data, o.data);
        std::swap(n, o.n);
        std::swap(cap, o.cap);
    }

    int size() const { return n; }
    bool empty() const { return n == 0; }
    void clear() { n = 0; }

    void themCuoi(const T& x) {
        if (n == cap) tangSucChua(cap == 0 ? 4 : cap * 2);
        data[n++] = x;
    }
    void push_back(const T& x) { themCuoi(x); }

    void xoaTai(int i) {
        if (i < 0 || i >= n) return;
        for (int k = i; k + 1 < n; ++k) data[k] = std::move(data[k + 1]);
        --n;
    }

    T& operator[](int i) { return data[i]; }
    const T& operator[](int i) const { return data[i]; }

    // iterator dạng con trỏ -> dùng được range-for: for (auto& x : ds)
    T* begin() { return data; }
    T* end() { return data + n; }
    const T* begin() const { return data; }
    const T* end() const { return data + n; }

    // ----- thay cho std::find -----
    int timViTri(const T& x) const {
        for (int i = 0; i < n; ++i) if (data[i] == x) return i;
        return -1;
    }
    bool chua(const T& x) const { return timViTri(x) != -1; }

    // ----- thay cho erase(remove(...)) -----
    void xoaGiaTri(const T& x) {
        int j = 0;
        for (int i = 0; i < n; ++i)
            if (!(data[i] == x)) data[j++] = std::move(data[i]);
        n = j;
    }

    // ----- thay cho erase(remove_if(...)): trả về true nếu có xóa -----
    template <typename Pred>
    bool xoaNeu(Pred dk) {
        int j = 0;
        for (int i = 0; i < n; ++i)
            if (!dk(data[i])) data[j++] = std::move(data[i]);
        bool coXoa = (j != n);
        n = j;
        return coXoa;
    }

    // ----- thay cho std::sort: quick sort tự viết -----
    template <typename Cmp>
    void sapXep(Cmp nhoHon) { quickSort(0, n - 1, nhoHon); }

private:
    template <typename Cmp>
    void quickSort(int l, int r, Cmp nhoHon) {
        while (l < r) {
            T pivot = data[l + (r - l) / 2];
            int i = l, j = r;
            while (i <= j) {
                while (nhoHon(data[i], pivot)) ++i;
                while (nhoHon(pivot, data[j])) --j;
                if (i <= j) { std::swap(data[i], data[j]); ++i; --j; }
            }
            // đệ quy nhánh ngắn, lặp nhánh dài để tránh tràn stack
            if (j - l < r - i) { quickSort(l, j, nhoHon); l = i; }
            else               { quickSort(i, r, nhoHon); r = j; }
        }
    }
};

// ========== Cap<A,B>: thay std::pair ==========
template <typename A, typename B>
struct Cap {
    A first;
    B second;
    Cap() : first(), second() {}
    Cap(const A& a, const B& b) : first(a), second(b) {}
    bool operator==(const Cap& o) const { return first == o.first && second == o.second; }
    bool operator<(const Cap& o) const {
        return first < o.first || (!(o.first < first) && second < o.second);
    }
};

// ========== BanDo<K,V>: cây nhị phân tìm kiếm (thay std::map) ==========
template <typename K, typename V>
class BanDo {
private:
    struct Nut {
        K khoa;
        V giaTri;
        Nut* trai;
        Nut* phai;
        Nut(const K& k) : khoa(k), giaTri(), trai(nullptr), phai(nullptr) {}
    };
    Nut* goc;

    void huy(Nut* x) {
        if (!x) return;
        huy(x->trai);
        huy(x->phai);
        delete x;
    }
    Nut* sao(Nut* x) const {
        if (!x) return nullptr;
        Nut* y = new Nut(x->khoa);
        y->giaTri = x->giaTri;
        y->trai = sao(x->trai);
        y->phai = sao(x->phai);
        return y;
    }
    template <typename F>
    void duyetLNR(Nut* x, F f) const {
        if (!x) return;
        duyetLNR(x->trai, f);
        f(x->khoa, x->giaTri);
        duyetLNR(x->phai, f);
    }

public:
    BanDo() : goc(nullptr) {}
    BanDo(const BanDo& o) : goc(sao(o.goc)) {}
    BanDo& operator=(const BanDo& o) {
        if (this != &o) { huy(goc); goc = sao(o.goc); }
        return *this;
    }
    ~BanDo() { huy(goc); }

    // giống map::operator[]: chưa có khóa thì tự tạo với giá trị mặc định
    V& operator[](const K& k) {
        if (!goc) { goc = new Nut(k); return goc->giaTri; }
        Nut* p = goc;
        while (true) {
            if (k < p->khoa) {
                if (!p->trai) { p->trai = new Nut(k); return p->trai->giaTri; }
                p = p->trai;
            } else if (p->khoa < k) {
                if (!p->phai) { p->phai = new Nut(k); return p->phai->giaTri; }
                p = p->phai;
            } else return p->giaTri;
        }
    }

    V* tim(const K& k) {
        Nut* p = goc;
        while (p) {
            if (k < p->khoa) p = p->trai;
            else if (p->khoa < k) p = p->phai;
            else return &p->giaTri;
        }
        return nullptr;
    }

    // duyệt theo thứ tự khóa tăng dần: f(khoa, giaTri)
    template <typename F>
    void duyet(F f) const { duyetLNR(goc, f); }
};



#endif // CAUTRUC_H
