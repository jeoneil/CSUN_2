/*
COMP 222
19 May, 2016
Project 3
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* declare global vars */
int max_length;
int parity;


/* define hamming string as a dynamic array of characters */ 
char *ham_string;

/***************************************************************/
void enterParameters()
{
	printf("Enter the maximum length: ");
	scanf("%d",&max_length);
	printf("Enter the parity (0=even, 1=odd): ");
	scanf("%d",&parity);
	ham_string = (char*)malloc(max_length * sizeof(char));
	
	if (parity == 0) parity = 1;
	else parity = 0;
}

/***************************************************************/

void checkHam()
{
	/* declare local vars */
	int ham_parity = 0;
	int error_bit = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	int ham_length = 0;
	int num_parity_bits = 0;
	int start = 0;
	int increment = 0;

	/* prompt for hamming code as a "string"*/
	printf("Enter the hamming code: ");
	scanf("%s", ham_string);
	
	/* Determine actual Hamming code length & number of parity bits */
	ham_length = strlen(ham_string);
	num_parity_bits = ceil(log(ham_length)/log(2));
	
	/* OUTER LOOP: for each parity bit in the Hamming code*/
	for (i = 0; i < num_parity_bits; i++)
	{ /* BEGIN OF OUTER LOOP */
		
		/* initialize appropriate local variables */
		start = pow(2,i);
		increment = 2 * start;
		ham_parity = parity;
		
		/* MIDDLE LOOP: for each starting bit of a sequence */
		for (j = start; j < ham_length; j = j + increment)
		{
			/* INNER LOOP: for each bit to be checked, check value and update parity information for current parity bit*/
			for (k = j; k < start + increment && k < ham_length; k++)
			{
				/* update parity checking by including result for parity bit checked in total */
				ham_parity ^= (ham_string[ham_length - k] - '0');
			}
		}
		error_bit += ham_parity * start;
	} /* END OF OUTER LOOP

	/* report error in hamming code based on result from parity bits or report no error if none */
	if (error_bit == 0) printf("There is no bit error!\n");
	else
	{
		printf("There is an error in bit: %d\n", error_bit);
		if (ham_string[ham_length - error_bit] == '0')
			ham_string[ham_length - error_bit] = '1';
		else ham_string[ham_length - error_bit] = '0';
		
		/* correct hamming code by flipping error bit (if necessary)*/
		printf("The corrected Hamming code is: %s\n", ham_string);
	}
	return;
}

/***************************************************************/
int main()
{
	int exit = 0; 
	int select = 0;
	
	while (exit == 0)
	{
		printf("\nError detection/correction:\n-------------\n1) Enter parameters\n2) Check hamming code\n3) Quit\n\nEnter selection: ");
		scanf("%d", &select);
		switch (select)
		{
			case 1: enterParameters();
					break;
			case 2: checkHam();
					break;
			case 3: exit = 1;
					break;
			default: break;
		}
	}
	return 1;
}