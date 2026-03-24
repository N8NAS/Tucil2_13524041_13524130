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

---
## Cara menjalankan dan menggunakan program

jika file voxel_gen aman maka bisa dengan menjalankan command berikut:
```bash
./bin/voxel_gen test/<nama_input_obj>.obj test/<nama_output_obj>.obj
```
> [!NOTE]
>Jika file <code>voxel_gen.exe</code> atau <code>voxel_gen</code> tidak dapat dikompilasi/error atau belum dibuild atau ingin di rebuild maka cara memperbaiki nya dengan cara berikut:
```bash
cd build 
cmake .. && make
cd ..
./bin/voxel_gen test/<nama_obj>.obj test/<nama_output_obj>..obj
```

---
## Author:
### 1. Nathan Adhika Sentosa [NIM: 13524041]
### 2. Faris Wirakusuma Triawan [NIM: 13524130]
