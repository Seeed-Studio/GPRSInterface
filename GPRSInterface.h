/*
  GPRSInterface.h
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
#ifndef __GPRSINTERFACE_H__

#include "GPRS.h"
#include "TCPSocketConnection.h"
#include "TCPSocketServer.h"

/** Interface using GPRS to connect to an IP-based network
 *
 */
class GPRSInterface: public GPRS
{

public:

    /** Constructor
     *  @param tx mbed pin to use for tx line of Serial interface
     *  @param rx mbed pin to use for rx line of Serial interface
     *  @param baudRate serial communicate baud rate
     *  @param apn name of the gateway for GPRS to connect to the network
     *  @param userName apn's username, usually is NULL
     *  @param passWord apn's password, usually is NULL
     */
    GPRSInterface(int tx, int rx, int baudRate, const char* apn, const char* userName = NULL, const char *passWord = NULL);

    /** Initialize the interface(no connection at this point).
     *  @return 0 on success, a negative number on failure
     */
    int init();

    /** Connect to the network and get IP address
     *  @returns 0 on success, a negative number on failure
     */
    int connect();

    /** Disconnect with the network
     *  @returns 0 on success, a negative number on failure
     */
    int disconnect();

    /** Get IP address
     *  @returns ip address
     */
    char* getIPAddress();

private:

    char ip_string[20];
    bool ip_set;
};

#endif
