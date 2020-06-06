Following https://medium.com/@au42/the-useful-raspberrypi-cross-compile-guide-ea56054de187
In this directory do:

1) Clone this repo in the proper location and cd to the project directory

2) In project directory clone tools:

$ git clone https://github.com/raspberrypi/tools.git

3) Change path to your compiler in toolchain file!

4) run make application script 
$./scripts/make-application.sh

5) Copy compiled program to raspberry pi (change IP in command)
$ scp spider-service root@192.168.0.60:/usr/bin/

6) run program on rpi
$ spider-service

7) to connect to the TCP server
$ telnet x.x.x.x(ip) 54000
