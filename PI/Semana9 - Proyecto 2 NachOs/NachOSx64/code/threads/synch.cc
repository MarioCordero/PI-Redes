// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(const char* debugName, int initialValue)
{
    name = (char *)debugName;
    value = initialValue;
    queue = new List<Thread*>;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append(currentThread);		// so go to sleep
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    interrupt->SetLevel(oldLevel);		// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    interrupt->SetLevel(oldLevel);
}

#ifdef USER_PROGRAM
//----------------------------------------------------------------------
// Semaphore::Destroy
// 	Destroy the semaphore, freeing the waiting threads
//	This is used to destroy a user semaphore
//----------------------------------------------------------------------

void Semaphore::Destroy() {
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    while ( (thread = queue->Remove() ) != NULL ) {
        scheduler->ReadyToRun(thread);
    }

    interrupt->SetLevel(oldLevel);
}

#endif


// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock(const char* debugName) {
    this->name = (char *) debugName;
    this->free = true;
    this->semaforo = new Semaphore( this->name , 1 );
}


Lock::~Lock() {
    if(this->semaforo){
        // this->semaforo->Destroy();
        delete this->semaforo;
    }
    this->name = nullptr;
}


void Lock::Acquire() {
    if(this->free){
        this->semaforo->P(); // Wait
    }
    this->free = false;
    this->mio = currentThread;
}


void Lock::Release() {
    if (this->mio != currentThread) { // Manejo de error: el hilo actual no posee el lock
        this->free = true;
        this->mio = NULL;
        this->semaforo->V(); // Free the semaphore
        return;
    }
}


bool Lock::isHeldByCurrentThread() {
   return this->mio == currentThread;
}


Condition::Condition(const char* debugName) {
    this->queue = new List<Thread*>;
}


Condition::~Condition() {
    if(this->queue)
        delete this->queue;
}


void Condition::Wait(Lock* conditionLock) {
    IntStatus oldLevel = interrupt->SetLevel(IntOff); // Disable interrupts
    conditionLock->Release();
    this->queue->Append(currentThread); // Add thread to wait queue
    currentThread->Sleep();       // Put thread to sleep

    conditionLock->Acquire();      // Reacquire the lock when woken up
    interrupt->SetLevel(oldLevel); // Restore interrupt state
}


void Condition::Signal(Lock* conditionLock) {
    IntStatus oldLevel = interrupt->SetLevel(IntOff); // Disable interrupts
    if (!queue->IsEmpty()) {
        Thread* thread = queue->Remove();  // Remove one waiting thread
        scheduler->ReadyToRun(thread);     // Wake the thread
    }
    interrupt->SetLevel(oldLevel); // Restore interrupt state
}


void Condition::Broadcast(Lock* conditionLock) {
    IntStatus oldLevel = interrupt->SetLevel(IntOff); // Disable interrupts

    while (!queue->IsEmpty()) {
        Thread* thread = this->queue->Remove();  // Remove all waiting threads
        scheduler->ReadyToRun(thread);     // Wake all threads
    }

    interrupt->SetLevel(oldLevel); // Restore interrupt state
}


// Mutex class
Mutex::Mutex(const char* debugName) {
    name = (char*) debugName;
    isLocked = false;               // Initially, the mutex is unlocked
    owner = nullptr;                // No thread owns the mutex initially
    waitingQueue = new List<Thread*>;  // Initialize the queue for waiting threads
}

Mutex::~Mutex() {
    delete waitingQueue;            // Clean up the waiting queue
}

void Mutex::Lock() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);  // Disable interrupts

    if (isLocked) {                  // If the mutex is already locked
        waitingQueue->Append(currentThread);  // Add current thread to the waiting queue
        currentThread->Sleep();      // Put the current thread to sleep until it's woken up
    } else {
        isLocked = true;             // Lock the mutex
        owner = currentThread;       // Set the current thread as the owner of the mutex
    }

    interrupt->SetLevel(oldLevel);   // Restore interrupts
}

void Mutex::Unlock() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);  // Disable interrupts

    if (owner != currentThread) {    // Error handling: only the owner can unlock the mutex
        interrupt->SetLevel(oldLevel); // Restore interrupts before returning
        return;
    }

    if (!waitingQueue->IsEmpty()) {                     // If there are threads waiting
        Thread* nextThread = waitingQueue->Remove();    // Remove one waiting thread
        scheduler->ReadyToRun(nextThread);              // Wake it up and make it ready to run
    } else {
        isLocked = false;            // Unlock the mutex
        owner = nullptr;             // No thread owns the mutex anymore
    }

    interrupt->SetLevel(oldLevel);   // Restore interrupts
}


// Barrier class
Barrier::Barrier(const char* debugName, int threadCount) {
    name = (char*) debugName;                   // Set debugging name
    count = threadCount;                        // Set the number of threads to wait for
    waiting = 0;                                // Initially, no threads are waiting
    barrierLock = new Lock(debugName);          // Initialize a lock for the barrier
    barrierSemaphore = new Semaphore(debugName, 0);  // Semaphore initialized with 0
}

Barrier::~Barrier() {
    delete barrierLock;
    delete barrierSemaphore;
}

void Barrier::Wait() {
    barrierLock->Acquire();                      // Acquire the lock to update the waiting count

    waiting++;                                   // Increment the number of waiting threads
    if (waiting == count) {                      // If the required number of threads have arrived
        for (int i = 0; i < count; i++) {        // Release all threads
            barrierSemaphore->V();               // Signal the semaphore to wake waiting threads
        }
        waiting = 0;                             // Reset the count for reuse
    }

    barrierLock->Release();                      // Release the lock
    barrierSemaphore->P();                       // Block the thread if not all have arrived
}