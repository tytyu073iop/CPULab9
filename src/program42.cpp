#include <iostream>
#include <vector>
#include <omp.h>

int main() {
    srand(time(0));
    for (int num_of_threads = 2; num_of_threads <= 8; num_of_threads *= 2) {
        for(int load = 1000000; load <= 16000000; load *= 4) {
            int* arr = new int[load];
            for (int i = 0; i < load; i++) {
                arr[i] = std::rand();
            }
            double start_time = omp_get_wtime();
            int min = arr[0];
            #pragma omp parallel for num_threads(num_of_threads)
            for (int i = 0; i < load; i++) {
                #pragma omp critical
                {
                    if (arr[i] < min) {
                        min = arr[i];
                    }
                }
            }
            double end_time = omp_get_wtime();
            std::cout << "thread amount: " << num_of_threads << " load: " << load << " time: " << (end_time - start_time) << " Min value found: " << min << std::endl;
            delete[] arr;
        }
    }
    
    return 0;
}