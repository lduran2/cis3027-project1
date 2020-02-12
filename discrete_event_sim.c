/*
 * by: Leomar Duran <https://github.com/lduran2>
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/* these deal with actions */
typedef enum Action {
	ARRIVES,
	ENTERS_CPU, ENTERS_DISK1, ENTERS_DISK2, ENTERS_NETWORK,
	FINISH_CPU, FINISH_DISK1, FINISH_DISK2, FINISH_NETWORK,
	TERMINATES
} Action;

char *action_names[] = {
	"arrives",
	"enters the CPU", "enters Disk #1", "enters Disk #2",
	"enters the Network",
	"finishes in the CPU", "finishes in Disk #1",
	"finishes in Disk #2", "finishes in the Network",
	"terminates"
};

/* the process alias */
typedef int Process;

/* the event structure */
typedef struct Event {
	int time;
	Process process;
	Action action;
} Event;

typedef struct PQNode {
	Event *data;
	struct PQNode *next;
	struct PQNode *prev;
} PQNode;

typedef struct PrtyQu {
	PQNode* pre_head;
} PrtyQu;

Event* evnew(int time, Process process, Action action) {
	Event* event = (Event*)malloc(sizeof(Event));
	event->time = time;
	event->process = process;
	event->action = action;
	return event;
}

PQNode* pqnnew(Event* data)
{
	PQNode *node = (PQNode*)malloc(sizeof(PQNode));
	node->data = data;
	node->next = NULL;
	node->prev = NULL;
	return node;
}

PrtyQu* prqnew() {
	PQNode* pqnnew(Event*);

	PrtyQu *queue = (PrtyQu*)malloc(sizeof(PrtyQu));
	PQNode *pre_head = pqnnew(NULL);
	pre_head->next = pre_head;
	pre_head->prev = pre_head;
	queue->pre_head = pre_head;
	return queue;
}

void prqins(PrtyQu* queue, PQNode* new_element)
{
	PQNode* position = queue->pre_head;
	// printf("%d\t%d\t%d\n", queue, queue->pre_head, queue->pre_head->next);
	for (;
		(position->next != queue->pre_head)
			&& (position->next->data->time <= new_element->data->time);
		position = position->next)
	{
		// nop
	}
	new_element->prev = position;
	new_element->next = position->next;
	new_element->next->prev = new_element;
	position->next = new_element;

}

int pq2str(char **pstr, PrtyQu queue)
{
	int ev2str(char**, Event);

	int size = 0;
	int sum = 0;
	char *line = NULL;
	char *new_str;

	PQNode* position;
	for (position = queue.pre_head->next;
		/* the pre-head is always empty, so skip it */
		(position != queue.pre_head);
		(position = position->next)

	) {
		ev2str(&line, *(position->data));

		size = snprintf(NULL, 0, "%s%s\n", *pstr, line);
		new_str = (char*)malloc((size + 1)*sizeof(char));
		sprintf(new_str, "%s%s\n", *pstr, line);
		*pstr = new_str;
		sum += size;
	}

	return sum;
}

/* these deal with constant configuration */
char *const_keys[] = {
	"SEED", "INIT_TIME", "FIN_TIME", "ARRIVE_MIN", "ARRIVE_MAX",	/* 0:4 */ 
	"CPU_MIN", "CPU_MAX", "DISK1_MIN", "DISK1_MAX",	/* 5:8 */
	"DISK2_MIN", "DISK2_MAX", "NETWORK_MIN", "NETWORK_MAX",	/* 9:12 */
	"QUIT_PROB", "NETWORK_PROB" /* 13:14 */
};
typedef enum IntConstantIndex {
	I_SEED, I_INIT_TIME, I_FIN_TIME, I_ARRIVE_MIN, I_ARRIVE_MAX,
	I_CPU_MIN, I_CPU_MAX, I_DISK1_MIN, I_DISK1_MAX,
	I_DISK2_MIN, I_DISK2_MAX, I_NETWORK_MIN, I_NETWORK_MAX
} IntConstantIndex;
typedef enum DblConstIndex {
	I_QUIT_PROB, I_NETWORK_PROB
} DblConstIndex;
// NETWORK_MAX is the last int constant,
// I_NETWORK_PROB is the last double constant.
// +2 because there are 2 arrays.
const size_t N_CONST_KEYS = (I_NETWORK_MAX + I_NETWORK_PROB + 2);

int
main(int argc, char **argv)
{
	//void prqins(PrtyQu, PQNode*);
	int pq2str(char**, PrtyQu);
	bool rdcfgk(FILE*, int[], double[]);

	const Process SIMULATION = -1;

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
	rdcfgk(config_file, int_const_values, dbl_const_values);
	fclose(config_file);

	SEED = int_const_values[I_SEED];
	INIT_TIME = int_const_values[I_INIT_TIME];
	FIN_TIME = int_const_values[I_FIN_TIME];
	ARRIVE_MIN = int_const_values[I_ARRIVE_MIN];
	ARRIVE_MAX = int_const_values[I_ARRIVE_MAX];
	CPU_MIN = int_const_values[I_CPU_MIN];
	CPU_MAX = int_const_values[I_CPU_MAX];
	DISK1_MIN = int_const_values[I_DISK1_MIN];
	DISK1_MAX = int_const_values[I_DISK1_MAX];
	DISK2_MIN = int_const_values[I_DISK2_MIN];
	DISK2_MAX = int_const_values[I_DISK2_MAX];
	NETWORK_MIN = int_const_values[I_NETWORK_MIN];
	NETWORK_MAX = int_const_values[I_NETWORK_MAX];
	QUIT_PROB = dbl_const_values[I_QUIT_PROB];
	NETWORK_PROB = dbl_const_values[I_NETWORK_PROB];

	PrtyQu* queue = prqnew();
	prqins(queue, pqnnew(evnew(50, 1, ARRIVES)));
	prqins(queue, pqnnew(evnew(200, 2, ARRIVES)));
	prqins(queue, pqnnew(evnew(25, 3, ARRIVES)));
	prqins(queue, pqnnew(evnew(250, 4, ARRIVES)));
	prqins(queue, pqnnew(evnew(50, 5, ARRIVES)));
	prqins(queue, pqnnew(evnew(10, 6, ARRIVES)));
	prqins(queue, pqnnew(evnew(275, -1, ARRIVES)));

	char* queue_string = "";
	pq2str(&queue_string, *queue);
	printf("%s", queue_string);

	return 0;
}

bool
rdcfgk(FILE *config_file, int int_const_values[], double dbl_const_values[])
{
	int strfci(char, const size_t, char*);
	int arrstr(const size_t, char*, const size_t, char**);

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
		else if (i_const_key <= I_NETWORK_MAX) {
			int_const_values[i_const_key] =
				strtol(str_const_value, &string_remainder, 10);
		}
		else if (i_const_key < N_CONST_KEYS) {
			dbl_const_values[i_const_key - (I_NETWORK_MAX + 1)] =
				strtod(str_const_value, &string_remainder);
		}
		else {
			/* nop */
		}
	}
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

int
ev2str(char **pstr, Event event)
{
	char* JOB_FORMAT = "Job #%3d";
	char* EVENT_FORMAT = "t = %10d: %3s %s.";

	char* JOB_NAME = NULL;
	Process process_id = event.process;
	size_t size;

	switch (process_id) {
		case -1: JOB_NAME = "simulation"; break;
		default:
			size = snprintf(NULL, 0, JOB_FORMAT, process_id);
			JOB_NAME = (char*)malloc((size + 1)*sizeof(char));
			sprintf(JOB_NAME, JOB_FORMAT, process_id);
		break;
	}

	size = snprintf(NULL, 0, EVENT_FORMAT,
		event.time, JOB_NAME, action_names[event.action]);
	*pstr = (char*)malloc((size + 1)*sizeof(char));
	sprintf(*pstr, EVENT_FORMAT, event.time, JOB_NAME, action_names[event.action]);
	return size;
}

