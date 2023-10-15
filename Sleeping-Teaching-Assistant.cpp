#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

sem_t chair_sem[3];
sem_t student;
sem_t ta_sleep;

pthread_t *Students;
pthread_t TA;

int chair_count = 0;
int index_chair = 0;
int helped = 0;
pthread_mutex_t ChairAccess;


void *TA_check(void *arg)
{
    while (helped < 1)
    {
        sem_wait(&ta_sleep);
        //printf("Student Arrives\n");
        printf("TA has been awakened by the student.\n");

        while (1)
        {
            pthread_mutex_lock(&ChairAccess);
            if (chair_count == 0)
            {
                pthread_mutex_unlock(&ChairAccess);
                break;
            }

            sem_post(&chair_sem[index_chair]);
            chair_count--;
            printf("Student leaves chair in waiting room and goes to the TA. Remaining Chairs = %d\n", 3 - chair_count);
            index_chair = (index_chair + 1) % 3;
            pthread_mutex_unlock(&ChairAccess);

            printf("TA is currently helping the student.\n");
            sleep(2);
            sem_post(&student);
            usleep(1000);
        }
    }
    return NULL;
}


void *Student_Check(void *threadID)
{
    long tid = (long)threadID+1;
    int Time_with_ta;

    while (helped < 1)
    {
        printf("Student %ld needs help from the TA\n", tid);
        pthread_mutex_lock(&ChairAccess);
        int count = chair_count;
        pthread_mutex_unlock(&ChairAccess);

        if (count < 3)
        {
            if (count == 0)
                sem_post(&ta_sleep);
            else
                printf("Student %ld sat on chair. Chairs Remaining: %d\n", tid, 3 - chair_count);

            pthread_mutex_lock(&ChairAccess);
            int index = (index_chair + chair_count) % 3;
            chair_count++;
            pthread_mutex_unlock(&ChairAccess);

            sem_wait(&chair_sem[index]);
            printf("Student %ld is getting help from the TA. \n", tid);
            sem_wait(&student);
            printf("Student %ld leaves the TA room.\n\n", tid);
        }
        else
            printf("Student %ld will return at a later time. \n", tid);

        helped++;
    }

    return NULL;
}

int main()
{
    int number_of_students;
    printf("Enter number of students: ");
    scanf("%d", &number_of_students);

    if (number_of_students == 0)
    {
        printf("TA continues sleeping");
        exit(0);
    }

    int id;
    srand(time(NULL));

    sem_init(&ta_sleep, 0, 0);
    sem_init(&student, 0, 0);

    for (id = 0; id < 3; ++id)
        sem_init(&chair_sem[id], 0, 0);

    pthread_mutex_init(&ChairAccess, NULL);
    Students = (pthread_t *)malloc(sizeof(pthread_t) * number_of_students);

    pthread_create(&TA, NULL, TA_check, NULL);

    for (id = 0; id < number_of_students; id++)
        pthread_create(&Students[id], NULL, Student_Check, (void *)(long)id);

    pthread_join(TA, NULL);

    for (id = 0; id < number_of_students; id++)
        pthread_join(Students[id], NULL);

    free(Students);
    return 0;
}
