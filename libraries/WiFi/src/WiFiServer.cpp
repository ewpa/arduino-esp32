/*
  Server.cpp - Server class for Raspberry Pi
  Copyright (c) 2016 Hristo Gochkov  All right reserved.

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
#include "WiFiServer.h"
#include <lwip/sockets.h>
#include <lwip/netdb.h>

#undef write
#undef close

int WiFiServer::setTimeout(uint32_t seconds){
  struct timeval tv;
  tv.tv_sec = seconds;
  tv.tv_usec = 0;
  if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval)) < 0)
    return -1;
  return setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));
}

size_t WiFiServer::write(const uint8_t *data, size_t len){
  return 0;
}

void WiFiServer::stopAll(){}

WiFiClient WiFiServer::available(){
  if(!_listening)
    return WiFiClient();
  int client_sock;
  if (_accepted_sockfd >= 0) {
    client_sock = _accepted_sockfd;
    _accepted_sockfd = -1;
  }
  else {
  struct sockaddr_in _client;
  int cs = sizeof(struct sockaddr_in);
    client_sock = lwip_accept_r(sockfd, (struct sockaddr *)&_client, (socklen_t*)&cs);
  }
  if(client_sock >= 0){
    int val = 1;
    if(setsockopt(client_sock, SOL_SOCKET, SO_KEEPALIVE, (char*)&val, sizeof(int)) == ESP_OK) {
      val = _noDelay;
      if(setsockopt(client_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&val, sizeof(int)) == ESP_OK)
        return WiFiClient(client_sock);
    }
  }
  return WiFiClient();
}

void WiFiServer::begin(uint16_t port){
  if(_listening)
    return;
  if(port){
      _port = port;
  }
  int n, ret;
  struct addrinfo hints, *addr_list, *cur;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  if (getaddrinfo("::", inet_ntoa(_port), &hints, &addr_list) != 0)
    return;
  /* Try the sockaddrs until a binding succeeds */
  ret = 1; // unknown host
  for (cur = addr_list; cur != NULL; cur = cur->ai_next)
  {
    sockfd = (int) socket(cur->ai_family, cur->ai_socktype, cur->ai_protocol);
    if (sockfd < 0)
    {
      //ret = 2; // socket failed
      continue;
    }

    n = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *) &n,
                   sizeof(n)) != 0)
    {
      lwip_close_r(sockfd);
      //ret = 3; // setsockopt failed
      continue;
    }

    if (bind(sockfd, cur->ai_addr, cur->ai_addrlen) != 0)
    {
      lwip_close_r(sockfd);
      //ret = 4; // bind failed
      continue;
    }

    if (listen(sockfd, _max_clients) != 0)
    {
      lwip_close_r(sockfd);
      //ret = 5; // listen failed
      continue;
    }

    /* Bind was successful */
    ret = 0;
    break;
  }

  freeaddrinfo(addr_list);

  if (ret != 0)
    return;
  fcntl(sockfd, F_SETFL, O_NONBLOCK);
  _listening = true;
  _noDelay = false;
  _accepted_sockfd = -1;
}

void WiFiServer::setNoDelay(bool nodelay) {
    _noDelay = nodelay;
}

bool WiFiServer::getNoDelay() {
    return _noDelay;
}

bool WiFiServer::hasClient() {
    if (_accepted_sockfd >= 0) {
      return true;
    }
    struct sockaddr_in _client;
    int cs = sizeof(struct sockaddr_in);
    _accepted_sockfd = lwip_accept_r(sockfd, (struct sockaddr *)&_client, (socklen_t*)&cs);
    if (_accepted_sockfd >= 0) {
      return true;
    }
    return false;
}

void WiFiServer::end(){
  lwip_close_r(sockfd);
  sockfd = -1;
  _listening = false;
}

void WiFiServer::close(){
  end();
}

void WiFiServer::stop(){
  end();
}

