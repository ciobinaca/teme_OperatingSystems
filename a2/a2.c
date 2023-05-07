#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "a2_helper.h"
#define NR_THREADS 4
#define NR_THREADS2 37
#include <pthread.h>
#include <semaphore.h>

typedef struct
{
    int nrThread;
    sem_t *logSem;
} TH_STRUCT;

int ok4=0,ok11=0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;
int active_threads = 0;
void* thread_function(void* params)
{
    TH_STRUCT* s=(TH_STRUCT*)params;
    if(s->nrThread==4&&ok4==0)
    {
        ok4=1;
        sem_wait(s->logSem);
    }
    else if(s->nrThread==3)
    {
        //ok3=1;
        info(BEGIN,3,3);

        sem_post(s->logSem);
        info(BEGIN,3,4);
        info(END,3,4);
        info(END,3,3);
    }

    else
    {
        info(BEGIN,3,s->nrThread);
        info(END,3,s->nrThread);
    }

    return NULL;
}

void* thread_function2(void* params)
{
    TH_STRUCT *s=(TH_STRUCT*)params;
    sem_wait(s->logSem);

//    pthread_mutex_lock(&counter_mutex);
//    active_threads++;
//    pthread_mutex_unlock(&counter_mutex);
//    if(s->nrThread==11)
//    {
//        info(BEGIN,4,11);
//        ok11=1;
//    }
//    else
//    {

        info(BEGIN,4,s->nrThread);
        info(END,4,s->nrThread);
//        pthread_mutex_lock(&counter_mutex);
//        active_threads--;
//        pthread_mutex_unlock(&counter_mutex);
//           if(active_threads==6&&ok11==1)
//    {
//        info(END,4,11);
//        pthread_mutex_lock(&counter_mutex);
//        active_threads--;
//        ok11=0;
//        pthread_mutex_unlock(&counter_mutex);
//    }
//    }

    sem_post(s->logSem);
    return NULL;
}

void* thread_function3(void* params)
{
    TH_STRUCT *s=(TH_STRUCT*)params;
    info(BEGIN,5,s->nrThread);
        info(END,5,s->nrThread);
       return NULL;
        }

int main()
{
    init();

    info(BEGIN, 1, 0);
    sem_t logSem,logSem2;
    sem_init(&logSem,0,0);
    sem_init(&logSem2,0,6);

    pthread_t tid[NR_THREADS];
    pthread_t tid2[NR_THREADS2];
    pthread_t tid3[5];
    int i;

    TH_STRUCT params[NR_THREADS];
    TH_STRUCT params2[NR_THREADS2];
    TH_STRUCT params3[5];
    pid_t pid2,pid3,pid4,pid8,pid5,pid6,pid7;
    pid2 = fork();
    if(pid2 == 0)
    {
        info(BEGIN, 2, 0);
        pid3 = fork();
        if(pid3 == 0)
        {

            info(BEGIN, 3, 0);
            pid8=fork();
            if(pid8==0)
            {
                info(BEGIN, 8, 0);
                info(END, 8, 0);
            }
            else
            {
                wait(NULL);
                for(i=0; i<NR_THREADS; i++)

                {
                    params[i].nrThread=i+1;
                    params[i].logSem=&logSem;
                    pthread_create(&tid[i], NULL, thread_function, &params[i]);
                }

                for(i=0; i<NR_THREADS; i++)
                    pthread_join(tid[i], NULL);

                info(END, 3, 0);
            }

        }
        else
        {
            wait(NULL);
            pid5=fork();
            if(pid5==0)
            {
                info(BEGIN,5,0);
                pid6=fork();
                if(pid6==0)
                {
                    info(BEGIN,6,0);
                    info(END,6,0);
                }
                else
                {
                    wait(NULL);
                    pid7=fork();
                    if(pid7==0)
                    {
                        info(BEGIN,7,0);
                        info(END,7,0);
                    }
                    else
                    {
                        wait(NULL);
                        for(i=0; i<5; i++)

            {
                params3[i].nrThread=i+1;
                //params3[i].logSem=&logSem2;
                pthread_create(&tid3[i], NULL, thread_function3, &params3[i]);
            }

            for(i=0; i<5; i++)
                pthread_join(tid3[i], NULL);
                        info(END, 5, 0);
                    }
                }

            }
            else
            {
                wait(NULL);
                info(END, 2, 0);
            }

        }
    }
    else
    {
        wait(NULL);
        pid4=fork();
        if(pid4==0)
        {
            info(BEGIN,4,0);
            for(i=0; i<NR_THREADS2; i++)

            {
                params2[i].nrThread=i+1;
                params2[i].logSem=&logSem2;
                pthread_create(&tid2[i], NULL, thread_function2, &params2[i]);
            }

            for(i=0; i<NR_THREADS2; i++)
                pthread_join(tid2[i], NULL);


            info(END, 4, 0);
        }
        else
        {
            wait(NULL);
            info(END, 1, 0);
        }

    }

    return 0;
}
