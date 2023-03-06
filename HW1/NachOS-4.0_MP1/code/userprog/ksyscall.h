/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"

#include "synchconsole.h"


void SysHalt()
{
  kernel->interrupt->Halt();
}

void SysPrintInt(int val)
{ 
  DEBUG(dbgTraCode, "In ksyscall.h:SysPrintInt, into synchConsoleOut->PutInt, " << kernel->stats->totalTicks);
  kernel->synchConsoleOut->PutInt(val);
  DEBUG(dbgTraCode, "In ksyscall.h:SysPrintInt, return from synchConsoleOut->PutInt, " << kernel->stats->totalTicks);
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysCreate(char *filename)
{
	// return value
	// 1: success
	// 0: failed
	return kernel->fileSystem->Create(filename);
}

// TODO (Open): Finish kernel interface for system call (Open).
// OpenFileId SysOpen(char *name) {}

// TODO (Read): Finish kernel interface for system call (Read).
// int SysRead(char *buffer, int size, OpenFileId id) {}


// TODO (Write): Finish kernel interface for system call (Write).
// int SysWrite(char *buffer, int size, OpenFileId id) {}

// TODO (Close): Finish kernel interface for system call (Close).
// int SysClose(OpenFileId id) {}

#endif /* ! __USERPROG_KSYSCALL_H__ */