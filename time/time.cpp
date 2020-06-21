#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

double
getTime(void)
{
    static double startTime = 0.0;
    static bool first = true;

    struct timeval t;
    double tm;

    gettimeofday(&t, 0);
    tm = (double)(t.tv_sec) + (double)(t.tv_usec) / 1000000.0;

    if (first) {
      first = false;
      startTime = tm;
    }
    //printf("sec = %d, usec = %d (%f) [%f]\n", t.tv_sec, t.tv_usec, tm, startTime);

    return (tm - startTime);
}

int main(void)
{
	struct timeval t;
	double tm;

	tm = getTime();
	clock_t begin = clock();
	sleep(1);
	clock_t end = clock();
	tm = getTime() - tm;

	printf("time = %ld\n", (end - begin));
	printf("time = %f\n", (double)(end - begin) / CLOCKS_PER_SEC);
	printf("CLOCKS_PER_SEC = %ld\n", CLOCKS_PER_SEC);

	printf("dtime = %f\n", tm);
}
