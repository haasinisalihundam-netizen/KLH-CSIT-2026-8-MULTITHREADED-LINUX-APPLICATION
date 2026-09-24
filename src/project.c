#include <stdio.h>
#include <pthread.h>
#include <sched.h>

#define NUM_THREADS 8
#define ITERATIONS 100000

long long counter = 0;

pthread_mutex_t mutex;

int use_mutex = 0;

void *worker(void *arg)
{
    for (int i = 0; i < ITERATIONS; i++)
    {
        if (use_mutex)
        {
            pthread_mutex_lock(&mutex);

            counter++;

            pthread_mutex_unlock(&mutex);
        }
        else
        {
            long long temp = counter;

            if (i % 100 == 0)
            {
                sched_yield();
            }

            counter = temp + 1;
        }
    }

    return NULL;
}

void run_test(int mutex_enabled)
{
    pthread_t threads[NUM_THREADS];

    counter = 0;
    use_mutex = mutex_enabled;

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_create(
            &threads[i],
            NULL,
            worker,
            NULL
        );
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(
            threads[i],
            NULL
        );
    }

    long long expected =
        (long long) NUM_THREADS * ITERATIONS;

    printf("\nExpected Result : %lld\n", expected);
    printf("Actual Result   : %lld\n", counter);

    if (counter == expected)
    {
        printf("Status          : CORRECT / THREAD SAFE\n");
    }
    else
    {
        printf("Status          : RACE CONDITION DETECTED\n");
        printf("Lost Updates    : %lld\n",
               expected - counter);
    }
}

int main()
{
    int choice;

    pthread_mutex_init(&mutex, NULL);

    printf("\n=====================================\n");
    printf(" MULTITHREADED LINUX APPLICATION\n");
    printf("=====================================\n");

    printf("\n1. Run WITHOUT Mutex\n");
    printf("2. Run WITH Mutex\n");
    printf("3. Run BOTH\n");

    printf("\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("\n--- WITHOUT MUTEX ---\n");
        run_test(0);
    }

    else if (choice == 2)
    {
        printf("\n--- WITH MUTEX ---\n");
        run_test(1);
    }

    else if (choice == 3)
    {
        printf("\n--- WITHOUT MUTEX ---\n");
        run_test(0);

        printf("\n--- WITH MUTEX ---\n");
        run_test(1);
    }

    else
    {
        printf("\nInvalid choice!\n");
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
