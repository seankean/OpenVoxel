@echo on
set PATH=%PATH%;c:\Qt\4.8.1\lib
.\release\DisplayProgramming --max_time 10000 --serial_device COM3 --camera_ip 192.168.1.100:5555 --num_images 8 --image_prefix align\image --fullscreen 2> log.txt

