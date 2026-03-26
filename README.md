## Penjelasan Program :
Program yang dikerjakan adalah program voxelization menggunakan octree dengan
menggunkaan algoritma Divide and Conquer.

Program ini dibuat dengan bahasa C++ dengan bantuan tools CMake dan python(untuk launcher visualisasi .obj nya).
## Requirement program dan instalasi 

### 1. CMake
* **Versi Minimal:** 3.10 atau lebih baru.
* **Metode Instalasi:**
    * **Linux:** `sudo apt install cmake`
    * **Windows:** Unduh di [cmake.org](https://cmake.org/download/)

### 2. Blender
* **Konfigurasi:** Pastikan direktori *executable* `blender` sudah terdaftar dalam sistem **PATH**.
* **Unduh:** [blender.org/download](https://www.blender.org/download/)

### 3. Makefile (GNU Make)
* **Metode Instalasi:**
    * **Linux/macOS:** Umumnya sudah tersedia secara default.
    * **Windows:** Gunakan [MinGW-w64](http://mingw-w64.org/) atau via Chocolatey:
        ```bash
        choco install make
        ```
### 4. Compiler C++ (MinGW-w64 / GCC)
Versi Minimal: C++17 (untuk mendukung fitur modern pada octree.hpp).

### Metode Instalasi:

### Linux (Ubuntu): 
```bash
sudo apt update
sudo apt install build-essential
```
### Windows (MinGW-w64): 
Untuk pengguna Windows, sangat disarankan menggunakan MSYS2 untuk mengelola toolchain GCC.

Instalasi MSYS2: Unduh installer di msys2.org.

Instalasi Toolchain: Buka terminal UCRT64 atau MinGW64 (dalam MSYS2) dan jalankan:

```bash
pacman -Syu
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-make
```
Penting: Tambahkan path folder <code>bin</code>d ari MSYS2 (contoh: <code> C:\msys64\ucrt64\bin</code>) ke dalam System Environment Variables (PATH) di Windows agar perintah g++ atau mingw32-make bisa dipanggil dari CMD atau PowerShell biasa.

---
## Cara menjalankan dan menggunakan program

jika file voxel_gen aman maka bisa dengan menjalankan command berikut:
```bash
./bin/voxel_gen test/<nama_input_obj>.obj test/<nama_output_obj>.obj
```
> [!NOTE]
>Jika file <code>voxel_gen.exe</code> atau <code>voxel_gen</code> tidak dapat dikompilasi/error atau belum dibuild atau ingin di rebuild maka cara memperbaiki nya dengan cara berikut:
### Linux :
```bash
cd build 
cmake .. && make
cd ..
./bin/voxel_gen test/<nama_obj>.obj test/<nama_output_obj>..obj
```
### Windows :
```bash
cd build
cmake -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=mingw32-make ..
mingw32-make
./bin/voxel_gen test/<nama_obj>.obj test/<nama_output_obj>..obj
```
---
## Author:
### 1. Nathan Adhika Sentosa [NIM: 13524041]
### 2. Faris Wirakusuma Triawan [NIM: 13524130]
