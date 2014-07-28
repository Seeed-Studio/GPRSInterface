/*
 * modem.cpp
 *
 * Copyright (c) 2014 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : lawliet zou
 * Create Time: Feb 2014
 * Change Log :
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
