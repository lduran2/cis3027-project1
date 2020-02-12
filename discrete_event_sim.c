/*
 * by: Leomar Duran <https://github.com/lduran2>
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int
main(int argc, char* *argv)
{
	int strfci(char, const size_t, char*);
	int arrstr(const size_t, char*, const size_t, char**);

	typedef struct Process {
		int id;
	} Process;

	long int SEED;

	/* the stating time of the simulator */
	long int INIT_TIME = 0;
	/* the ending time of the simulator */
	long int FIN_TIME = 0;

	/* the minimum interarrival time */
	long int ARRIVE_MIN = 0;
	/* the maximum interarrival time */
	long int ARRIVE_MAX = 0;

	/* the minimum time the CPU will service a process */
	long int CPU_MIN = 0;
	/* the maximum time the CPU will service a process */
	long int CPU_MAX = 0;
	/* the minimum time disk 1 will service a process */
	long int DISK1_MIN = 0;
	/* the maximum time disk 1 will service a process */
	long int DISK1_MAX = 0;
	/* the minimum time disk 2 will service a process */
	long int DISK2_MIN = 0;
	/* the maximum time disk 2 will service a process */
	long int DISK2_MAX = 0;
	/* the minimum time the network will service a process */
	long int NETWORK_MIN = 0;
	/* the maximum time the network will service a process */
	long int NETWORK_MAX = 0;

	/* the probability of the current job quitting the system */
	double QUIT_PROB = 0;
	/* the probability that the current job will access the network */
	double NETWORK_PROB = 0;

	const size_t N_CONST_KEYS = 15;
	char *const_keys[] = {
		"SEED", "INIT_TIME", "FIN_TIME", "ARRIVE_MIN", "ARRIVE_MAX",	/* 0:4 */ 
		"CPU_MIN", "CPU_MAX", "DISK1_MIN", "DISK1_MAX",	/* 5:8 */
		"DISK2_MIN", "DISK2_MAX", "NETWORK_MIN", "NETWORK_MAX",	/* 9:12 */
		"QUIT_PROB", "NETWORK_PROB" /* 13:14 */
	};
	int int_const_values[] = {
		0, 0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	double dbl_const_values[] = {
		0.0, 0.0
	};

	/* logical time of simulation */
	int t;

	FILE *config_file = fopen("sim.config", "r");
	char *line = NULL;
	size_t line_len = 0;
	int i_space;
	int i_const_key;
	char *str_const_value;
	char *string_remainder;

	if (NULL == config_file) {
		exit(1);
	}
	while (-1 != getline(&line, &line_len, config_file)) {
		i_space = strfci(' ', line_len, line);
		if (i_space < 0) {
			continue;
		}
		i_const_key = arrstr(i_space, line, N_CONST_KEYS, const_keys);
		str_const_value = (line + i_space + 1);
		if (i_const_key < 0) {
			continue;
		}
		else if (i_const_key < 13) {
			int_const_values[i_const_key] =
				strtol(str_const_value, &string_remainder, 10);
		}
		else if (i_const_key < N_CONST_KEYS) {
			dbl_const_values[i_const_key - 13] =
				strtod(str_const_value, &string_remainder);
		}
		else {
			/* nop */
		}
	}


	SEED = int_const_values[0];
	INIT_TIME = int_const_values[1];
	FIN_TIME = int_const_values[2];
	ARRIVE_MIN = int_const_values[3];
	ARRIVE_MAX = int_const_values[4];
	CPU_MIN = int_const_values[5];
	CPU_MAX = int_const_values[6];
	DISK1_MIN = int_const_values[7];
	DISK1_MAX = int_const_values[8];
	DISK2_MIN = int_const_values[9];
	DISK2_MAX = int_const_values[10];
	NETWORK_MIN = int_const_values[11];
	NETWORK_MAX = int_const_values[12];
	QUIT_PROB = dbl_const_values[0];
	NETWORK_PROB = dbl_const_values[1];

	return 0;
}

int
strfci(char needle,
	const size_t HAYSTACK_LEN, char *haystack)
{
	int k;

	for (k = 0; (HAYSTACK_LEN != k); ++k) {
		if (haystack[k] == needle) {
			return k;
		}
	}

	return -1;
}

int
arrstr(const size_t NEEDLE_LEN, char *needle,
	const size_t HAYSTACK_LEN, char **haystack)
{
	int k;

	for (k = HAYSTACK_LEN; (0 != (k--)); ) {
		if (0 == strncmp(haystack[k], needle, NEEDLE_LEN)) {
			return k;
		}
	}

	return -1;
}
	
