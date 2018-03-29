#include <iostream>
#include <cmath>
#include <omp.h>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
using namespace std;

const int EXP = 9;  // the expotial of 2,which can be changed

int getMax() {
    int res = 1;
    for (int i = 0; i < EXP; ++i) {
        res *= 2;
    }
    // cout << "max: " << res * 1000 << endl;
    return res * 1000;
}

int sieve() {
    int n = getMax();
    bool * prime = new bool[n + 1];
    memset(prime, true, n + 1);
    int threadNum = ceil (sqrt(n) / 2);
    int count = 0;
    int i;
    #pragma omp parallel for num_threads(threadNum)
    for (i = 2; i <= (int) sqrt(n); ++i) {
        if (prime[i]) {
            for (int j = 2; i * j <= n; j++) {
                prime[i * j] = false;
            }               
        }
    }

    #pragma omp parallel for reduction(+:count)
    for (int i = 2; i <= (int) n; ++i) {
        if (prime[i]) {
            // cout << i << endl;
            count++;
        }
    }
    delete []prime;
    return count;
}

int serial_sieve() {
    int n = getMax();
    bool * prime = new bool[n + 1];
    memset(prime, true, n + 1);
    // int threadNum = ceil (sqrt(n) / 2);
    int count = 0;
    int i;
    // #pragma omp parallel for 
    for (i = 2; i <= sqrt(n); ++i) {
        if (prime[i]) {
            for (int j = 2; i * j <= n; j++) {
                prime[i * j] = false;
            }               
        }
    }

    // #pragma omp parallel for reduction(+:count)
    for (int i = 2; i <= n; ++i) {
        if (prime[i]) {
            // cout << i << endl;
            count++;
        }
    }
    delete []prime;
    return count;
}

int main() {
    clock_t startTime, endTime;
    startTime = clock();
    int count = sieve();
    endTime = clock();
    cout << "Totally " << count << " primes\n";
    cout << "Total time: " << (endTime - startTime) << endl;

    // serialize
    startTime = clock();
    count = serial_sieve();
    endTime = clock();
    cout << "Totally " << count << " primes\n";
    cout << "Total time: " << (endTime - startTime) << endl;
    return 0;
}