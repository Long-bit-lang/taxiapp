#pragma once
#include <utility>   
#include <initializer_list>

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
        for (const T& x : ds) push_back(x);
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

    void push_back(const T& x) {
        if (n == cap) tangSucChua(cap == 0 ? 4 : cap * 2);
        data[n++] = x;
    }

    void xoaTai(int i) {
        if (i < 0 || i >= n) return;
        for (int k = i; k + 1 < n; ++k) data[k] = std::move(data[k + 1]);
        --n;
    }

    T& operator[](int i) { return data[i]; }
    const T& operator[](int i) const { return data[i]; }

    T* begin() { return data; }
    T* end() { return data + n; }
    const T* begin() const { return data; }
    const T* end() const { return data + n; }

    int timViTri(const T& x) const {
        for (int i = 0; i < n; ++i) if (data[i] == x) return i;
        return -1;
    }
    bool chua(const T& x) const { return timViTri(x) != -1; }

    void xoaGiaTri(const T& x) {
        int j = 0;
        for (int i = 0; i < n; ++i)
            if (!(data[i] == x)) data[j++] = std::move(data[i]);
        n = j;
    }

    template <typename Pred>
    bool xoaNeu(Pred dk) {
        int j = 0;
        for (int i = 0; i < n; ++i)
            if (!dk(data[i])) data[j++] = std::move(data[i]);
        bool coXoa = (j != n);
        n = j;
        return coXoa;
    }

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
            if (j - l < r - i) { quickSort(l, j, nhoHon); l = i; }
            else               { quickSort(i, r, nhoHon); r = j; }
        }
    }
};

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

template <typename Key, typename Value>
class Map {
private:
    struct Node {
        Key khoa;
        Value giaTri;
        Node* left;
        Node* right;
        Node(const Key& k) : khoa(k), giaTri(), left(nullptr), right(nullptr) {}
    };
    Node* goc;

    void huy(Node* x) {
        if (!x) return;
        huy(x->left);
        huy(x->right);
        delete x;
    }
    Node* copy(Node* x) const {
        if (!x) return nullptr;
        Node* y = new Node(x->khoa);
        y->giaTri = x->giaTri;
        y->left = copy(x->left);
        y->right = copy(x->right);
        return y;
    }
    template <typename F>
    void duyetLNR(Node* x, F f) const {
        if (!x) return;
        duyetLNR(x->left, f);
        f(x->khoa, x->giaTri);
        duyetLNR(x->right, f);
    }

public:
    Map() : goc(nullptr) {}
    Map(const Map& o) : goc(copy(o.goc)) {}
    Map& operator=(const Map& o) {
        if (this != &o) { huy(goc); goc = copy(o.goc); }
        return *this;
    }
    ~Map() { huy(goc); }

    Value& operator[](const Key& k) {
        if (!goc) { goc = new Node(k); return goc->giaTri; }
        Node* p = goc;
        while (true) {
            if (k < p->khoa) {
                if (!p->left) { p->left = new Node(k); return p->left->giaTri; }
                p = p->left;
            } else if (p->khoa < k) {
                if (!p->right) { p->right = new Node(k); return p->right->giaTri; }
                p = p->right;
            } else return p->giaTri;
        }
    }

    Value* tim(const Key& k) {
        Node* p = goc;
        while (p) {
            if (k < p->khoa) p = p->left;
            else if (p->khoa < k) p = p->right;
            else return &p->giaTri;
        }
        return nullptr;
    }

    template <typename F>
    void duyet(F f) const { duyetLNR(goc, f); }
};
