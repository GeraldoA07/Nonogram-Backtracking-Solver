# Nonogram Backtracking Solver

> Solusi otomatis untuk puzzle **Nonogram** menggunakan algoritma **Backtracking** dengan preprocessing.

**Tugas Makalah — Strategi Algoritma (IF2211)**  
Geraldo Artemius — 13524005  
Institut Teknologi Bandung

---

## Tentang Nonogram

**Nonogram** (juga dikenal sebagai *Picross* atau *Paint by Numbers*) adalah puzzle logika berbasis grid. Setiap baris dan kolom pada grid memiliki **clue** — daftar bilangan bulat positif yang menyatakan panjang blok sel yang harus diisi secara berurutan, dengan setidaknya satu sel kosong di antara blok yang berbeda.

Tujuan: mengisi grid sehingga semua clue baris dan kolom terpenuhi.

**Contoh Nonogram 5×5:**

```
Clue Baris : [2 2], [2 3], [2 1 4], [2 4], [2 5 1], [5 2], [1 1 3], [3 4], [4 5], [4 5]
Clue Kolom : [4 3], [10], [1 1 3], [1 2 2], [3], [1 2 2], [5 4], [10], [3 5], [2 3]

Solusi:
X██XXX██XX
██XXXX███X
██X█X████X
██XXXX████
██X█████X█
X█████X██X
X█XX█X███X
███XX████X
████X█████
████X█████
```

---

## Deskripsi Program

Program ini menyelesaikan puzzle Nonogram berukuran sembarang (N baris × M kolom) secara otomatis. Program:

1. **Membaca input** dari file teks yang berisi ukuran papan dan clue baris/kolom.
2. **Memvalidasi input** — memastikan format benar dan clue tidak melebihi ukuran papan.
3. **Menjalankan pre-processing** — mengisi sel-sel yang sudah dapat dipastikan nilainya sebelum backtracking dimulai, untuk mempercepat pencarian.
4. **Menjalankan backtracking solver** dengan *bounding function* untuk memangkas cabang yang tidak mungkin menghasilkan solusi.
5. **Menampilkan solusi** dalam bentuk grid karakter di terminal.

### Representasi Sel

| Simbol | Nilai Enum      | Arti                          |
|--------|-----------------|-------------------------------|
| `█`    | `FILLED = 1`    | Sel terisi (bagian dari blok) |
| `X`    | `CROSSED = 2`   | Sel kosong / disilang         |
| ` `    | `UNASSIGNED = 0`| Sel belum ditentukan          |

---

## Algoritma yang Digunakan

### Backtracking

Program menggunakan pendekatan **backtracking** dengan penelusuran sel demi sel dari kiri-atas ke kanan-bawah.

**Langkah utama `solve(row, col)`:**

1. Jika `row == N` → seluruh papan telah terisi → kembalikan `true` (solusi ditemukan).
2. Jika sel sudah ditentukan oleh pre-processing → lanjut ke sel berikutnya.
3. **Cabang 1:** Coba isi sel dengan `FILLED`.
   - Jalankan `boundingFunction` → jika tidak melanggar, rekursi ke sel berikutnya.
4. **Cabang 2:** Coba isi sel dengan `CROSSED`.
   - Jalankan `boundingFunction` → jika tidak melanggar, rekursi ke sel berikutnya.
5. Jika kedua cabang gagal → kembalikan `false` (backtrack).

### Bounding Function

`boundingFunction(row, col)` memeriksa konsistensi aturan dari papan pada setiap langkah. Pemeriksaan yang dilakukan:

| Pemeriksaan | Deskripsi |
|---|---|
| **Batas jumlah filled per baris** | Total sel `FILLED` pada baris saat ini tidak boleh melebihi total clue baris |
| **Batas jumlah filled per kolom** | Total sel `FILLED` pada kolom saat ini tidak boleh melebihi total clue kolom |
| **Ukuran blok baris** | Blok yang sudah terbentuk di baris saat ini harus cocok dengan urutan clue baris |
| **Ukuran blok kolom** | Blok yang sudah terbentuk di kolom saat ini harus cocok dengan urutan clue kolom |

### Pre-processing

Sebelum backtracking, `preProcess()` mengisi sel-sel yang dapat dipastikan secara deterministik:

- **Single clue majority:** Jika sebuah baris/kolom hanya memiliki satu clue `C` dan `C > L/2` (di mana `L` adalah panjang baris/kolom), maka sel-sel di area irisan (*overlap*) pasti `FILLED`.
- **Tight fit:** Jika jumlah clue + jarak minimum antar blok sama persis dengan panjang baris/kolom, seluruh pola dapat langsung ditentukan.

---

## Struktur Direktori

```
Nonogram-Backtracking-Solver/
├── include/
│   └── Nonogram.hpp      # Deklarasi class Nonogram dan enum CellState
├── src/
│   ├── main.cpp          # Entry point: parsing input, menampilkan info, memanggil solver
│   └── Nonogram.cpp      # Implementasi class Nonogram (solve, boundingFunction, preProcess)
├── input/
│   ├── test1.txt         # Test case 10×10
│   ├── test2.txt         # Test case 15×15
│   └── test3.txt         # Test case 10×10
├── doc/                  # Dokumentasi tambahan (makalah, laporan)
├── LICENSE
└── README.md
```

---

## Format Input

File input adalah file teks (`.txt`) dengan struktur sebagai berikut:

```
N M
<clue baris 1>
<clue baris 2>
...
<clue baris N>
<clue kolom 1>
<clue kolom 2>
...
<clue kolom M>
```

- **Baris pertama:** dua bilangan bulat positif `N` (jumlah baris) dan `M` (jumlah kolom).
- **N baris berikutnya:** clue untuk setiap baris, berupa satu atau lebih bilangan bulat positif yang dipisahkan spasi.
- **M baris berikutnya:** clue untuk setiap kolom, dengan format yang sama.

**Contoh (puzzle 5×5):**
```
5 5
3
1 1
5
1 1
3
3
1 1
5
1 1
3
```

---

## Cara Kompilasi

Pastikan kompiler **g++** (C++17 atau lebih baru) sudah terpasang.

### Menggunakan g++ langsung

Jalankan dari direktori `src/`:

```bash
g++ main.cpp Nonogram.cpp -I../include -o nonogram
```

Atau dari direktori root proyek:

```bash
g++ src/main.cpp src/Nonogram.cpp -Iinclude -o nonogram
```

---

## Cara Menjalankan

Program harus dijalankan dari dalam direktori `src/` agar path relatif ke folder `input/` berfungsi dengan benar.

```bash
cd src/
```

**Metode 1: Nama file sebagai argumen command-line**

```bash
./nonogram <nama_file_input>
```

Contoh:
```bash
./nonogram test1.txt
```

**Metode 2: Input interaktif**

Jalankan program tanpa argumen, lalu masukkan nama file saat diminta:

```bash
./nonogram
```
```
Masukkan path file input: test1.txt
```

> **Catatan:** Program mencari file input di direktori `../input/` relatif terhadap lokasi program dijalankan. Pastikan menjalankan program dari dalam direktori `src/`.

---

## Contoh Penggunaan

```
$ ./nonogram test1.txt

[INFO] Membaca file: test1.txt ...
[INFO] Ukuran papan: 10 baris x 10 kolom

--- Clue Baris ---
  Baris 1: 2 2
  Baris 2: 2 3
  ...

--- Clue Kolom ---
  Kolom 1: 4 3
  Kolom 2: 10
  ...

[INFO] Menjalankan pre-processing...
[INFO] Menjalankan backtracking solver...

SOLUSI DITEMUKAN!
XX████XX██
...
```

Jika tidak ada solusi yang valid:

```
TIDAK ADA SOLUSI YANG VALID
```

---
