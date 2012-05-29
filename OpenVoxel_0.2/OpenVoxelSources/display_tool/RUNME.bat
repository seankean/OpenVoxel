@echo on
set PATH=%PATH%;c:\Qt\4.8.1\lib
.\release\DisplayProgramming --serial_device COM3 --camera_ip 192.168.1.100:5555 --num_images 4 --image_prefix testimages\image --fullscreen

REM .\release\DisplayProgramming --serial_device COM3 --camera_ip 192.168.1.100:5555 --num_images 100 --image_prefix tarzanimages\image

