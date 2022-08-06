#ifndef _APP_C_
#define _APP_C_
#include "app.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <random>
#include <time.h>


void appStart(void)
{
	printf("-=Reading AccountsDB...\n");
	readAccountDB();
	printf("-=Succesfully loaded AccountsDB=-\n\n");
}

//Was used to generate random balance and PAN in AccountsDB.txt
//To run this function correctly you need to:
//-comment out srand line in GenerateLuhn() first
void fill() {
	FILE* ptr;
	ptr = fopen("AccountsDB.txt", "w");
	time_t t1;
	srand((unsigned)time(&t1));
	for (int i = 0; i < 255; i++) {
		float amount = (float)(rand() % (int)1e6) / (float)(1e3 / 100);
		fprintf(ptr, "%.0f %s\n", amount, GenerateLuhn());
	}
}
#endif
