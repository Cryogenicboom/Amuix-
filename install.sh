#!/bin/bash
sudo apt update
sudo apt install -y gcc make git

git clone https://github.com/Cryogenicboom/Amunix.git
cd Amunix

make
./AMUNIX

#  TO RUN : 
#  chmod +x install.sh
#  ./install.sh