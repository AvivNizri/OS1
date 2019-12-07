#include <stdio.h>
#include <sys/fcntl.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("The system got more items than needed..\n exiting...");
		return 0;
	}

	// Opening Both Files and holding their file descriptors
	int File0 = open(argv[1], O_RDONLY);
	int File1 = open(argv[2], O_RDONLY);

	if(File0 == -1 || File1 == -1){
		printf("There was an error opening the files\n exiting...");
		close(File0);
		close(File1);
		return 0;
	}

	int buff0, buff1 = 0;	

	do{
		char buffer0, buffer1 = 0;
		buff0 = read(File0, &buffer0, 1);
		buff1 = read(File1, &buffer1, 1);

		//printf("FIle0 char is %c\n", buffer0);
		//printf("FIle1 char is %c\n", buffer1);

		if(buff0 != buff1){
			close(File0);
			close(File1);
			//printf("The Files not the same, exited 1\n");
			exit(1);
		}else{
			buff0 = read(File0, &buffer0, 1);
			buff1 = read(File1, &buffer1, 1);
			//printf("FIle0 char is %c\n", buffer0);
			//printf("FIle1 char is %c\n", buffer1);
		}
	}while(buff0 && buff1);

	// whether one of the files is at the EOF and the other one isnt
	if((buff0 == 0 && buff1 != 0) || (buff0 != 0 && buff1 == 0)){
		close(File0);
		close(File1);
		exit(1);
	}

	close(File0);
	close(File1);
	//printf("The Files are the same!! , exited 2\n");
	exit(2);
}