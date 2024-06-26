#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define num_steps 10000000

void calculate_pi(int scheduling_type, int chunk_size, int num_threads, double *pi, double *execution_time) {
    double x, step, sum = 0.0;

    // Set the number of threads
    omp_set_num_threads(num_threads);

    // Choose the scheduling type based on the command line argument
    switch(scheduling_type) {
        case 1:
            omp_set_schedule(omp_sched_static, chunk_size);
            break;
        case 2:
            omp_set_schedule(omp_sched_dynamic, chunk_size);
            break;
        case 3:
            omp_set_schedule(omp_sched_guided, chunk_size);
            break;
        default:
            *execution_time = -1.0;
            return;
    }

    step = 1.0 / (double) num_steps;
    double start_time = omp_get_wtime();

    // Parallel loop
    #pragma omp parallel for schedule(runtime) reduction(+:sum) private(x)
    for (long i = 0; i < num_steps; i++) {
        x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }

    *pi = step * sum;
    double end_time = omp_get_wtime();

    // Store the results
    *execution_time = end_time - start_time;
}
