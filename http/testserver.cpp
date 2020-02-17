#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>

#include <string.h> /* for strncpy */
#include <unistd.h> /* for close */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "httplib.h"
#include <iostream>

using namespace httplib;

void receiveCommand(int command)
{
  printf("Command Received : %d\n", command);
}

int main(void)
{
  Server svr;
  int fd;
  struct ifreq ifr;
  char ipaddr[32];

  fd = socket(AF_INET, SOCK_DGRAM, 0);
  // Retrieve IPv4 IP address
  ifr.ifr_addr.sa_family = AF_INET;
  // Retrieve etho0 IP address
  //strncpy(ifr.ifr_name, "etho0", IFNAMSIZ-1);
  strncpy(ifr.ifr_name, "enp2s0f0", IFNAMSIZ - 1);
  ioctl(fd, SIOCGIFADDR, &ifr);
  close(fd);
  sprintf(ipaddr, "%s", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
  printf("%s\n", ipaddr);

  // Access "http://localhost:1234/hi"
  svr.Get("/hi", [](const Request& req, Response& res) {
    res.set_content("Hello HTTP !", "text/plain");
  });

  // Access "http://localhost:1234/cmd/768" will return 768 as an argument.
  svr.Get(R"(/cmd/(\d+))", [&](const Request& req, Response& res) {
    auto cmd = req.matches[1];
    res.set_content(cmd, "text/plain");
    int command = 0;
    try {
      command = std::stoi(cmd);
    } catch(const std::invalid_argument &e) {
      command = -1;
    } catch(const std::out_of_range &e) {
      command = -2;
    }
    receiveCommand(command);
  });

  //svr.listen("localhost", 1234);
  svr.listen(ipaddr, 1234);
  return 0;
}
