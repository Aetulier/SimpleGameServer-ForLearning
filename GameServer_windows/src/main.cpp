#include "Servers/Server.h"
#include <iostream>
#include <stdlib.h>

int main()
{
  system("chcp 65001");
  
  Server server;
  server.Start();
  system("pause");
  return 0;
}