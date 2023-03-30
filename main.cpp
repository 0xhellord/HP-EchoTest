
#include "Client.h"
#include "Server.h"
#include <cstdint>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void RunServer() {
  CRPCServer RPCServer;
  RPCServer.StartServer();

  uint64_t s_pkts = 0;
  while (1) {
    usleep(1000000);
    uint64_t now_s_pkts = RPCServer.GetTotalPkts();

    printf("Server qps: %lu\n", now_s_pkts - s_pkts);

    s_pkts = now_s_pkts;
  }
}

void RunClient(LPCTSTR host = _T("127.0.0.1"), USHORT port = 5555) {
  CRPCClient RPCClinet;
  RPCClinet.StartClient();

  for (int i = 0; i < 1; i++) {
    RPCClinet.TestSendEcho(host, port);
  }

  uint64_t c_pkts = 0;
  while (1) {
    usleep(1000000);
    uint64_t now_c_pkts = RPCClinet.GetTotalPkts();

    printf("Client QPS: %lu\n", now_c_pkts - c_pkts);

    c_pkts = now_c_pkts;
  }
}

int main(int argc, char *const argv[]) {

  if (strcmp(argv[1], "-s") == 0) {
    RunServer();
  } else if (strcmp(argv[1], "-c") == 0) {
    RunClient(argv[2], atoi(argv[3]));
  }

  getchar();
  return 0;
}