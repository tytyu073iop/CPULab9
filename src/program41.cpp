#include <iostream>
#include <vector>
#include <omp.h>

int main() {
    {
        int counter = 0;
        int value = 2;
        int n = 10000;
        
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            #pragma omp atomic
            counter += value;
        }
        
        std::cout << "Final counter value: " << counter << std::endl;
    }

    {
        int counter = 0;
        int value = 2;
        int n = 10000;
        
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            counter += value;
        }
        
        std::cout << "Final counter value: " << counter << std::endl;
    }
    
    return 0;
}