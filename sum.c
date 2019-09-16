#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>

int sum_efficient(int *A, size_t n) {
    // j stores the highest valid index in A
    int j = n - 1;
    int count = 0;
    while (j >= 1) {
        printf("iteration %d\n", count++);
        // e.g. n = 6 needs 3 threads, so 5 / 2 + 1 = 3
        //      n = 3 needs 2 threads, so 2 / 2 + 1 = 2
        //      n = 1 needs 1 thread, so 1 / 2 + 1 = 1
        #pragma omp parallel num_threads(j / 2 + 1)
        {
            int k = omp_get_thread_num();
            int x = A[2 * k];
            // for odd n, this could be an invalid index, so check
            int y = 0;
            if (2 * k + 1 <= j) {
                y = A[2 * k + 1];
            }
            int s = x + y;
            // exclusive read: all threads must have read before continuing
            printf("thread %d: %d+%d=%d\n", k, x, y, s);
            #pragma omp barrier
            A[k] = s;
        }
        j /= 2;
        puts("---");
    }
    return A[0];
}

int main() {
    const int SIZE = 100;
    //srand(time(NULL));

    int array[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        array[i] = rand() % 10;
        printf("%d, ", array[i]);
    }
    puts("");

    omp_set_dynamic(0);
    omp_set_num_threads(SIZE);

    printf("sum: %d\n", sum_efficient(array, SIZE));
}