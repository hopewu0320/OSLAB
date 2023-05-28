// scheduler.cc 
//	Routines to choose the next thread to run, and to dispatch to
//	that thread.
//
// 	These routines assume that interrupts are already disabled.
//	If interrupts are disabled, we can assume mutual exclusion
//	(since we are on a uniprocessor).
//
// 	NOTE: We can't use Locks to provide mutual exclusion here, since
// 	if we needed to wait for a lock, and the lock was busy, we would 
//	end up calling FindNextToRun(), and that would put us in an 
//	infinite loop.
//
// 	Very simple implementation -- no priorities, straight FIFO.
//	Might need to be improved in later assignments.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "debug.h"
#include "scheduler.h"
#include "main.h"
#define max(a,b) ((a>b)?a:b)
//----------------------------------------------------------------------
// Scheduler::Scheduler
// 	Initialize the list of ready but not running threads.
//	Initially, no ready threads.
//----------------------------------------------------------------------


static int
compareSR(Thread* t1,Thread* t2)
{
    //選最小Remainging time
    
        if(t1->Remaining > t2->Remaining) return 1;
        else if(t1->Remaining < t2->Remaining) return -1;
        
        return 0;
    
    
}


Scheduler::Scheduler()
{ 
    readyList = new List<Thread *>;
    SRList = new SortedList<Thread *>(compareSR);
    toBeDestroyed = NULL;
} 

//----------------------------------------------------------------------
// Scheduler::~Scheduler
// 	De-allocate the list of ready threads.
//----------------------------------------------------------------------

Scheduler::~Scheduler()
{ 
    delete readyList; 
    delete SRList;
} 

//----------------------------------------------------------------------
// Scheduler::ReadyToRun
// 	Mark a thread as ready, but not running.
//	Put it on the ready list, for later scheduling onto the CPU.
//
//	"thread" is the thread to be put on the ready list.
//----------------------------------------------------------------------

void
Scheduler::ReadyToRun (Thread *thread)
{   
    //int start = kernel->stats->totalTicks;
    

    ASSERT(kernel->interrupt->getLevel() == IntOff);
    DEBUG(dbgThread, "Putting thread on ready list: " << thread->getName());
    DEBUG(dbgSche,"[A] Tick ["<<kernel->stats->totalTicks<<"]: Thread "<<thread->getID()
    <<" is inserted into queue")
	//cout << "Putting thread on ready list: " << thread->getName() << endl ;
    
    //thread->end = kernel->stats->totalTicks;
    //thread->setBurstTime(thread->end-thread->start);
    //cout<<"CPU burstTime: "<<thread->getBurstTime()<<endl;
    
    thread->setStatus(READY);
    
    SRList->Insert(thread); 
    
    
}

//----------------------------------------------------------------------
// Scheduler::FindNextToRun
// 	Return the next thread to be scheduled onto the CPU.
//	If there are no ready threads, return NULL.
// Side effect:
//	Thread is removed from the ready list.
//----------------------------------------------------------------------

Thread *
Scheduler::FindNextToRun ()
{
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    
    if (SRList->IsEmpty()) {
		return NULL;
    } else {
        
        Thread* frontThread  = SRList->RemoveFront();
        DEBUG(dbgSche,"[B] Tick ["<<kernel->stats->totalTicks<<"]: Thread "<<frontThread->getID()
    <<" is removed from queue")
        if(kernel->currentThread->Remaining<frontThread->Remaining){
            SRList->Insert(frontThread);
            return kernel->currentThread ;
        }
    	return frontThread ;
    }
}

//----------------------------------------------------------------------
// Scheduler::Run
// 	Dispatch the CPU to nextThread.  Save the state of the old thread,
//	and load the state of the new thread, by calling the machine
//	dependent context switch routine, SWITCH.
//
//      Note: we assume the state of the previously running thread has
//	already been changed from running to blocked or ready (depending).
// Side effect:
//	The global variable kernel->currentThread becomes nextThread.
//
//	"nextThread" is the thread to be put into the CPU.
//	"finishing" is set if the current thread is to be deleted
//		once we're no longer running on its stack
//		(when the next thread starts running)
//----------------------------------------------------------------------

//分配CPU
void
Scheduler::Run (Thread *nextThread, bool finishing)
{
    
    Thread *oldThread = kernel->currentThread;
    
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    if (finishing) {	// mark that we need to delete current thread
         ASSERT(toBeDestroyed == NULL);
	 toBeDestroyed = oldThread;
    }
    
    if (oldThread->space != NULL) {	// if this thread is a user program,
        oldThread->SaveUserState(); 	// save the user's CPU registers
	oldThread->space->SaveState();
    }
    
    oldThread->CheckOverflow();		    // check if the old thread
					    // had an undetected stack overflow

    kernel->currentThread = nextThread;  // switch to the next thread
    
    nextThread->setStatus(RUNNING);      // nextThread is now running
   

    DEBUG(dbgThread, "Switching from: " << oldThread->getName() << " to: " << nextThread->getName());
    
    // This is a machine-dependent assembly language routine defined 
    // in switch.s.  You may have to think
    // a bit to figure out what happens after this, both from the point
    // of view of the thread and from the perspective of the "outside world".
    
    //int start = kernel->stats->totalTicks;
    //int end = kernel->stats->totalTicks;
    //nextThread->setBurstTime(end-start);
    
    //int start = kernel->stats->totalTicks;
    
    SWITCH(oldThread, nextThread);  //從被block狀態回來
    
    
    //int end = kernel->stats->totalTicks;
    //oldThread->setBurstTime(end-start);
    //cout<<"CPU burstTime: "<<oldThread->getBurstTime()<<endl;
    
    
    // we're back, running oldThread
      
    // interrupts are off when we return from switch!
    ASSERT(kernel->interrupt->getLevel() == IntOff);

    DEBUG(dbgThread, "Now in thread: " << oldThread->getName());

    CheckToBeDestroyed();		// check if thread we were running
					// before this one has finished
					// and needs to be cleaned up
    
    if (oldThread->space != NULL) {	    // if there is an address space
        oldThread->RestoreUserState();     // to restore, do it.
	oldThread->space->RestoreState();
    }
}

//----------------------------------------------------------------------
// Scheduler::CheckToBeDestroyed
// 	If the old thread gave up the processor because it was finishing,
// 	we need to delete its carcass.  Note we cannot delete the thread
// 	before now (for example, in Thread::Finish()), because up to this
// 	point, we were still running on the old thread's stack!
//----------------------------------------------------------------------

void
Scheduler::CheckToBeDestroyed()
{
    if (toBeDestroyed != NULL) {
        delete toBeDestroyed;
	toBeDestroyed = NULL;
    }
}
 
//----------------------------------------------------------------------
// Scheduler::Print
// 	Print the scheduler state -- in other words, the contents of
//	the ready list.  For debugging.
//----------------------------------------------------------------------
void
Scheduler::Print()
{
    cout << "Ready list contents:\n";
    readyList->Apply(ThreadPrint);
}