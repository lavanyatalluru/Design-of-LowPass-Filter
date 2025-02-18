// LowPassFilter.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZ 10000
#define fs 20000 // Sampling Frequency
#define f 9000  // Frequency of the signal
#define no_filter_coeff 50
#define no_bits 1000
#define filter_memory_length no_filter_coeff

void read_impulse_coeff(double *h)
{
	char *next_token, *token, c[1000];
	int j = 0;
	FILE *fp;

	fopen_s(&fp, "impulse_response_coeff.csv", "r");

	if (fp == NULL)
	{
		printf("Cannot open file \n");
		exit(0);
	}

	// Read contents from file 
	fgets(c, 1000, fp);

	token = strtok_s(c, ",", &next_token);
	while (token != NULL)
	{
		h[j] = atof(token);
		token = strtok_s(NULL, ",", &next_token);
		j++;
	}
}


void filter(double *sig_temp, double *filter_memory, double *h, double *result)
{
	int k, j, n, l, i;
	double sum;
	FILE *fp;
	fopen_s(&fp, "Result.csv", "w");


	for (n = 0; n < no_bits; n++)
	{
		sum = 0; j = 0;
		for (i = 0; i < filter_memory_length - 1; i++)
		{
			filter_memory[i] = filter_memory[i + 1];
		}
		filter_memory[filter_memory_length - 1] = sig_temp[n];
		for (k = no_filter_coeff - 1; k >= 0; k--)
		{
			sum = sum + (h[k] * filter_memory[j]);
			j = j + 1;
		}
		//result[n] = sum; //printf("Result %lf\n", result[n]);
		fprintf(fp, "%lf\n", sum);
	}
	fclose(fp);
	printf("Done");
}


int main()
{
	int i, j, n, k, l;
	double sig_temp[BUFSIZ], result[BUFSIZ], h[no_filter_coeff], filter_memory[no_filter_coeff];


	for (n = 0; n < no_filter_coeff; n++)
	{
		filter_memory[n] = 0;
	}

	for (n = 0; n < no_bits; n++)
		sig_temp[n] = sin(2 * 3.14*f / fs * n);

	read_impulse_coeff(&h[0]);

	filter(&sig_temp[0], &filter_memory[0], &h[0], &result[0]);
	printf("\nResult is saved in Result.CSV file");

	getchar();
	return 0;
}

