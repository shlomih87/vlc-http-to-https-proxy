{\rtf1\ansi\ansicpg1252\cocoartf1504\cocoasubrtf830
{\fonttbl\f0\fswiss\fcharset0 Helvetica;\f1\fnil\fcharset0 Menlo-Regular;\f2\fmodern\fcharset0 Courier;
}
{\colortbl;\red255\green255\blue255;\red14\green14\blue14;\red235\green236\blue237;\red36\green38\blue41;
\red43\green39\blue19;\red0\green0\blue0;\red235\green235\blue235;\red0\green0\blue0;\red235\green235\blue235;
}
{\*\expandedcolortbl;;\cssrgb\c6667\c6667\c6667;\cssrgb\c93725\c94118\c94510;\cssrgb\c18824\c20000\c21176;
\cssrgb\c22353\c20000\c9412;\cssrgb\c0\c0\c0;\cssrgb\c93725\c93725\c93725;\cssrgb\c0\c0\c0;\cssrgb\c93725\c93725\c93725;
}
\paperw11900\paperh16840\margl1440\margr1440\vieww25400\viewh16000\viewkind0
\pard\tx566\tx1133\tx1700\tx2267\tx2834\tx3401\tx3968\tx4535\tx5102\tx5669\tx6236\tx6803\pardirnatural\partightenfactor0

\f0\fs24 \cf0 \
Installations needed before running the application:\
a. Install gcc (either with  
\f1\fs26 \cf2 \cb3 \expnd0\expndtw0\kerning0
\outl0\strokewidth0 \strokec2 sudo apt-get install build-essential\
 
\f0\fs24 \cf0 \cb1 \kerning1\expnd0\expndtw0 \outl0\strokewidth0 or 
\f1\fs26 \cf2 \cb3 \expnd0\expndtw0\kerning0
\outl0\strokewidth0 \strokec2 sudo apt-get install gcc
\f0\fs24 \cf0 \cb1 \kerning1\expnd0\expndtw0 \outl0\strokewidth0 ).\
b. Install Boost:\
   
\f1\fs26 \cf4 \cb3 \expnd0\expndtw0\kerning0
\outl0\strokewidth0 \strokec4 sudo apt-get install libboost-all-dev\cf5 \strokec5 \

\f0\fs24 \cf0 \cb1 \kerning1\expnd0\expndtw0 \outl0\strokewidth0 c. Install OpenSSL:\
\pard\pardeftab720\sl280\partightenfactor0

\f2\fs28 \cf6 \cb7 \expnd0\expndtw0\kerning0
\outl0\strokewidth0 \strokec6 sudo apt-get install libssl-dev\
\pard\tx566\tx1133\tx1700\tx2267\tx2834\tx3401\tx3968\tx4535\tx5102\tx5669\tx6236\tx6803\pardirnatural\partightenfactor0

\f0\fs24 \cf0 \cb1 \kerning1\expnd0\expndtw0 \outl0\strokewidth0 c. Install VLC:\
\pard\pardeftab720\sl280\partightenfactor0

\f2\fs28 \cf8 \cb9 \expnd0\expndtw0\kerning0
sudo apt-get install vlc\cf6 \cb7 \outl0\strokewidth0 \strokec6 \
\pard\pardeftab720\sl280\partightenfactor0
\cf6 \
\pard\tx566\tx1133\tx1700\tx2267\tx2834\tx3401\tx3968\tx4535\tx5102\tx5669\tx6236\tx6803\pardirnatural\partightenfactor0

\f0\fs24 \cf0 \cb1 \kerning1\expnd0\expndtw0 \outl0\strokewidth0 \
In order to run just build sources with \'93make\'94 command and run the application with:\
\pard\pardeftab720\sl280\partightenfactor0

\f2\fs28 \cf8 \cb9 \expnd0\expndtw0\kerning0
sudo ./ProxyServer
\f0\fs24 \cf0 \cb1 \kerning1\expnd0\expndtw0 \
\pard\tx566\tx1133\tx1700\tx2267\tx2834\tx3401\tx3968\tx4535\tx5102\tx5669\tx6236\tx6803\pardirnatural\partightenfactor0
\cf0 \
\
1. \
\
a) My application doesn\'92t end correcly. Either if VLC is closed or the user terminates the execution via the terminal, some error messages appear.\
b) Prints from the VLC are sent to the terminal as well.\
\
\
2.\
a) If for some reason the HTTP request from the client were fragmented, the proxy wouldn\'92t not work properly.\
b) If the API of the third parties I use is different on the customer\'92s environment, the proxy will not work properly.\
c) Different VLC version or changes in the server SSL attributes might cause problem in the connection.\
\
\
Bonus questions:\
These are the improvements I would do:\
\
a. Run the Proxy\'92s server and SSL client in 2 different threads. Each time the client starts a new session the threads will\
work on different instances of the server and the SSL client.\
b. Treat the scenario of fragmented HTTP data coming from VLC.\
c. Use smart pointers.\
d. End the client connection properly when the server ends it.\
e. Support gracefully shutting down the application.\
f. I\'92d learn more about how Boost\'92s ASIO works.\
e. Check for memory leaks.\
f. Support other versions of TLS/SSL.}