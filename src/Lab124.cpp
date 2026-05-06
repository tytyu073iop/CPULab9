#include <thread>
#include <iostream>
#include <sstream>
#include <mutex>
#include <vector>

struct pcout : public std::stringstream {
    static inline std::mutex cout_mutex;
    ~pcout() {
        std::lock_guard<std::mutex> l(cout_mutex);
        std::cout << this->rdbuf();   // rdbuf() is a member of stringstream
        std::cout.flush();
    }
};



void thread(int id) {
    std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % 1000));
    pcout {} << "thread: " << id << " is done!" << '\n';
}

int main() {
    srand(time(0));

    const int n = 5;
    std::vector<std::thread> v(n);
    
    for (int i = 0; i < 5; i++) {
        v[i] = std::thread(thread, i);
    } 
    for (int i = 0; i < 5; i++) {
        v[i].join();
    }

    return 0;
}