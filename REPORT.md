# User Level Thread Library Report

### Make sure each line has a max of 80 chars - Will just type in not thinking of max 80 but will compile into the Markdown editior when we are done

### Queue Implementation
In *queue.c*, we used a doubly linked list to implement the queue, where each node has a pointer to the next and previous node. We felt like a doubly linked list would be more sufficient and flexible than an array.
- *queue_create* allocates memory for the queue and initializes the head and tail of the list. *queue_destroys* on the other hand, deallocates the queue's memory.
- *queue_enqueue* creates a new node and adds it at the end of the list by linking it with the tail node through *prev* and *next* variables.
- *queue_delete* iterates through the list until it finds a match. When it does it links the previous node and next node and then deallocates the memory created by the corresponding node.
- *queue_iterate* iterates through the array and execute the given function with each element in the queue.

### Queue Tester
*queue_tester* file contains 19 testcases in order to detect any error in the *queue* implementation. We created testcases for each aspect of every function in the *queue* API.
- *queue_destroy* function is tested when it’s supposed to succeed, when queue is not empty, and when queue is NULL.
- *queue_enqueue* and *queue_dequeue* are tested via several functions in the tester. It tests both functions with both large and small number of elements. It does this by checking if *queue_dequeue* removes the right element and if all elements were added or removed using *queue_length*. In addition, it also handles edge cases when the queue is NULL or empty.
- *queue_delete* is tested with both large and small elements. As well as, edge cases such as a NULL queue or data. In addition, it also tests that the first element is deleted if there are duplicates.
- *queue_iterate* is tested with two functions *even_to_zero* and *odd_to_zero*. 
    - *odd_to_zero* changes elements with odd values to zero and deletes elements with even values. The tester, therefore, tests if even elements were successfully deleted and all odd elements were successfully changed to zero.
    - *even_to_zero* only changes elements with even values to zero, without deleting any element.

### Uthread API

### Semaphore API
In Semaphore, 
- *struct_semaphore* has *queue_blocked* which is the queue of the waiting threads which are in blocked states and not eligible for the scheduling and *count*, an internal counter, to keep track of numbers of available resources.
- *sem_create* initiates and allocates new memory for the new semaphore with *count* and return the pointer.
- *sem_destroy* checkes whether there are threads still being blocked or the memory is NULL; otherwise, deallocate the semaphore.
- *sem_up* releases the resource from the semaphore which increase the number of available reosource, *count++*. If the waiting list of theads is not empty, then the first, oldest, thread gets unblocked by using Utherad API, *uthread_unblock(first thread)*.
- *sem_down* takes the resource from the semaphore which decrease the number of available reosource, *count--*. If there is no avaialbe resource, the requesting thread will get blocked using Uthread API, *uthread_block()*.

### Preemption

### Preemption Tester

### Challenges/Limitations Faced
One of the challenges we faced is with implementing user-level thread library API. It was difficult to try to imagine how threads would be run since it is a relatively new concept to us. In addition, integrating the functions given to us in our API implementation without being able to see some of the functions' implementation. Another challenged we faced is having to figure out a bug that was occurring in a function used by *uthread_ctx_switch*.
  
### References
1. http://www.it.uu.se/education/course/homepage/os/vt18/module-4/simple-threads/ - for better understanding of preemptive scheduling 

###### By *Gharam Alsaedi* & *Seunghyup Alex Oh*

####  AUTHORS.csv Makefile REPORT.md