##GPRSInterface
* **GPRS(General packet radio service)**, is a packet oriented mobile data service on the 2G and 3G cellular communication system's global system for mobile communications (GSM).<br> 
* the **GPRSInterface** is a library to use GPRS like Ethernet or WiFi, you can have a try with a GPRS module(ex. [Seeed_GPRS_Sheild](http://www.seeedstudio.com/wiki/GPRS_Shield_V2.0))<br>

### Usage:
The folowing we will introduce some function which is used very normal. 
#### Initialization
At the very beginning, we should had it initialized. You can use this function :
 
    GPRSInterface gprs(PIN_TX,PIN_RX,19200,"cmnet",NULL,NULL)
    gprs.init();

**PIN_TX** and **PIN_RX** is the serial communication pin with GPRS module<br>
**19200** is the baud rate of serial communication<br>
**"cmnet"** is the [APN](http://en.wikipedia.org/wiki/Access_Point_Name) for GPRS service<br>
the first **NULL** is the user name for APN<br>
the second **NULL** is the password for APN <br>

#### Connect the Internet

you can connect the Internet as the following : 

    gprs.connect();

#### Get IP Address

GPRS will get IP address by using **DHCP**, if connect success, you can get the IP address as following :

    gprs.getIPAddress();

#### Send/Recv Data

you can create a TCPSocketConnection instance to send/receive data now :

    TCPSocketConnection sock;
    sock.connect(HOST, PORT);
    sock.send(DATA,LENGTH);
    sock.receive(BUFFER,LENGTH);

#### CLOSE Socket

    sock.close();

#### Disconnect

    gprs.disconnect();

----
This software is written by lawliet zou (![](http://www.seeedstudio.com/wiki/images/f/f8/Email-lawliet.zou.jpg)) for [Seeed Technology Inc.](http://www.seeed.cc) and is licensed under [The MIT License](http://opensource.org/licenses/mit-license.php). Check License.txt/LICENSE for the details of MIT license.<br>

Contributing to this software is warmly welcomed. You can do this basically by<br>
[forking](https://help.github.com/articles/fork-a-repo), committing modifications and then [pulling requests](https://help.github.com/articles/using-pull-requests) (follow the links above<br>
for operating guide). Adding change log and your contact into file header is encouraged.<br>
Thanks for your contribution.

Seeed is a hardware innovation platform for makers to grow inspirations into differentiating products. By working closely with technology providers of all scale, Seeed provides accessible technologies with quality, speed and supply chain knowledge. When prototypes are ready to iterate, Seeed helps productize 1 to 1,000 pcs using in-house engineering, supply chain management and agile manufacture forces. Seeed also team up with incubators, Chinese tech ecosystem, investors and distribution channels to portal Maker startups beyond.



[![Analytics](https://ga-beacon.appspot.com/UA-46589105-3/GPRSInterface)](https://github.com/igrigorik/ga-beacon)

