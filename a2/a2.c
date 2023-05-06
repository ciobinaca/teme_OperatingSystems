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

int ok4=0,ok11=0,n=0;

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
//         n++;
//    if(s->nrThread==11)
//    {
//        info(BEGIN,4,11);
//        ok11=1;
//
//     printf("%d\n",n);
//
//    }
//    else if(ok11==1)
//    {
//    for(int i=0;i<5;i++)
//      {info(BEGIN,4,s->nrThread);
//      info(END,4,s->nrThread);
//      }
//      ok11=0;
//      info(END,4,11);}
  //  else
    //{
      info(BEGIN,4,s->nrThread);
      info(END,4,s->nrThread);

   // }
    sem_post(s->logSem);
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
    int i;

    TH_STRUCT params[NR_THREADS];
    TH_STRUCT params2[NR_THREADS2];
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
