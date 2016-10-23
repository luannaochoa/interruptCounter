#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <time.h>

int get_proc_interrupts_info () {
	FILE* fp;
	char buffer[1024]; 
	size_t bytes_read;
	char* match;
	int keyboard_interrupts;

	fp = fopen("/proc/interrupts", "r");
	bytes_read = fread (buffer, 1, sizeof (buffer), fp);
	fclose(fp);

	match = strstr(buffer, "1: ");

	sscanf(match,"1:\t %d", &keyboard_interrupts);
	return keyboard_interrupts;
}


int main (int argc, char *argv[]) {

	FILE* write_to_WaveFile = fopen(argv[1], "w");
	int user_provided_program_run_time = argv[2];
	time_t current_time = time(NULL);
	time_t previous_time = time(NULL);
	int counter = 0; 
	int systemCall1= 0;
	int systemCall2 =0;
	pid_t shell_pid = getpid();

	do {

		if (shell_pid == getpid()) {

			current_time = time(NULL);
			if (current_time>previous_time) {

				counter++;
				systemCall1 = get_proc_interrupts_info();

				previous_time = current_time;

				if (systemCall1 == systemCall2){

					fprintf(write_to_WaveFile,"0");

				} else if (systemCall1 != systemCall2) {

					fprintf(write_to_WaveFile, "1");
				}
				systemCall2 = systemCall1;
			} 
		}

		
	} while (counter < user_provided_program_run_time);
		
	fclose(write_to_WaveFile);
	return 0;

}