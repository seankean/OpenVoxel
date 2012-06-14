Changed Files:

plytvxbx.cpp
file_list_test_bunny.txt

Added: --zoom_progressive .01 multiplies the Z value and the parameter to scale the x,y value only...

================================


Added --zoom_mid and --zoom_hi to adjust the size of slices that are more distant from the first set.  
It is a simple multiplier, affecting X,Y only.  Use values under 1.0 to shrink the mid and hi slices. 

0 and 6 No changes.
1 and 5 multiply X,Y by --zoom_mid 
2 and 4 multiply X,Y by --zoom_hi 


3 and 7 - (black frames) - unchanged.

==================================

Added file list and scaling logic to ply2vxbx and increased speed dramatically.  Please use this new method


./ply2vxbx --ply_file_list ./file_list_test_bunny.txt

file_example 
--ply_file ./bunny.ply --swapXZ --swapXY --flipZ 
--ply_file ./bunny2.ply --swapYZ --flipZ 
--ply_file ./bunny3.ply --swapXZ --flipZ 
--ply_file ./bunny4.ply --swapXZ --swapYZ --flipZ 

========================================
vxbx_player keeps his old behavior but adds the ability to loop over a txt file list of ordered-complete path/filenames.  This supports the format created by ply2vxbx easily by using the following example file 

./DisplayProgramming --max_time 10000  --serial_device COM3 --camera_ip 192.168.1.100:5555 --num_images 32 --file_list_name file_list_test.txt 

file_example
../ply2vxbx/bunny2/image00000.bmp
../ply2vxbx/bunny2/image00001.bmp
../ply2vxbx/bunny2/image00002.bmp
../ply2vxbx/bunny2/image00003.bmp
../ply2vxbx/bunny2/image00004.bmp
../ply2vxbx/bunny2/image00005.bmp
../ply2vxbx/bunny2/image00006.bmp
../ply2vxbx/bunny2/image00007.bmp
../ply2vxbx/bunny3/image00000.bmp
../ply2vxbx/bunny3/image00001.bmp
../ply2vxbx/bunny3/image00002.bmp
../ply2vxbx/bunny3/image00003.bmp
../ply2vxbx/bunny3/image00004.bmp
../ply2vxbx/bunny3/image00005.bmp
../ply2vxbx/bunny3/image00006.bmp
../ply2vxbx/bunny3/image00007.bmp
../ply2vxbx/bunny4/image00000.bmp
../ply2vxbx/bunny4/image00001.bmp
../ply2vxbx/bunny4/image00002.bmp
../ply2vxbx/bunny4/image00003.bmp
../ply2vxbx/bunny4/image00004.bmp
../ply2vxbx/bunny4/image00005.bmp
../ply2vxbx/bunny4/image00006.bmp
../ply2vxbx/bunny4/image00007.bmp
../ply2vxbx/bunny/image00000.bmp
../ply2vxbx/bunny/image00001.bmp
../ply2vxbx/bunny/image00002.bmp
../ply2vxbx/bunny/image00003.bmp
../ply2vxbx/bunny/image00004.bmp
../ply2vxbx/bunny/image00005.bmp
../ply2vxbx/bunny/image00006.bmp
../ply2vxbx/bunny/image00007.bmp

I also removed some of the debug from the main loop (Not the serial read of course ;) )


