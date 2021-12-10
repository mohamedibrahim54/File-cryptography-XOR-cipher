#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void listcurrntdir(const char *name,int level) {
	DIR *dir;
	struct dirent *entry;



	if (!(dir = opendir(name)))
		return;
	if (!(entry = readdir(dir)))
		return;

	do {
		if (entry->d_type == DT_DIR) {
			char path[1024];
			int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
			path[len] = 0;
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
			printf("%*s[%s]\n", level*2, "", entry->d_name);
		} else
			printf("%*s- %s\n", level*2, "", entry->d_name);
	} while (entry = readdir(dir));
	closedir(dir);
}

void listdir(const char *name, int level) {
	DIR *dir;
	struct dirent *entry;

	if (!(dir = opendir(name)))
		return;
	if (!(entry = readdir(dir)))
		return;

	do {
		if (entry->d_type == DT_DIR) {
			char path[1024];
			int len = snprintf(path, sizeof(path)-1, "%s/%s", name, entry->d_name);
			path[len] = 0;
			if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
				continue;
			printf("%*s[%s]\n", level*2, "", entry->d_name);
			listdir(path, level + 1);
		} else
			printf("%*s- %s\n", level*2, "", entry->d_name);
	} while (entry = readdir(dir));
	closedir(dir);
}

void display(const char *file_name) {
	char ch;
	FILE *fp;

	fp = fopen(file_name,"rb");

	if( fp == NULL ) {
		perror("Error while opening the file.\n");
		return;
	}
	printf("The contents of file are: \n");
	while( ( ch = fgetc(fp) ) != EOF ) {
		printf("%x\t",ch);
	}
	printf("\n");

	fclose(fp);
}

int encrypt(char *dir_name) {
	char ch, file_name[25],newfilename[25],passwd[20], file_path[150], new_file_path[150];
	FILE *fp,*fp1;

	printf("Enter the name of file you wish to encrypt: ");
	scanf("%s",file_name);
	strcpy(file_path,dir_name);
	strcat(file_path,file_name);
	fp = fopen(file_path,"rb"); // read binary file mode
	if( fp == NULL ) {
		perror("Error while opening the file.\n");
		return 0;
	}

	printf("Enter the password: ");
	scanf("%s",passwd);

	printf("Enter the name of file you wish to save encrypted data: ");
	scanf("%s",newfilename);
	strcpy(new_file_path,dir_name);
	strcat(new_file_path,newfilename);

	fp1= fopen(new_file_path,"wb");//write binary file mode

	int block_size = strlen(passwd);
	unsigned char block[block_size];
	int num_bytes;
	do {
		num_bytes = fread(block, 1, block_size, fp);

		// XOR byte by byte
		for (int i = 0; i < num_bytes; i++)
			block[i] ^= passwd[i];

		fwrite(block, 1, num_bytes, fp1);
	} while (num_bytes == block_size);

	fclose(fp);
	fclose(fp1);
	printf("\nencrypted successfully.\n");
	return 0;
}


int decrypt(char *dir_name) {
	char ch, file_name[25],newfilename[25],passwd[10], file_path[150], new_file_path[150];
	FILE *fp,*fp1;
	printf("Enter the name of file you wish to Decrypt: ");
	scanf("%s",file_name);
	strcpy(file_path,dir_name);
	strcat(file_path,file_name);
	fp = fopen(file_path,"rb"); // read binary file mode
	if( fp == NULL ) {
		perror("Error while opening the file.\n");
		return 0;
	}

	printf("Enter the password: ");
	scanf("%s",passwd);
	printf("Enter the name of file you wish to save Decrypyted file: ");
	scanf("%s",newfilename);
	strcpy(new_file_path,dir_name);
	strcat(new_file_path,newfilename);
	fp1= fopen(new_file_path,"wb");//write binary file mode


	int block_size = strlen(passwd);
	unsigned char block[block_size];
	int num_bytes;
	do {
		num_bytes = fread(block, 1, block_size, fp);

		// XOR byte by byte
		for (int i = 0; i < num_bytes; i++)
			block[i] ^= passwd[i];

		fwrite(block, 1, num_bytes, fp1);
	} while (num_bytes == block_size);


	fclose(fp);
	fclose(fp1);
	printf("\nDecrypyted successfully.\n");
	return 0;
}




int main() {
	char dirname[100],filename[25],path[100];
	int choice;
	while(1) {
		printf("\n\t\t\t0 – Exit \n \
                        1 – Select di1rectory to work in \n \
                        2 – List directory content (in first level) \n \
                        3 – List directory content (in all levels)  \n \
                        4 – Delete a file (hexadecimal view) \n \
                        5 – Display file contents  \n \
                        6 – Encrypt a file (XOR with password)  \n \
                        7 – Decrypt a  file (XOR with password) \n");
		printf("\nSelect option: ");

		scanf("%d",&choice);

		if(choice==0) exit(0);

		else if(choice==1) {
			printf("Enter the directory [absolute] name(/home/user/)");
			scanf("%s",dirname);
		}

		else if(choice==2) {
			listcurrntdir(dirname,0);
		}

		else if(choice==3) {
			listdir(dirname,0);
		}

		else if(choice==4) {
			printf("Enter File name you wish to delete: ");
			scanf("%s",filename);
			strcpy(path,dirname);
			strcat(path,filename);
			//del(path);
			int ret = remove(path);
			if(ret == 0) {
				printf("File deleted successfully");
			} else {
				printf("Error: unable to delete the file or file unavailable");
			}
		}

		else if(choice==5) {
			printf("Enter File name you wish to display: ");
			scanf("%s",filename);
			strcpy(path,dirname);
			strcat(path,filename);
			display(path);
		}

		else if(choice==6) {
			encrypt(dirname);

		}

		else if(choice==7) {
			decrypt(dirname);
		}


	}
	return 0;

}