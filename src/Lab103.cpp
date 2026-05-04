#include <omp.h>
#include <iostream>
#include <vector>

// Функция вычисления частичной суммы на интервале [a, b)
double compute_partial_sum(double step, long start, long end) {
    double sum = 0.0;
    for (long i = start; i < end; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    return sum;
}

// Разделяй и властвуй: рекурсивное разбиение задачи
double divide_and_conquer(double step, long start, long end, int depth, int max_depth) {
    // Базовый случай: достаточно маленький интервал
    if (end - start <= 10000 || depth >= max_depth) {
        return compute_partial_sum(step, start, end);
    }
    
    // Разделение на две половины
    long mid = start + (end - start) / 2;
    
    double left_sum, right_sum;
    
    #pragma omp task shared(left_sum)
    left_sum = divide_and_conquer(step, start, mid, depth + 1, max_depth);
    
    #pragma omp task shared(right_sum)
    right_sum = divide_and_conquer(step, mid, end, depth + 1, max_depth);
    
    #pragma omp taskwait
    
    return left_sum + right_sum;
}

int main() {
    for (long num_steps = 100000; num_steps <= 10000000; num_steps *= 10) {
        for (int NUM_THREADS = 1; NUM_THREADS <= 8; NUM_THREADS *= 2) {
            double start_time = omp_get_wtime();
            
            double step = 1.0 / (double)num_steps;
            double pi;
            double sum = 0.0;
            
            omp_set_num_threads(NUM_THREADS);
            
            #pragma omp parallel
            {
                #pragma omp single
                {
                    sum = divide_and_conquer(step, 0, num_steps, 0, 8);
                }
            }
            
            pi = sum * step;
            
            double end_time = omp_get_wtime();
            std::cout << "Threads: " << NUM_THREADS 
                      << " | Time: " << (end_time - start_time) 
                      << " | pi = " << pi 
                      << " | steps: " << num_steps << std::endl;
        }
    }
    return 0;
}