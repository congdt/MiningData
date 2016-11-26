#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#define e 2,71828182845904523536
#define	DELTA 				(exp(5))
#define	EPSILON 			(exp(1)*pow(10, -4))	
#define	NUM_HASH			5						// = log(1/DELTA)
#define SIZE_HASH 			10000					// = e/EPSILON
#define P_HASH				123457

/*	--- test tiny data ----
#define WORDS_FILE 	"words_stream_tiny.txt"
#define COUNTS_FILE	"counts_tiny.txt"
#define MAX_VALUE		139039
*/

#define WORDS_FILE 			"words_stream.txt"
#define COUNTS_FILE 		"counts.txt"
#define MAX_VALUE			1407593					// from 1 to MAX_VALUE


int read_hash_params(char *);
int compute_matrix();
void init_matrix();
int hash(int, int, int);
int plotting();

int a[NUM_HASH], b[NUM_HASH];
int b1,b2,b3,b4,b5;
int matrix[NUM_HASH][SIZE_HASH];


int main(int argc, char const *argv[])
{

	/* read hash_params */
	if(read_hash_params("hash_params.txt")){
		return 1;
	}
	
	/* init matrix */
	init_matrix();

	/* compute_matrix */
	compute_matrix();

	plotting();

	return 0;
}


int plotting(){
	FILE *f_count;
	FILE *gnuplot = popen("gnuplot", "w");
	
	if( ! (f_count = fopen(COUNTS_FILE, "r"))){
		printf("cannot open counts.txt\n");
		return 1;
	}

	//fprintf(gnuplot, "set terminal png\n");
	fprintf(gnuplot, "plot '-' u 1:2\n");

	int i, j;
	int count;
	int word;
	int values[NUM_HASH]; 	// list value of item in matrix, with hash_function
	int hash_code[NUM_HASH];
	

	for(i = 0; i < MAX_VALUE; i++ ){
		for(j = 0; j< NUM_HASH; j++){
			hash_code[j] = hash(a[j], b[j], i);
			values[j] = matrix[j][hash_code[j]];
		}
		int min = values[0];
		int k;
		for(k = 1; k < NUM_HASH; k++){
			if (values[k] < min){
				min = values[k];
			}
		}

		fscanf(f_count, "%d", &word);
		fscanf(f_count, "%d", &count);
		//printf("word: %g", count*1.0/MAX_VALUE);
		//printf("count: %g\n", (min - count)*1.0/count);

		fprintf(gnuplot, "%f %f\n", count*1.0/MAX_VALUE, (min - count)*1.0/count );
		//fprintf(gnuplot, "%f %f\n", count, min);
	}
	fclose(f_count);
	fputs("e\n", gnuplot);
	
	pclose(gnuplot);
	
	return 0;
	
}


int compute_matrix(){
	FILE *f;
	if( !(f = fopen(WORDS_FILE, "r")) ){
		printf("cannot open words_stream_tiny.txt\n");
		return 1;
	}

	int word;
	int i;
	int h;

	while(fscanf(f, "%d", &word) != EOF){
		for(i = 0; i < NUM_HASH; i++){
			h = hash(a[i], b[i], word );
			matrix[i][h]++;
		}
	}

	fclose(f);
	return 0;
}

void init_matrix(){
	int i, j;

	for(i = 0; i < NUM_HASH; i++){
		for(j = 0; j < SIZE_HASH; j++){
			matrix[i][j] = 0;
		}
	}
}

int hash(int a, int b, int x){
	int y;
	int hash_val;

	y = x % P_HASH;
	hash_val = (a*y + b) % P_HASH;
	return (hash_val%SIZE_HASH);
}

int read_hash_params(char *file){
	int i;
	FILE *f;
	if( !(f = fopen(file, "r"))){
		printf("cannot open hash_params file\n");
		return 1;
	}
	for (i = 0; i < NUM_HASH; i++){
		fscanf(f, "%d", &a[i]);
		fscanf(f, "%d", &b[i]);
	}

	fclose(f);
	return 0;
}