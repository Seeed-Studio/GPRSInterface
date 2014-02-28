/*
  GPRSInterface.cpp
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

#include "GPRSInterface.h"

GPRSInterface::GPRSInterface(int tx, int rx, int baudRate, const char* apn, const char* userName, const char *passWord) : GPRS(tx,rx,baudRate,apn,userName,passWord)
{
    ip_set = false;
}

int GPRSInterface::init()
{
    reset();
    return 0;
}

int GPRSInterface::connect()
{
    return GPRS::join();
}

int GPRSInterface::disconnect()
{
    return GPRS::disconnect();
}

char* GPRSInterface::getIPAddress()
{
    snprintf(ip_string, sizeof(ip_string), "%d.%d.%d.%d", (_ip>>24)&0xff,(_ip>>16)&0xff,(_ip>>8)&0xff,_ip&0xff); 
    ip_set = true;
    return ip_string;
}
