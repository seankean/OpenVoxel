# Jamie Tremaine
# _____________
# This script will install the required packages and make the executable

# To install:
# Install the build-essential package, which includes compiler, header libraries, etc.:
sudo apt-get install build-essential

# Install the QT4 libraries
sudo apt-get install qt4-designer qt4-dev-tools qt4-qmake

# This uses the qmake build system:
qmake
make clean
make
