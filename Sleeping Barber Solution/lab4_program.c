/*
Jack O'Neil
13 November, 2016
Lab 4: Implementing Pipes: The Sleeping Barber
*/

#include <stdio.h>
#include <time.h>

void randomwait();
void barber_run();
void customer_run();

int seats[2];
int customers[2];
int barber[2];
int freeseats[2];

void barber_run() {
	int i, free_seats;
	
	// At most 10 customers will get their hair cut
	for (i = 1; i <= 10; ++i) { 	
		printf("Barber %d is trying to get a customer.\n", i);
		P(customers);
		printf("Barber %d is waiting for the seat to become free.\n", i);
		P(seats);
		read(freeseats[0], &free_seats, sizeof(int));
		free_seats++;
		write(freeseats[1], &free_seats, sizeof(int));
		printf("Barber %d is increasing the number of free seats to %d.\n", i, free_seats);
		V(seats);
		V(barber);
		printf("Barber is now cutting hair %d.\n", i);
		
		// random wait before finishing haircut
		randomwait(1); 			
	}
}

void customer_run() {
	int free_seats;
	printf("- New customer trying to find a seat.\n");
	P(seats);
	read(freeseats[0], &free_seats, sizeof(int));
	
	// Customer has found available seat
	if (free_seats > 0) {
		free_seats--;
		write(freeseats[1], &free_seats, sizeof(int));
		printf("- Customer is decreasing the number of free seats to %d.\n", free_seats);
		V(customers);
		V(seats);
		printf("- Customer is now waiting for the barber.\n");
		P(barber);
		printf("- Customer is now getting a hair cut.\n");
	}
	
	// There were no seats available
	else {
		write(freeseats[1], &free_seats, sizeof(int));
		V(seats);
		printf("\' Customer giving up: No free chairs in waiting room.\n");
	}
}

void randomwait(int d) { // random 0 to d sec delay
	int delay;
	struct timespec tim, tim2;
    tim.tv_sec = 0;
	delay = abs(rand() % 1000000000) * d;
	tim.tv_nsec = delay;
	nanosleep(&tim, &tim2);
}

void V(int pd[]) {
	int a = 1;
	write(pd[1], &a, sizeof(int));
}

void P(int pd[]) {
	int a;
	read(pd[0], &a, sizeof(int));
}

void main() {
	pipe(seats); 				// lock to modify freeseats
	pipe(customers); 			// how many customers are waiting on seats
	pipe(barber); 				// barber ready to cut?  0=no 1=yes	
	
	pipe(freeseats); 			// store the integer value for number of seats
	
	V(seats); 					// release the lock on seats by setting to 1
	
	int num = 3; 				// Free Seats = 3
	write(freeseats[1], &num, sizeof(int));
	
	if (fork() == 0) {  		// Start 1 barber
		srand(time(0) + 11); 	// Randomized seed
		barber_run();
		return;
	}
	int i;
	for (i = 1; i <= 10; i++) { // Start 10 customers randomly
		if (fork() == 0) {
			randomwait(i); 		// random wait before next customer arrives
			srand(time(0) + i); // different seed than barber for different delays
			customer_run();
			return;
		}
	}
}