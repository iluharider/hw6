#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {

	if (argc < 2) {
		fprintf(2, "too few arguments\n");
		exit(1);
	}

	int p1[2];
	int p2[2];

	pipe(p1);
	pipe(p2);

	int pid = fork();

	if (pid == 0) { 
		close(p2[0]);
		close(p1[1]);

		char byte_buffer;
		while (read(p1[0], &byte_buffer, 1) > 0) {
			fprintf(1, "%d: received %c\n", getpid(), byte_buffer);
			write(p2[1], &byte_buffer, 1);
		}

		close(p2[1]);
		close(p1[0]);
		exit(0);
	}
	else if (pid > 0) { 
		close(p1[0]);
		close(p2[1]);

		for (int i = 0; argv[1][i] != 0; i++) {
			write(p1[1], argv[1] + i, 1);
		}

		close(p1[1]);
		wait(0);

		char byte_buffer_1;
		while (read(p2[0], &byte_buffer_1, 1) > 0) {
			fprintf(1, "%d: received %c\n", getpid(), byte_buffer_1);
		}

		close(p2[0]);
		exit(0);
	}
	else {
		fprintf(2, "forking error\n");
		exit(1);
	}
}