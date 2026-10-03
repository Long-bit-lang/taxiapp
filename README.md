# He thong Quan ly Taxi Tien Sa (C++ / Qt5)

Chuong trinh quan ly he thong taxi, xay dung bang C++ va giao diện Qt5 Widgets,
theo dung thiet ke da trinh bay trong bao cao do an (Chuong 2, 3, 4).

## Cau truc thu muc

```
taxi_app/
├── include/              # Cac lop nghiep vu (business classes) - dung chung, khong phu thuoc Qt
│   ├── Common.h           Date, TrangThai, VaiTro
│   ├── TaiXe.h             Lop Tai xe (encapsulation)
│   ├── Taxi.h              Lop Taxi
│   ├── ChuyenXe.h          Lop Chuyen xe (tinh cuoc phi)
│   ├── NguoiDung.h         NguoiDung (abstract) -> QuanLy, DieuHanh (ke thua + da hinh)
│   ├── QuanLyTaiXe.h       CRUD + tim kiem + doc/ghi file taixe.txt
│   ├── QuanLyTaxi.h        CRUD + tim taxi phu hop + doc/ghi file taxi.txt
│   ├── QuanLyChuyenXe.h    Tao chuyen xe + thong ke + doc/ghi file chuyenxe.txt
│   ├── QuanLyTaiKhoan.h    Kiem tra dang nhap + doc file taikhoan.txt
│   └── HeThongTaxi.h       Lop trung tam (composition root)
│
├── src/
│   ├── main.cpp            Entry point
│   └── gui/                Cac widget giao dien (Qt)
│       ├── Style.h              Stylesheet QSS dung chung
│       ├── LoginWidget.*         Man hinh dang nhap
│       ├── MainWindow.*          Khung chinh: title bar + sidebar + noi dung
│       ├── DriverPage.* + DriverDialog.*   Quan ly tai xe (them/sua/xoa/tim kiem)
│       ├── TaxiPage.*            Quan ly taxi (them/xoa/phan cong tai xe)
│       ├── BookingPage.*         Dat va xac nhan chuyen xe
│       └── StatsPage.*           Thong ke / bang luong theo taxi
│
├── data/                  # File du lieu (van ban, phan tach boi dau '|')
│   ├── taikhoan.txt
│   ├── taixe.txt
│   ├── taxi.txt
│   └── chuyenxe.txt
│
└── QuanLyTaxiTienSa.pro   # File du an qmake
```

## Yeu cau moi truong

- Qt5 (qtbase5-dev) va bo cong cu qmake
- Bo bien dich ho tro C++17 (g++ 9+ hoac tuong duong)

### Cai dat tren Ubuntu/Debian

```bash
sudo apt-get install qtbase5-dev qtbase5-dev-tools qt5-qmake
```

### Cai dat tren Windows

- Cai Qt Creator (kem Qt 5.15 hoac 6.x) tu https://www.qt.io/download
- Mo file `QuanLyTaxiTienSa.pro` bang Qt Creator -> Build -> Run

## Build va chay tren Linux

```bash
cd taxi_app
qmake QuanLyTaxiTienSa.pro
make -j4
./QuanLyTaxiTienSa
```

## Tai khoan dang nhap mau

| Tai khoan   | Mat khau | Vai tro        |
|-------------|----------|----------------|
| quanly01    | 123455   | Quan ly        |
| dieuhanh01  | 12345   | Dieu hanh      |

## Du lieu mau co san

- 4 tai xe (TX001 - TX004)
- 3 taxi (43A-12345, 43A-67890, 43A-11223)
- 4 chuyen xe da thuc hien (gan cho taxi 43A-12345, tai xe TX001)

## Ghi chu ky thuat

- Du lieu duoc doc/ghi vao cac file `.txt` trong thu muc `data/` (dung duong dan tuong doi,
  nen **chay chuong trinh tu thu muc goc `taxi_app/`**, khong chay tu ben trong thu muc build khac).
- Thuat toan "tim taxi phu hop" hien tai sap xep theo bien so (minh hoa) - co the nang cap
  de tich hop toa do GPS thuc te.
- Day la ban demo hoan chinh chuc nang chinh: dang nhap/phan quyen, CRUD tai xe/taxi,
  dat & xac nhan chuyen xe, thong ke bang luong - dung lam co so de phat trien them.
