#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>

int main()
{
	double start, stop;
	start = time(NULL);
	sleep(5);
	stop = time(NULL);
	printf("Temps écoulé : %f\n", stop - start);
	return 0;
}
