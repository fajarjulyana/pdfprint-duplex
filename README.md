# PDF Printer GUI

Aplikasi GUI berbasis GTK3 untuk mencetak file PDF menggunakan perintah `lp` di sistem berbasis Linux.

## Fitur
- Memilih file PDF untuk dicetak
- Memilih printer yang tersedia
- Memilih sumber kertas
- Opsi cetak terbalik (reverse printing)
- Mencetak menggunakan perintah `lp`

## Prasyarat
Pastikan sistem memiliki:
- GTK3 (`libgtk-3-dev` untuk pengembangan)
- CUPS (`cups` dan `cups-client` untuk manajemen printer)
- Compiler C++ seperti `g++`

## Cara Kompilasi & Menjalankan
```sh
# Instal dependensi (jika belum ada)
sudo apt update && sudo apt install g++ libgtk-3-dev cups cups-client -y

# Kompilasi program
g++ -o pdf_printer main.cpp `pkg-config --cflags --libs gtk+-3.0`

# Jalankan aplikasi
./pdf_printer
```

## Cara Menggunakan
1. Jalankan aplikasi.
2. Pilih printer dari daftar yang tersedia.
3. Pilih sumber kertas.
4. Centang opsi "Cetak terbalik" jika diperlukan.
5. Klik tombol "Cetak" dan pilih file PDF yang ingin dicetak.

## Troubleshooting
- **Tidak ada printer terdeteksi:** Pastikan `cups` sedang berjalan (`sudo systemctl start cups`).
- **Error saat kompilasi:** Pastikan GTK3 terinstal dengan `libgtk-3-dev`.
- **Perintah cetak gagal:** Coba jalankan `lpstat -p` untuk memastikan printer tersedia.

## Lisensi
Proyek ini dirilis dengan lisensi MIT.


