#include "syscall.h"
//#include "stdio.h"
int main() {
  int n;
  for (n = 1; n < 100; ++n);
  PrintInt(1);
  
  //MSG("1 First round");
  for (n = 1; n < 20000; ++n);
  
  Exit(1);
}
