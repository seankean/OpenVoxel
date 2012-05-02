# Configure the serial port:
# NOTE: the number below should be whatever baud rate your arduino expects to use.
# Note that this should also match with the rate in main.cpp
stty -F /dev/ttyACM0 cs8 115200 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts 

# Run the app:
./app
