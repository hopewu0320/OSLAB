#include "syscall.h"

int main() {
  int n;
  for (n = 1; n < 100; ++n);
  PrintInt(1);
  //cout<<"1 First round"<<endl;
  for (n = 1; n < 20000; ++n);
  //cout<<"1 second round"<<endl;
  Exit(1);
}
