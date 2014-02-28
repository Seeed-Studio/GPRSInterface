/*
  GPRS.cpp
  2014 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:lawliet zou(lawliet.zou@gmail.com)
  2014-2-24

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "GPRS.h"

GPRS* GPRS::inst;

GPRS::GPRS(int tx, int rx, int baudRate, const char* apn, const char* userName, const char* passWord) : Modem(tx,rx,baudRate)
{
    inst = this;
    _apn = apn;
    _userName = userName;
    _passWord = passWord;
    socketID = -1;
}

bool GPRS::preInit()
{
    for(int i = 0; i < 2; i++) {
        sendCmd("AT\r\n");
        delay(1000);
    }
    return checkSIMStatus();
}

bool GPRS::checkSIMStatus(void)
{
    char gprsBuffer[32];
    int count = 0;
    cleanBuffer(gprsBuffer,32);
    while(count < 3) {
        sendCmd("AT+CPIN?\r\n");
        readBuffer(gprsBuffer,32,DEFAULT_TIMEOUT);
        if((NULL != strstr(gprsBuffer,"+CPIN: READY"))) {
            break;
        }
        count++;
        delay(1000);
    }
    if(count == 3) {
        return false;
    }
    return true;
}

bool GPRS::join()
{
    char cmd[64];
    char ipAddr[32];
    //Select multiple connection
    sendCmdAndWaitForResp("AT+CIPMUX=1\r\n","OK",DEFAULT_TIMEOUT,CMD);

    //set APN
    snprintf(cmd,sizeof(cmd),"AT+CSTT=\"%s\",\"%s\",\"%s\"\r\n",_apn,_userName,_passWord);
    sendCmdAndWaitForResp(cmd, "OK", DEFAULT_TIMEOUT,CMD);

    //Brings up wireless connection
    sendCmdAndWaitForResp("AT+CIICR\r\n","OK",DEFAULT_TIMEOUT,CMD);

    //Get local IP address
    sendCmd("AT+CIFSR\r\n");
    readBuffer(ipAddr,32,2);

    if(NULL != strstr(ipAddr,"AT+CIFSR")) {
        _ip = str_to_ip(ipAddr+12);
        if(_ip != 0) {
            return true;
        }
    }
    return false;
}

bool GPRS::setProtocol(int socket, Protocol p)
{
    if (socket < 0 || socket > MAX_SOCK_NUM-1) {
        return false;
    }
    //ToDo: setProtocol
    return true;
}

bool GPRS::connect(int socket, Protocol ptl,const char * host, int port, int timeout)
{
    char cmd[64];
    char resp[96];
    if (socket < 0 || socket > MAX_SOCK_NUM-1) {
        return false;
    }
    if(ptl == TCP) {
        sprintf(cmd, "AT+CIPSTART=%d,\"TCP\",\"%s\",%d\r\n",socket, host, port);
    } else if(ptl == UDP) {
        sprintf(cmd, "AT+CIPSTART=%d,\"UDP\",\"%s\",%d\r\n",socket, host, port);
    } else {
        return false;
    }
    sendCmd(cmd);
    readBuffer(resp,96,2*DEFAULT_TIMEOUT);
    if(NULL != strstr(resp,"CONNECT")) { //ALREADY CONNECT or CONNECT OK
        return true;
    }
    return false;//ERROR
}

bool GPRS::gethostbyname(const char* host, uint32_t* ip)
{
    uint32_t addr = str_to_ip(host);
    char buf[17];
    snprintf(buf, sizeof(buf), "%d.%d.%d.%d", (addr>>24)&0xff, (addr>>16)&0xff, (addr>>8)&0xff, addr&0xff);
    if (strcmp(buf, host) == 0) {
        *ip = addr;
        return true;
    }
    return false;
}

bool GPRS::disconnect()
{
    sendCmd("AT+CIPSHUT\r\n");
    return true;
}

bool GPRS::is_connected(int socket)
{
    char cmd[16];
    char resp[96];
    snprintf(cmd,16,"AT+CIPSTATUS=%d\r\n",socket);
    sendCmd(cmd);
    readBuffer(resp,sizeof(resp),DEFAULT_TIMEOUT);
    if(NULL != strstr(resp,"CONNECTED")) {
        //+CIPSTATUS: 1,0,"TCP","216.52.233.120","80","CONNECTED"
        return true;
    } else {
        //+CIPSTATUS: 1,0,"TCP","216.52.233.120","80","CLOSED"
        //+CIPSTATUS: 0,,"","","","INITIAL"
        return false;
    }
}

void GPRS::reset()
{

}

bool GPRS::close(int socket)
{
    char cmd[16];
    char resp[16];

    if (socket < 0 || socket > MAX_SOCK_NUM-1) {
        return false;
    }
    // if not connected, return
    if (is_connected(socket) == false) {
        return true;
    }
    snprintf(cmd, sizeof(cmd),"AT+CIPCLOSE=%d\r\n",socket);
    snprintf(resp,sizeof(resp),"%d, CLOSE OK",socket);
    if(0 != sendCmdAndWaitForResp(cmd, resp, DEFAULT_TIMEOUT,CMD)) {
        return false;
    }
    return true;
}

bool GPRS::readable(void)
{
    return readable();
}

int GPRS::wait_readable(int socket, int wait_time)
{
    if (socket < 0 || socket > MAX_SOCK_NUM-1) {
        return -1;
    }
    char resp[16];  
    snprintf(resp,sizeof(resp),"\r\n\r\n+RECEIVE,%d",socket);//"+RECEIVE:<socketID>,<length>"
    int len = strlen(resp);
    if(false == respCmp(resp,len,wait_time)) {
        return -1;
    }
    delay(50);
    char c = readByte();//','
    char dataLen[4];
    int i = 0;
    c = readByte();
    while((c >= '0') && (c <= '9')) {
        dataLen[i++] = c;
        c = readByte();
    }
    c = readByte();//'\n'
    len = atoi(dataLen);

    return len;
}

int GPRS::wait_writeable(int socket, int req_size)
{
    if (socket < 0 || socket > MAX_SOCK_NUM-1) {
        return -1;
    }
    return req_size>256?256:req_size+1;
}

int GPRS::send(int socket, const char * str, int len)
{
    if (socket < 0 || socket > MAX_SOCK_NUM-1) {
        return -1;
    }

    char cmd[32];
    char resp[16];
    delay(1000);
    if(len > 0){
        snprintf(cmd,sizeof(cmd),"AT+CIPSEND=%d,%d\r\n",socket,len);
        if(0 != sendCmdAndWaitForResp(cmd,">",DEFAULT_TIMEOUT,CMD)) {
            return false;
        }
        snprintf(resp,sizeof(resp),"%d, SEND OK",socket);
        if(0 != sendCmdAndWaitForResp(str,resp,DEFAULT_TIMEOUT+2,DATA)) {
            return -1;
        }
    }
    return len;
}

int GPRS::recv(int socket, char* buf, int len)
{
    if (socket < 0 || socket > MAX_SOCK_NUM-1) {
        return -1;
    }
    cleanBuffer(buf,len);
    readBuffer(buf,len,DEFAULT_TIMEOUT/2);
    return len;
    //return strlen(buf);
}

int GPRS::new_socket()
{
    return socketID >= (MAX_SOCK_NUM-1)?(-1):(++socketID);
}

uint16_t GPRS::new_port()
{
    uint16_t port = rand();
    port |= 49152;
    return port;
}

uint32_t GPRS::str_to_ip(const char* str)
{
    uint32_t ip = 0;
    char* p = (char*)str;
    for(int i = 0; i < 4; i++) {
        ip |= atoi(p);
        p = strchr(p, '.');
        if (p == NULL) {
            break;
        }
        ip <<= 8;
        p++;
    }
    return ip;
}
