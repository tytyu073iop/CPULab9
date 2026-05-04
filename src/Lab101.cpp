#include <omp.h>
#include <iostream>

int main()
{
    double step;
    for (long num_steps = 100000; num_steps <= 10000000; num_steps *= 10){
        for (int NUM_THREADS = 1; NUM_THREADS <= 8; NUM_THREADS*=2) {
            double start_time = omp_get_wtime();
            int i, nthreads;
            double pi;
            double sum[NUM_THREADS];
            
            step = 1.0 / (double)num_steps;
            omp_set_num_threads(NUM_THREADS);
            
            #pragma omp parallel
            {
                int i, id, nthrds;
                double x;
                
                id = omp_get_thread_num();
                nthrds = omp_get_num_threads();
                
                if (id == 0) 
                nthreads = nthrds;
                
                sum[id] = 0.0;
                
                for (i = id; i < num_steps; i = i + nthrds) {
                    x = (i + 0.5) * step;
                    sum[id] += 4.0 / (1.0 + x * x);
                }
            }
            
            pi = 0.0;
            for (i = 0; i < nthreads; i++)
            pi += sum[i] * step;
            
            double end_time = omp_get_wtime();
            std::cout << "Core amount: " << nthreads << " time: " << (end_time - start_time) << " pi = " << pi << " num_steps: " << num_steps << std::endl;
        }
    }
    
    return 0;
}