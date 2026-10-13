# Midterm - ls đơn giản

Họ tên: Lương Thành Đạt
MSSV: 24IT327
Repo: https://github.com/loan08111994/LuongThanhDat_24IT327_midterm

## Giới thiệu

Viết lại lệnh `ls` đơn giản bằng C, đọc theo manual NetBSD trong đề.

## Tính năng đã làm

- Liệt kê file, thư mục, không đối số thì liệt kê `.`
- Các tùy chọn: `-A -a -c -d -F -f -h -i -k -l -n -q -R -r -S -s -t -u -w`
- Xử lý `-R`/`-d`, `-l`/`-n`, `-c`/`-u`, `-q`/`-w`, `-k`/`-h` theo kiểu cái sau đè cái trước
- `-l` in mode, link, owner, group, size (hoặc major, minor), thời gian, tên, symlink in thêm `->`
- `-s` in số block, có đọc `BLOCKSIZE`, ra terminal thì in dòng `total`
- Sắp xếp theo tên, `-S` theo size, `-t` theo thời gian, `-r` đảo, `-f` không sắp xếp
- File ẩn: `-a` hiện hết, `-A` trừ `.` `..`, root tự bật `-A`
- Không crash khi gặp file không tồn tại, không có quyền

## Cấu trúc

- `main.c`: hàm main, tách file / thư mục
- `options.h/.c`: đọc tham số
- `fileinfo.h/.c`: giữ danh sách file
- `sort.h sort.c`: sắp xếp
- `display.h/.c`: in file, mở thư mục, làm `-R`
- `util.h/.c`: mode, size, thời gian, `-F`, `-q`
- `Makefile`, `.gitignore`

## Cách chạy

```sh
make
./myls
./myls -la
./myls -l -R /tmp
./myls -d -F file1 dir1
make clean
```
