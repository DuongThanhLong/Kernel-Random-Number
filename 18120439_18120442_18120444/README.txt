Kernel_Random_Numbers
Cách sử dụng
- Mở terminal cd vào thư mục source (Hoặc tại thư mục source chuột phải chọn open in terminal để mở terminal)
- Chạy lệnh "make" để compile module
- Chạy lệnh "sudo insmod randModule.ko" để thêm module
- chạy lệnh "sudo dmesg -c" để kiểm tra trạng thái driver đã đăng kí
- Chạy lệnh "gcc readTest.c -o test" để compile file test cho user
- chạy lệnh "sudo ./test" để lấy số random.
- chạy lệnh "sudo dmesg -c" để kiểm số random được đọc ra ở user test có đúng với số random của module hay không
- chạy lệnh "sudo rmmod randModule.ko" để tháo module
- chạy lênh "make clean" để clean hết cái file compile