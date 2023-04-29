#include "syscall.h"

int main() {
  int n;
  for (n = 1; n < 10000; ++n);
  PrintInt(2); //I/O
  //cout<<"2 First round"<<endl;
  for (n = 1; n < 10000; ++n);
  //cout<<"2 second round"<<endl;
  Exit(2);
}
