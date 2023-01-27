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

