GPRSInterface
-------------------------------------------------------------
GPRS(General packet radio service), is a packet oriented mobile data service on the 2G and 3G cellular communication system's global system for mobile communications (GSM). the GPRSInterface is a library to use GPRS like Ethernet or WiFi, you can have a try with a GPRS module(ex. [Seeed_GPRS_Sheild](http://www.seeedstudio.com/wiki/GPRS_Shield_V2.0))

<br>
## Usage:
The folowing we will introduce some function which is used very normal. 
### Initialization
At the very beginning, we should had it initialized. You can use this function :
 
    GPRSInterface gprs(PIN_TX,PIN_RX,19200,"cmnet",NULL,NULL)
    gprs.init();

PIN_TX and PIN_RX is the serial communication pin with GPRS module
19200 is the baud rate of serial communication
"cmnet" is the [APN](http://en.wikipedia.org/wiki/Access_Point_Name) for GPRS service
the first NULL is the user name for APN
the second NULL is the password for APN 

<br>
### Connect the Internet

you can connect the Internet as the following : 

    gprs.connect();


<br>
### Get IP Address

GPRS will get IP address by using DHCP, if connect success, you can get the IP address as following :

    gprs.getIPAddress();

<br>
### Send/Recv Data

you can create a TCPSocketConnection instance to send/receive data now :

    TCPSocketConnection sock;
    sock.connect(HOST, PORT);
    sock.send(DATA,LENGTH);
    sock.receive(BUFFER,LENGTH);

<br>
### CLOSE Socket

    sock.close();

<br>
### Disconnect

    gprs.disconnect();

<br>
----
This software is written by lawliet zou[wei.zou@seeedstudio.com](wei.zou@seeedstudio.com) for seeed studio<br>
and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt for more information.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed Studio is an open hardware facilitation company based in Shenzhen, China. <br>
Benefiting from local manufacture power and convenient global logistic system, <br>
we integrate resources to serve new era of innovation. Seeed also works with <br>
global distributors and partners to push open hardware movement.<br>



[![Analytics](https://ga-beacon.appspot.com/UA-46589105-3/GPRSInterface)](https://github.com/igrigorik/ga-beacon)






