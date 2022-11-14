#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int cmpfnc(const void *a, const void *b)
{
    return (((int *)a)[0] - ((int *)b)[0]);
}

struct stud
{
    pthread_t pthread;
    int s_no;
    int arrival_time;
    int washtime;
    int patience;
};

sem_t wm, mutex;
int unfinished = 0;
int wasted_time = 0;
int begin_time;
struct timespec *tm, *bt;

int gettime()
{
    clock_gettime(CLOCK_REALTIME, bt);
    return (bt->tv_sec - begin_time);
}

void *wash(void *arg)
{
    struct stud *stud = (struct stud *)arg;
    int ind = stud->s_no;
    pthread_t *studd = &(stud->pthread);
    sleep(stud->arrival_time);

    printf("%d: Student %d arrives\n", gettime(), ind);

    clock_gettime(CLOCK_REALTIME, tm);
    int time = tm->tv_sec;
    tm->tv_sec += stud->patience;
    tm->tv_nsec += 100000000;   // to give 1/10th delay between this and finishing washing

    if (sem_timedwait(&wm, tm) == -1 && errno == ETIMEDOUT)
    {
        printf("\033[0;31m%d: Student %d leaves without washing\n\033[0;37m", gettime(), ind);
        
        sem_wait(&mutex);
        unfinished++;
        sem_post(&mutex);
        
        wasted_time += stud->patience;
    }
    else
    {
        clock_gettime(CLOCK_REALTIME, tm);
        time = tm->tv_sec - time;
        wasted_time += time;

        printf("\033[0;32m%d: Student %d starts washing\n\033[0;37m", gettime(), ind);
        sleep(stud->washtime);
        
        sem_post(&wm);
        printf("\033[0;33m%d: Student %d leaves after washing\n\033[0;37m", gettime(), ind);
    }
}

int main()
{
    int n, m;
    scanf("%d %d", &n, &m);

    sem_init(&wm, 0, m);
    sem_init(&mutex, 0, 1);

    struct stud *stud[n];

    for(int i=0; i<n; i++)
    {
        stud[i] = (struct stud *)malloc(sizeof(struct stud *));
        stud[i]->s_no = i + 1;
        scanf("%d %d %d", &stud[i]->arrival_time, &stud[i]->washtime, &stud[i]->patience);
    }

    tm = (struct timespec *)malloc(sizeof(struct timespec *));
    bt = (struct timespec *)malloc(sizeof(struct timespec *));
    
    clock_gettime(CLOCK_REALTIME, bt);
    begin_time = bt->tv_sec;

    for(int i=0; i<n; i++)
    {
        pthread_create(&(stud[i]->pthread), NULL, wash, (void *)(stud[i]));
    }

    for(int i=0; i<n; i++)
    {
        pthread_join(stud[i]->pthread, NULL);
    }

    sem_destroy(&wm);
    sem_destroy(&mutex);

    printf("%d\n%d\n", unfinished, wasted_time);

    if (unfinished >= (float)n/4)
    {
        printf("Yes\n");
    }
    else
    {
        printf("No\n");
    }

    return 0;
}