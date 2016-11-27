#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#define e 2,71828182845904523536
#define	DELTA 				(exp(5))
#define	EPSILON 			(exp(1)*pow(10, -4))	
#define	NUM_HASH			5						// = log(1/DELTA)
#define SIZE_HASH 			10000					// = e/EPSILON, space for hashing 
#define P_HASH				123457					// const int hash functions

/*	--- test tiny data ----
#define WORDS_FILE 	"words_stream_tiny.txt"
#define COUNTS_FILE	"counts_tiny.txt"
#define MAX_VALUE		139039
#define ALL_STREAM		
*/

#define WORDS_FILE 			"words_stream.txt"		// word_file 
#define COUNTS_FILE 		"counts.txt"			// count_file
#define MAX_VALUE			1407593					// sum of items 
			

int read_hash_params(char *);
int compute_matrix();
void init_matrix();
int hash(int, int, int);
int plotting();

int a[NUM_HASH], b[NUM_HASH];						// parameters of hash functions
int matrix[NUM_HASH][SIZE_HASH];					// matrix store c[j][hash(i)]
long all_stream = 0;									// sum of elements in stream

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



/* plotting graph */
int plotting(){
	FILE *f_count;
	FILE *gnuplot = popen("gnuplot", "w");
	
	if( ! (f_count = fopen(COUNTS_FILE, "r"))){
		printf("cannot open counts.txt\n");
		return 1;
	}

	//fprintf(gnuplot, "set terminal png\n");
	fprintf(gnuplot, "set terminal png\n");
	fprintf(gnuplot, "set output 'graph.png'\n");
	fprintf(gnuplot, "plot '-' using 1:2 title 'datastream'\n");

	int i, j;
	int count;
	int word;
	int values[NUM_HASH]; 	// list value of item in matrix, with hash_function
	int hash_code[NUM_HASH];
	long all = 0;

	/*
		with item 'i', compute all hash_j(i)
		compute min {c[j][hash_j[i]]} with each j
		plot with error, and frequency of item 'i'
	*/
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
		all += count;
		fprintf(gnuplot, "%f %f\n", log10(count*1.0/all_stream), log10((min - count)*1.0/count) );
		//fprintf(gnuplot, "%f %f\n", count, min);
	}

	printf("all = %ld, all_stream = %ld", all, all_stream);
	fclose(f_count);
	fputs("e\n", gnuplot);
	
	pclose(gnuplot);
	
	return 0;
	
}


/* 
	read file word_stream.txt 
	compute matrix
*/
int compute_matrix(){
	FILE *f;
	if( !(f = fopen(WORDS_FILE, "r")) ){
		printf("cannot open words_stream.txt\n");
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
		all_stream++;
	}

	fclose(f);
	return 0;
}

/* set matrix = 0 */
void init_matrix(){
	int i, j;

	for(i = 0; i < NUM_HASH; i++){
		for(j = 0; j < SIZE_HASH; j++){
			matrix[i][j] = 0;
		}
	}
}

/* hash function */
int hash(int a, int b, int x){
	int y;
	int hash_val;

	y = x % P_HASH;
	hash_val = (a*y + b) % P_HASH;
	return (hash_val%SIZE_HASH);
}


/* get hash parameters, store in a[], b[] */
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
