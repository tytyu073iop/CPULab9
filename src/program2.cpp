#include <iostream>
#include <omp.h>
#include <cmath>

int main() {
    #pragma omp parallel for
    for (int i = 0; i < 20; ++i) {
        #pragma omp critical
        {
            std::cout << "Thread " << omp_get_thread_num() << " is processing iteration " << i << std::endl;
        }
    }

    std::cout << '\n' << "Static scheduling completed." << '\n' << std::endl;

    for (int num_of_threads = 2; num_of_threads <= 8; num_of_threads *= 2) {
        omp_set_num_threads(num_of_threads);
        for (int size = 1000000; size <= 16000000; size *= 4) {
            double start = omp_get_wtime();
            double *sinTable = new double[size];
            #pragma omp parallel for
            for (int i = 0; i < size; ++i) {
            sinTable[i] = std::sin(2 * M_PI * i / size);
            }
            delete[] sinTable;
            double end = omp_get_wtime();
            std::cout << "Time taken for size " << size << ": " << (end - start) << " seconds." << " with " << num_of_threads << " threads." << std::endl;
        }
    }

    return 0;
}