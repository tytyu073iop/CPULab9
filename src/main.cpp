#include <iostream>
#include <vector>
#include <omp.h>

int main() {
    std::cout << "OpenMP Version: " << _OPENMP << std::endl;

    #pragma omp parallel
    {
        #pragma omp critical
        {
            std::cout << "Thread " << omp_get_thread_num() << " of " << omp_get_num_threads() << std::endl;
        }
    }

    int n = 100;
    std::vector<int> data(n);
    for (int i = 0; i < n; ++i) data[i] = i;

    int sum = 0;
    #pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; ++i) {
        sum += data[i];
    }

    std::cout << "Sum: " << sum << std::endl;
    return 0;
}