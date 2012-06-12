@echo on
set PATH=%PATH%;c:\Qt\4.8.1\lib
.\debug\DisplayProgramming --max_time 250 --serial_device COM3 --camera_ip 192.168.1.100:5555 --num_images 8 --image_prefix testimages\image --fullscreen 2> log.txt


