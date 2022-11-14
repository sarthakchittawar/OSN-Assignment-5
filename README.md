# Notes to evaluator:

1. I have created a total of 2 semaphores, `wm` which holds the value of the number of free washing machines, and `mutex`(mutex lock) which locks global variables when being modified by one thread.

2. **The algorithm followed is as follows:**
* We initialise 1 pthread for each of the 'n' students at the beginning.
* The pthread executes a function `wash`, in which there is initially a sleep() for `T_i` seconds until the student arrives at the scene.
* After he/she arrives, the `wm` semaphore is checked to identify if any washing machine is free to use. If it is not, then the student will wait for `P_i` seconds for a machine to get empty, else leave w/o washing. This has been implemented using the `sem_timedwait` function (returns -1 if semaphore is <=0 after it reaches a given timestamp).
* When the `wm` semaphore value is > 0, i.e. if a washing machine is free to use, the student starts washing and semaphore is decremented. We implement sleep(`W_i`) before incrementing the semaphore to simulate the washing time.
* In all such cases, the waiting time of every student is added up to a global variable, and also the count of students who left w/o washing.

3. FCFS is always followed. However there are certain cases like in sample output 1, Student 1 & 3 arrive at the same time. So the order on which pthread is created first is non-deterministic (depending on whether the processor follows FCFS or not) and hence outputs could vary in the 2 cases.
However, if Student 1 comes before student 3 and both are waiting, Student 1 WILL use a washing machine before 3, neglecting patience issues. **No queue has been used for FCFS.**

4. Let us say if Student 4 is going to finish washing at 5s, and Student 1 is waiting since 2s to wash, with a patience of 3s. Now we are going to assume that Student 4 will finish washing before Student 1's patience gets exhausted in all inputs having such a case.
Thus, to simluate this, we give a slight delay of 0.1s to Student 1 in such a case.

5. The number on the LHS of the output indicates the timestamp at which the checkpoint has occured.

6. Colouring of outputs has been done using printf statements only.

7. The **bonus in Q1 has been done** as well.
