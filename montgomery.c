/**
 * @file montgomery.c
 * @author Masahiko Hyuga <mail@mhyuga.jp>
 */

#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

// Defining N as a variable since compiler optimize the modulo operation into multiplication.
//#define N  (987654321UL)
uint64_t N = 987654321UL;
// NN := \tilde{N}
#define NN (974725039UL)
#define R_LOG (31)
#define R     ((1UL<<R_LOG))
#define RM1   (R - 1)
//#define S ((R * R) % N)
#define S  (591964051UL)

double microtime(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + 1e-6 * tv.tv_usec;
}

uint64_t modpow_normal(const uint64_t a, const uint64_t b) {
	uint64_t z = 1;
	for(uint64_t i=0; i<b; i++) {
		z = (z * a) % N;
	}
	return z;
}

uint64_t mont_reduction(const uint64_t t) {
	const uint64_t b = ((t & RM1) * NN) & RM1;
	const uint64_t k = (t + b * N) >> R_LOG;
	return (k < N ? k : k - N);
}

#define mont_mul(a, b) (mont_reduction((a) * (b)))

uint64_t modpow_mont(const uint64_t a, const uint64_t b) {
	uint64_t aa = mont_mul(a, S);
	uint64_t z = R;
	for(uint64_t i=0; i<b; i++) {
		z = mont_mul(z, aa);
	}
	return mont_mul(z, 1);
}

int main() {
	
	const uint64_t a = 114514;
	const uint64_t b = 100 * 1000 * 1000;
	
	printf("I: parameter: %ld^%ld %% %ld\n\n", a, b, N);
	
	//
	// Normal computation.
	printf("I: computing normally...");
	const double normal_begin = microtime();
	uint64_t normal_result = modpow_normal(a, b);
	const double normal_end = microtime();
	printf("done.\n");
	printf("Time: %7.2f ms, result = %ld\n\n", 1000*(normal_end-normal_begin), normal_result);
	
	//
	// Montgomery computation.
	printf("I: computing using Montgomery...");
	const double mont_begin = microtime();
	uint64_t mont_result = modpow_mont(a, b);
	const double mont_end = microtime();
	printf("done.\n");
	printf("Time: %7.2f ms, result = %ld\n\n", 1000*(mont_end-mont_begin), mont_result);
	
	if(normal_result != mont_result) {
		printf(
			"####################################################################################################\n"
			"# DIFFERENT RESULT DETECTED! PLEASE CHECK THE CODE IMMEDIATELY!!!                                  #\n"
			"####################################################################################################\n"
		);
		return 1;
	}
	
	return 0;
	
}

