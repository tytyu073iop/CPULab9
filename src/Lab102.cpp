#include <omp.h>
#include <iostream>

int main()
{
    double step;
    for (long num_steps = 100000; num_steps <= 10000000; num_steps *= 10){
        for (int NUM_THREADS = 1; NUM_THREADS <= 8; NUM_THREADS*=2) {
            double start_time = omp_get_wtime();
            int i, nthreads = NUM_THREADS;
            double pi;
            double sum = 0;
            
            step = 1.0 / (double)num_steps;
            omp_set_num_threads(NUM_THREADS);
            
            #pragma omp parallel for reduction(+:sum)
            for (int i = 0; i < num_steps; i++) {
                double x;
                x = (i + 0.5) * step;
                sum += 4.0 / (1.0 + x * x);
            }
            
            pi = sum * step;
            
            double end_time = omp_get_wtime();
            std::cout << "Core amount: " << nthreads << " time: " << (end_time - start_time) << " pi = " << pi << " num_steps: " << num_steps << std::endl;
        }
    }
    
    return 0;
}