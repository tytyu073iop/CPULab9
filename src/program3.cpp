#include <iostream>
#include <omp.h>

int main() {
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < omp_get_num_threads() * 3; ++i) {
        #pragma omp critical
        {
            std::cout << "Thread " << omp_get_thread_num() << " is processing iteration " << i << std::endl;
        }
    }

    std::cout << '\n' << "Static scheduling completed." << '\n' << std::endl;

    #pragma omp parallel for schedule(dynamic)
    for (int i = 0; i < omp_get_num_threads() * 3; ++i) {
        #pragma omp critical
        {
            std::cout << "Thread " << omp_get_thread_num() << " is processing iteration " << i << std::endl;
        }
    }

    std::cout << '\n' << "Dynamic scheduling completed." << '\n' << std::endl;

    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < omp_get_num_threads() * 3; ++i) {
        #pragma omp critical
        {
            std::cout << "Thread " << omp_get_thread_num() << " is processing iteration " << i << std::endl;
        }
    }

    std::cout << '\n' << "Guided scheduling completed." << '\n' << std::endl;

    return 0;
}