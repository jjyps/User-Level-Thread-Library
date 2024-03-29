# User Level Thread Library Report

### Queue Implementation
In *queue.c*, we used a doubly linked list to implement the queue, where each 
node has a pointer to the next and previous node. We felt like a doubly linked 
list would be more sufficient and flexible than an array.
- *queue_create* allocates memory for the queue and initializes the head and 
tail of the list. *queue_destroys* on the other hand, deallocates the memory 
taken by queue.
- *queue_enqueue* creates a new node and adds it at the end of the list by 
linking it with the tail node through *prev* and *next* pointer variables.
- *queue_delete* iterates through the list until it finds a match. When it does 
it links the previous node and next node and then deallocates the memory 
created by the corresponding node.
- *queue_iterate* iterates through the array and execute the given function 
with each element in the queue until it reaches the end.

### Queue Tester
*queue_tester* file contains 19 testcases in order to detect any error in the 
*queue* implementation. We created testcases for each aspect of every function 
in the *queue* API.
- *queue_destroy* function is tested when it’s supposed to succeed, when queue 
is not empty, and when queue is NULL. 
- *queue_enqueue* and *queue_dequeue* are tested via several functions in the 
tester. It tests both functions with both large and small number of elements. 
It does this by checking if *queue_dequeue* removes the right element and if 
all elements were added or removed using *queue_length*. In addition, it also 
handles edge cases when the queue is NULL or empty.
- *queue_delete* is tested with both large and small elements. As well as, edge
cases such as a NULL queue or data. In addition, it also tests that the first 
 element is deleted if there are duplicates.
- *queue_iterate* is tested with two functions *even_to_zero* and *odd_to_zero* 
    - *odd_to_zero* changes elements with odd values to zero and deletes 
elements with even values. The tester, therefore, tests if half the elements in 
 the queue were successfully deleted. and that all odd elements were 
successfully changed to zero.
    - *even_to_zero* only changes elements with even values to zero, without 
deleting any element. The tester tests if this is successful.

### Uthread API
Implementation of user level thread library which manages the scheduling of 
threads' context executions. In *uthread_start* which initially starts the 
multithreading library along with *uthread_yield*, the current thread is an 
*idle* thread, behavior of initial access point of the process. The library b
egins with enumerated state to represent each thread's state, *struct uthread_
tcb* which have its own stack, execution context, and *int curr_state* to indi
cate its state, the queue, container, of the threads, and the current thread.
- *uthread_create* first allocates the new memory and properties to the 
*new_thread* of ready state and then enqueues into the queue, *threads*.
- *uthread_yield* switches between the current thread which will become the 
previous thread and be enqueued into the *threads* and the next thread which 
will need to be dequeued from *threads* and become the current thread. We make 
sure the *prev_thread* is no longer in running state but in ready state and the
 *next_thread* to become the running thread; And then, we switch two threads' 
context using *uthread_ctx_switch*. Prevention of the idle thread to be enque
ued is implemented by the condition statement of whether the next thread is *
idle* and there are more than one threads left in the queue. 
- *uthread_start* initialize the queue, *threads*, which is a container of the 
multi threads and allocate the memory for an idle thread with *uthread_ctx_t*.
Set current thread as initial idle thread and change the state to running stat
e and we make sure all the threads finish the execution until there is no more 
threads left in the queue.
- *uthread_exit* makes the state of current thread to *terminated* and then 
yield to the next thread. We free the allocated memory of terminated thread, 
its exectuion context and stack. Once all the threads are finished, add the sav
ed idle thread back to the queue and run for the last.

### Semaphore API
In order to control the access to the common resources by multiple threads, we 
need semaphore to keep track of numbers of available resources.
- *struct_semaphore* has *queue_blocked* which is the queue of the waiting 
threads which are in blocked states and not eligible for the scheduling and
*count*, an internal counter, to keep track of numbers of available resources.
- *sem_create* initiates and allocates new memory for the new semaphore with
*count* and return the pointer.
- *sem_destroy* checks whether there are threads still being blocked or the
memory is NULL; otherwise, deallocate the semaphore.
- *sem_up* releases the resource from the semaphore which increase the number
of available reosource, *count++*. If the waiting list of threads is not empty,
then the first, oldest, thread gets unblocked by using Uthread API, 
*uthread_unblock(first thread)*.
- *sem_down* takes the resource from the semaphore which decrease the number of
available resources, *count--*. If there is no available resource, the 
requesting thread will get blocked using Uthread API, *uthread_block()*.

### Preemption
- *preempt_start* uses a sigaction struct. It assigns *alarm_handler* function
to the struct's handler. Which yields to the next thread if an interruption
occurs and it sets up an alarm using *settimer* such that the frequency of the
preemption is 100 HZ. 
- *preempt_stops* returns the default action of the signal. It does this by 
setting the flag to *SA_RESETHAND*.
- *preempt_disable*/*preempt_enable* block/unblock the signal *SIGVTALRM* by
creating a sigset and adding *SIGVTALRM* to the set. It uses the function 
*sigprocmask* & SIG_BLOCK/SIG_UNBLOCK to block/unblock the alarm signal.

### Preemption Tester
To test the preemption. We created two threads *thread1* and *thread2*.
*thread2* has an infinite while loop and will therefore take the CPU forever,
unless preemption is enabled. In the tester, we test if after *thread1* yields
to *thread2* does it come back to *thread1* and prints "Im back" and exits. If
it does then *preempt.c* passed successfully, if not then it failed the test.
This is because the signal was successfully raised and signal handler function
forced the current thread to yield.

### Challenges/Limitations Faced
One of the challenges we faced is with implementing user-level thread library
API. It was difficult to try to imagine how threads would run since it is a 
relatively new concept to us. In addition, integrating the functions given to 
us in our API implementation without being able to see some of the functions' 
implementation. 

Another challenge we faced is having to figure out a bug that was occurring in
a function used by *uthread_ctx_switch*. We kept getting *Segfault* from
*uthread_ctx_switch* which turns out to be we forgot to check the state of the
threads and also, we were not properly considering the *idle thread* which is
the first, main thread that is used as an entry point to the process and must
not be returned untill all the other threads finish. 

The last challenge we faced is running the *sem_simple.c* tester. We are expec
ted to have "Thread3 Thread2 Thread1". However, we were having "Thread3 Thread1 
Thread2" as an output. That shows that the first main thread which is supposed
to not be returnning until the rest of threads are finished running and returned
is returnned before Thread2. We checked *uthread_unblock* was called from the 
thread of which state is not *blocked* and figured we had not implemented the 
prevention for *idle thread* to be enqueued back to the queue in *uthread_yield*
. By globally creating *tcb idle*, making sure it not get accessed untill the 
rest of threads execution are finished and prior terminated threads to be 
terminated, we could pass the tester and memory leak as well.

### References
1. www.it.uu.se/education/course/homepage/os/vt18/module-4/simple-threads/ -
for better understanding of preemptive scheduling 
2. https://man7.org/linux/man-pages/man2/setitimer.2.html - to understand
setitimer
3. http://tldp.org/HOWTO/Program-Library-HOWTO/static-libraries.html - used for
Makefile, to understand static libraries.
4. www.gnu.org/software/libc/manual/html_mono/libc.html#Blocking-Signals
- Used to understand signal blocking.

###### By *Gharam Alsaedi* & *Seunghyup Alex Oh*
