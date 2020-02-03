/*
    GPRS.h

    Copyright (c) 2014 seeed technology inc.
    Website    : www.seeed.cc
    Author     : lawliet zou
    Create Time: Feb 2014
    Change Log :

    The MIT License (MIT)

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#ifndef __GPRS_H__
#define __GPRS_H__

#include "arduino.h"
#include "modem.h"

#define DEFAULT_WAIT_RESP_TIMEOUT 500
#define MAX_SOCK_NUM 7 //(0~6)

enum Protocol {
    CLOSED = 0,
    TCP    = 1,
    UDP    = 2,
};

class GPRS: public Modem {

  public:
    /** Constructor
        @param tx mbed pin to use for tx line of Serial interface
        @param rx mbed pin to use for rx line of Serial interface
        @param baudRate serial communicate baud rate
        @param apn name of the gateway for GPRS to connect to the network
        @param userName apn's username, usually is NULL
        @param passWord apn's password, usually is NULL
    */
    GPRS(int tx, int rx, int baudRate, const char* apn, const char* userName = NULL, const char* passWord = NULL);

    /** get instance of GPRS class
    */
    static GPRS* getInstance() {
        return inst;
    };

    /** Connect the GPRS module to the network.
        @return true if connected, false otherwise
    */
    bool join(void);

    /** Disconnect the GPRS module from the network
        @returns true if successful
    */
    bool disconnect(void);

    /** Close a tcp connection
        @returns true if successful
    */
    bool close(int socket);

    /** Open a tcp/udp connection with the specified host on the specified port
        @param socket an endpoint of an inter-process communication flow of GPRS module,for SIM900 module, it is in [0,6]
        @param ptl protocol for socket, TCP/UDP can be choosen
        @param host host (can be either an ip address or a name. If a name is provided, a dns request will be established)
        @param port port
        @param timeout wait seconds till connected
        @returns true if successful
    */
    bool connect(int socket, Protocol ptl, const char* host, int port, int timeout = DEFAULT_TIMEOUT);

    /** Set the protocol (UDP or TCP)
        @param socket socket
        @param p protocol
        @returns true if successful
    */
    bool setProtocol(int socket, Protocol p);

    /** Reset the GPRS module
    */
    void reset();

    /** check if GPRS module is readable or not
        @returns true if readable
    */
    bool readable(void);

    /** wait a few time to check if GPRS module is readable or not
        @param socket socket
        @param wait_time time of waiting
    */
    int wait_readable(int socket, int wait_time);

    /** wait a few time to check if GPRS module is writeable or not
        @param socket socket
        @param wait_time time of waiting
    */
    int wait_writeable(int socket, int req_size);

    /** Check if a tcp link is active
        @returns true if successful
    */
    bool is_connected(int socket);

    /** send data to socket
        @param socket socket
        @param str string to be sent
        @param len string length
        @returns return bytes that actually been send
    */
    int send(int socket, const char* str, int len);

    /** read data from socket
        @param socket socket
        @param buf buffer that will store the data read from socket
        @param len string length need to read from socket
        @returns bytes that actually read
    */
    int recv(int socket, char* buf, int len);

    /** convert the host to ip
        @param host host ip string, ex. 10.11.12.13
        @param ip long int ip address, ex. 0x11223344
        @returns true if successful
    */
    bool gethostbyname(const char* host, uint32_t* ip);

    int new_socket();
    uint16_t new_port();
    uint32_t _ip;

  protected:

    bool preInit();
    bool checkSIMStatus(void);
    uint32_t str_to_ip(const char* str);
    static GPRS* inst;
    int socketID;
    const char* _apn;
    const char* _userName;
    const char* _passWord;
};

#endif
