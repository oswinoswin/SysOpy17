#include <stdlib.h>
#include <stdio.h>
#include <sys/times.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int swapInFileSys(unsigned char *, unsigned char *, int, int); 
void sortSys(int, unsigned char *, unsigned char *, int);
int swapInFileStd(unsigned char *, unsigned char *, int, FILE *);
void sortStd(FILE *, unsigned char *, unsigned char *, int);
void printTime(clock_t, clock_t, struct tms *, struct tms *);

static struct tms tmsStart, tmsEnd;

int main (int argc, char *args[]) {
	const char *fileName;
	const char *mode;
	int sysMode;
	unsigned char *buffer1, *buffer2;
	
	
	int	recordSize;
	int intHandle;
	FILE *fileHandle;	
	clock_t clockStart, clockEnd;
	
	if (argc != 4) {
		fprintf(stderr, "usage: <file name> <size of record> <[sys]|[lib]>\n");
		exit(EXIT_FAILURE);
	}
	
	mode = args[3];
	
	if(!strcmp(mode, "lib")){
		sysMode = 0;
	} else if (!strcmp(mode, "sys")){
		sysMode = 1;
	} else {
		fprintf(stderr, "choosing mode error\n");
		exit(EXIT_FAILURE);
	}
	
	fileName = args[1];
	recordSize = atoi(args[2]);
	
	buffer1 = malloc(sizeof(char) * recordSize);
	buffer2 = malloc(sizeof(char) * recordSize);
	
	if(buffer1 == NULL || buffer2 == NULL) {
		fprintf(stderr, "malloc error\n");
		exit(EXIT_FAILURE);
	}
	
	//Start timer
	if((clockStart = times(&tmsStart)) < 0) {
		fprintf(stderr, "times error/n");
	}
	
	if(sysMode){
		
		if ((intHandle = open(fileName, O_RDWR)) == -1) {
			fprintf(stderr, "open error\n");
			exit(EXIT_FAILURE);
		}
		
		sortSys(intHandle, buffer1, buffer2, recordSize);
		
		if (close(intHandle) == -1) {
			fprintf(stderr, "close error\n");
			exit(EXIT_FAILURE);
		}
		
	} else { //if libMode
		
		fileHandle = fopen(fileName, "r+w");
		if (fileHandle == NULL) {
			fprintf(stderr, "fopen failed\n");
			exit(EXIT_FAILURE);
		}
		
		sortStd(fileHandle, buffer1, buffer2, recordSize);
		
		if (fclose(fileHandle) != 0) {
			fprintf(stderr, "fclose failed\n");
			exit(EXIT_FAILURE);
		}
		
	}
	
	//Stop timer
	if((clockEnd = times(&tmsEnd)) < 0) {
		fprintf(stderr, "times error/n");
	}
	
	//Print time
	printTime(clockStart, clockEnd, &tmsStart, &tmsEnd);
	
	return 0;
}
//--------------------------------------------------------------------------------
/* 
 * Swap pair of records in file (records read into buffer1 and buffer2).
 */
int swapInFileSys(unsigned char *buffer1, unsigned char *buffer2, int recordSize, int fileDesc) {
	if (lseek(fileDesc, (-1) * recordSize, SEEK_CUR) == -1) {
		fprintf(stderr, "lseek error");
		exit(EXIT_FAILURE);
	}
	
	if (write(fileDesc, buffer1, recordSize) != recordSize) {
		fprintf(stderr, "write error");
		exit(EXIT_FAILURE);
	}
	
	if (write(fileDesc, buffer2, recordSize) != recordSize) {
		fprintf(stderr, "write error");
		exit(EXIT_FAILURE);
	}
	
	if (lseek(fileDesc, (-1) * recordSize, SEEK_CUR) == -1) {
		fprintf(stderr, "lseek error");
		exit(EXIT_FAILURE);
	}
	
	return 0;
}

/* 
 * Insertion sort based on records comparison (in 2 buffers) using system functions
 */
void sortSys(int fileHandle, unsigned char *buffer1, unsigned char *buffer2, int recordSize) {
	
	int res;
	int n = 0;

	off_t offsetEnd;
	long nmax;
	
	if ((offsetEnd = lseek(fileHandle, (-1) * recordSize, SEEK_END)) == -1) {
			fprintf(stderr, "lseek error\n");
			exit(EXIT_FAILURE);
	}

	nmax = (long) (offsetEnd / recordSize);
	
	do {
	
		if (lseek(fileHandle, (n+1) * recordSize, SEEK_SET) == -1) {
			fprintf(stderr, "lseek error\n");
			exit(EXIT_FAILURE);
		}
		
		res = read(fileHandle, buffer1, recordSize);
		
		if (res == 0) {
			fprintf(stderr, "unexpected EOF\n");
			exit(EXIT_FAILURE);
		} else if (res != recordSize) {
			fprintf(stderr, "read error\n");
			exit(EXIT_FAILURE);
		}
		
		while (lseek(fileHandle, (-2)*recordSize, SEEK_CUR) != -1){
			
			res = read(fileHandle, buffer2, recordSize);
					
			if (res == -1) {
				fprintf(stderr, "read error\n");
				exit(EXIT_FAILURE);
			}
			
			if (buffer2[0] > buffer1[0]){
				swapInFileSys(buffer1, buffer2, recordSize, fileHandle);
			}
		} 
		
		n++;
		
		
	} while (n < nmax);
}
//-----------------------------------------------------------
/* 
 * Swap pair of records in file (records read into buffer1 and buffer2).
 */
int swapInFileStd(unsigned char *buffer1, unsigned char *buffer2, int recordSize, FILE *fileDesc) {
	if (fseek(fileDesc, (-1) * recordSize, SEEK_CUR) != 0) {
		fprintf(stderr, "fseek error");
		exit(EXIT_FAILURE);
	}

	if (fwrite(buffer1, sizeof(char), recordSize, fileDesc) != recordSize) {
		fprintf(stderr, "fwrite error");
		exit(EXIT_FAILURE);
	}
	
	if (fwrite(buffer2, sizeof(char), recordSize, fileDesc) != recordSize) {
		fprintf(stderr, "fwrite error");
		exit(EXIT_FAILURE);
	}
	
	if (fseek(fileDesc, (-1) * recordSize, SEEK_CUR) != 0) {
		fprintf(stderr, "fseek error");
		exit(EXIT_FAILURE);
	}

	return 0;
}

/* 
 * Insertion sort based on records comparison (in 2 buffers) using std functions
 */
 
void sortStd(FILE *fileHandle, unsigned char *buffer1, unsigned char *buffer2, int recordSize) {
	
	size_t res;
	int n = 0;

	long offsetEnd;
	long nmax;
	
	if (fseek(fileHandle, (-1) * recordSize, SEEK_END) == -1) {
		fprintf(stderr, "fseek error\n");
		exit(EXIT_FAILURE);
	}
	
	if ((offsetEnd = ftell(fileHandle)) == -1L) {
		fprintf(stderr, "ftell error\n");
		exit(EXIT_FAILURE);
	}

	nmax = (long) (offsetEnd / recordSize);
	
	do {
	
		if (fseek(fileHandle, (n+1) * recordSize, SEEK_SET) == -1) {
			fprintf(stderr, "fseek error\n");
			exit(EXIT_FAILURE);
		}
		
		res = fread(buffer1, sizeof(char), recordSize, fileHandle);
		
		if (feof(fileHandle) != 0) {
			fprintf(stderr, "unexpected EOF\n");
			exit(EXIT_FAILURE);
		} else if (res != recordSize) {
			fprintf(stderr, "fread error\n");
			exit(EXIT_FAILURE);
		}
		
		while (fseek(fileHandle, (-2)*recordSize, SEEK_CUR) == 0){
			res = fread(buffer2, sizeof(char), recordSize, fileHandle);
					
			if (res != recordSize) {
				fprintf(stderr, "read error\n");
				exit(EXIT_FAILURE);
			}
			
			if (buffer2[0] > buffer1[0]) {
				swapInFileStd(buffer1, buffer2, recordSize, fileHandle);
			}
		} 
		
		n++;
		
		
	} while (n < nmax);
}

//-------------------------------------------------

void printTime(clock_t start, clock_t stop,
	struct tms *tmsStart, struct tms *tmsStop) {
	long clk = sysconf(_SC_CLK_TCK);
	
	printf("|------------------|\n");
	printf("| real | %*.6fs |\n", 7, (stop - start) / (double)clk);
	printf("| user | %*.6fs |\n",
		7,	(tmsStop->tms_utime - tmsStart->tms_utime) / (double)clk);
	printf("| sys  | %*.6fs |\n",
		7, (tmsStop->tms_stime - tmsStart->tms_stime) / (double)clk);
	printf("|------------------|\n");
}
