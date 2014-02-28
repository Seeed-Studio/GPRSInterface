/*
  modem.cpp
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

#include "modem.h"

char Modem::readByte(void)
{
    return serialModem.read();
}

bool Modem::readable()
{
    return serialModem.available();
}

int Modem::readBuffer(char *buffer,int count, unsigned int timeOut)
{
    int i = 0;
	unsigned long timerStart,timerEnd;
	timerStart = millis();
    timerEnd = 1000*timeOut + timerStart;
    while(1) {
        while (serialModem.available()) {
            char c = serialModem.read();
            buffer[i++] = c;
            if(i >= count)break;
        }
        if(i >= count)break;
		if(millis() > timerEnd) {
            break;
        }
    }
    return 0;
}

void Modem::cleanBuffer(char *buffer, int count)
{
    for(int i=0; i < count; i++) {
        buffer[i] = '\0';
    }
}

void Modem::sendCmd(const char* cmd)
{
    serialModem.write(cmd);
}

void Modem::sendATTest(void)
{
    sendCmdAndWaitForResp("AT\r\n","OK",DEFAULT_TIMEOUT,CMD);
}

bool Modem::respCmp(const char *resp, unsigned int len, unsigned int timeout)
{
    int sum=0;
	unsigned long timerStart,timerEnd;
	timerStart = millis();
    timerEnd = 1000*timeout + timerStart;
    while(1) {
        if(serialModem.available()) {
            char c = serialModem.read();
            sum = (c==resp[sum]) ? sum+1 : 0;
            if(sum == len)break;
        }
		if(millis() > timerEnd) {
            return false;
        }
    }

    return true;
}

int Modem::waitForResp(const char *resp, unsigned int timeout,DataType type)
{
    int len = strlen(resp);
    int sum = 0;
	unsigned long timerStart,timerEnd;
	timerStart = millis();
    timerEnd = 1000*timeout + timerStart;
    while(1) {
        if(serialModem.available()) {
            char c = serialModem.read();
            sum = (c==resp[sum]) ? sum+1 : 0;
            if(sum == len)break;
        }
        if(millis() > timerEnd) {
            return -1;
        }
    }

    if(type == CMD) {
        while(serialModem.available()) {
            char c = serialModem.read();
        }
    }

    return 0;
}

int Modem::sendCmdAndWaitForResp(const char* data, const char *resp, unsigned timeout,DataType type)
{
    sendCmd(data);
    return waitForResp(resp,timeout,type);
}
