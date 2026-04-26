#include <iostream>
#include <vector>
#include <omp.h>

int main() {
    {
        for (int num_of_threads = 2; num_of_threads <= 8; num_of_threads *= 2) {
            for (int load = 10; load <= 20; load += 5) {
                double start_time = omp_get_wtime();
                long long fac = 1;
                #pragma omp parallel for reduction(*:fac)
                for (long long i = 2; i <= load; i++) {
                    fac *= i;
                }
                double end_time = omp_get_wtime();
                std::cout << "thread amount: " << num_of_threads << " load: " << load << " time: " << (end_time - start_time) << " Factorial found: " << fac << std::endl;
            }
        }
    }
    
    std::cout << '\n' << "factorial with reduction complete" << '\n' << std::endl;
    
    {
        for (int num_of_threads = 2; num_of_threads <= 8; num_of_threads *= 2) {
            for (int load = 10; load <= 20; load += 5) {
                double start_time = omp_get_wtime();
                long long fac = 1;
                #pragma omp parallel for
                for (long long i = 2; i <= load; i++) {
                    #pragma omp atomic
                    fac *= i;
                }
                double end_time = omp_get_wtime();
                std::cout << "thread amount: " << num_of_threads << " load: " << load << " time: " << (end_time - start_time) << " Factorial found: " << fac << std::endl;
            }
        }
    }
    
    std::cout << '\n' << "factorial with atomic complete" << '\n' << std::endl;
    
    {
        for (int num_of_threads = 2; num_of_threads <= 8; num_of_threads *= 2) {
            for (int steps = 1000000; steps <= 16000000; steps *= 4) {
                double start_time = omp_get_wtime();
                double sum = 0;
                #pragma omp parallel for reduction(+:sum)
                for (long long i = 0; i <= steps; i++) {
                    double x = (i + 0.5) / steps;
                    sum += 4.0 / (1.0 + x * x);
                }
                double pi = sum / steps;
                double end_time = omp_get_wtime();
                std::cout << "thread amount: " << num_of_threads << " load: " << steps << " time: " << (end_time - start_time) << " Pi found: " << pi << std::endl;
            }
        }
    }
    
    std::cout << '\n' << "Pi with reduction complete" << '\n' << std::endl;
    
    {
        for (int num_of_threads = 2; num_of_threads <= 8; num_of_threads *= 2) {
            for (int steps = 1000000; steps <= 16000000; steps *= 4) {
                double start_time = omp_get_wtime();
                double sum = 0;
                #pragma omp parallel for
                for (long long i = 0; i <= steps; i++) {
                    double x = (i + 0.5) / steps;
                    #pragma omp atomic
                    sum += 4.0 / (1.0 + x * x);
                }
                double pi = sum / steps;
                double end_time = omp_get_wtime();
                std::cout << "thread amount: " << num_of_threads << " load: " << steps << " time: " << (end_time - start_time) << " Pi found: " << pi << std::endl;
            }
        }
    }
    
    std::cout << '\n' << "Pi with atomic complete" << '\n' << std::endl;
    
    {
        for (int num_of_threads = 2; num_of_threads <= 8; num_of_threads *= 2) {
            for (int load = 100000; load <= 1600000; load *= 4) {
                int* a = new int[load];
                int* b = new int[load];
                for (int i = 0; i < load; i++) {
                    b[i] = std::rand();
                    a[i] = std::rand();
                }
                double start_time = omp_get_wtime();
                long long sum = 0;
                #pragma omp parallel for reduction(+:sum)
                for (long long i = 0; i < load; i++) {
                    sum += a[i] * b[i];
                }
                double end_time = omp_get_wtime();
                std::cout << "thread amount: " << num_of_threads << " load: " << load << " time: " << (end_time - start_time) << " scalar product found: " << sum << std::endl;
            }
        }
    }

    std::cout << '\n' << "scalar product with reduction complete" << '\n' << std::endl;

    srand(time(0));
    
    {
        for (int num_of_threads = 2; num_of_threads <= 8; num_of_threads *= 2) {
            for (int load = 100000; load <= 1600000; load *= 4) {
                int* a = new int[load];
                int* b = new int[load];
                for (int i = 0; i < load; i++) {
                    b[i] = std::rand();
                    a[i] = std::rand();
                }
                double start_time = omp_get_wtime();
                long long sum = 0;
                #pragma omp parallel for
                for (long long i = 0; i < load; i++) {
                    #pragma omp atomic
                    sum += a[i] * b[i];
                }
                double end_time = omp_get_wtime();
                std::cout << "thread amount: " << num_of_threads << " load: " << load << " time: " << (end_time - start_time) << " scalar product found: " << sum << std::endl;
            }
        }
    }

    std::cout << '\n' << "scalar product with atomic complete" << '\n' << std::endl;
    
    return 0;
}