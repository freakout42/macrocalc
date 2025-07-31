debian install
--------------

from source:
    sudo apt update                                          # Debian and variants
    sudo apt install flex rcs
    sudo yum update                                          # CentOS and variants
    sudo yum install flex flex-devel byacc rcs ncurses-devel
    wget https://github.com/freakout42/macrocalc/archive/refs/heads/main.tar.gz
    tar xzf main.tar.gz
    cd macrocalc-main
    ./configure 
    make -s
    sudo make install
    /opt/arx/bin/mc -u

alma:
    dnf install tar
    dnf install gcc
    dnf install git
    dnf install make
    dnf install ncurses-devel
    dnf install flex
    dnf install bison
    dnf install byacc

with rpm:
    sudo apt install rpm # only Debian
    sudo rpm --nodigest --nofiledigest -Uvh http://beef.toarx.de/rpms/i386/macrocalc.i386.rpm
    sudo ldconfig
    . /etc/profile.d/macrocalc.sh
    mc -u

by docker:
    docker pull ghcr.io/freakout42/macrocalc:latest
    export COLUMNS="`tput cols`"
    export LINES="`tput lines`"
    docker run -e COLUMNS -e LINES -e TERM -ti ghcr.io/freakout42/macrocalc:latest

windows:
#!/bin/sh
set -x
case "$1" in
 -g)
  rm -rf microemacs macrocalc
  git clone https://github.com/freakout42/microemacs.git
  git clone https://github.com/freakout42/macrocalc.git
  ;;
 -m)
  cd microemacs
    make -f Makefile.W32 clean mex.exe
  cd ..
  cd macrocalc/arx/src
    make -f Makefile.W32 clean all
  cd ../../..
  cd macrocalc/mc/src
    make -f Makefile.W32 clean all
  cd ../../..
  ;;
 -c)
  cp -v microemacs/mex.exe /z/tmp/me.exe
  cp -v macrocalc/mc/src/mc{,2wks}.exe /z/tmp/
  ;;
esac
exit
