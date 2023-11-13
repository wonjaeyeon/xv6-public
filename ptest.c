#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

unsigned long lcg_rand(unsigned long *seed) {
    // LCG parameters
    unsigned long m = 2147483648;
    unsigned long a = 1103515245;
    unsigned long c = 12345;

    *seed = (a * (*seed) + c) % m;
    return *seed;
}

double random_double(unsigned long *seed) {
    // Generate a random integer
    unsigned long rand_int = lcg_rand(seed);

    // Scale it to the range 0 to 1
    double fraction = ((double)rand_int / 2147483648);

    // Scale and shift to the range 70000 to 90000
    return 70000 + fraction * 40000;
}


int main(int argc, char *argv[])
{
    int k, n, id;
    double x=0, z;

    if(argc < 2)
        n = 1;  // default value
    else
        n = atoi(argv[1]);  // from user input
    if(n<0 || n>100)
        n = 2;

    x = 0;
    id = 0;
    for(k=0; k<n; k++)
    {
        id = fork();
        if(id < 0)
            printf(1, "%d failed in fork!\n", getpid());
        else if(id > 0)
        {   // Parent
            printf(1, "Parent %d creating child %d\n", getpid(), id);
            wait();
        }
        else
        {   // Child
            printf(1, "Child %d created\n", getpid());
            unsigned long seed = getpid(); // Using PID as seed for randomness
            double random_limit = random_double(&seed);
            for(z=0;z<random_limit;z+=0.001)
                x = x + 3.14*69.69; // Useless calculations to consume CPU time
            break;
        }
    }
    exit();
}
