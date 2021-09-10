#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

typedef struct line {
	int valid;
	char *tag;
	int usage;
}Line;

long int reads=0;
long int writes=0;
long int hits=0;
long int miss=0;
long int ptr=0;
long int sbits=0;
long int offbits=0;
long int tagbits=0;
int length=0;
int n=0;
Line **cache;

int main(int argc,char *argv[]) {

bool pow(int n) {

	if(n==0) {
		return 0;
		}
	while(n!=1)
	{
		if(n%2!=0){
		return 0;
		}
		n=n/2;
	}
	return 1;
}

char *hexbin(char *hex) {

	char bin[48]="";

	for(int i=0;hex[i]!='\0';i++) {
		switch(hex[i]) {
			case '0':
				strcat(bin,"0000");
				break;
			case '1':
				strcat(bin,"0001");
				break;
			case '2':
				strcat(bin,"0010");
				break;
			case '3':
				strcat(bin,"0011");
				break;
			case '4':
				strcat(bin,"0100");
				break;
			case '5':
				strcat(bin,"0101");
				break;
			case '6':
				strcat(bin,"0110");
				break;
			case '7':
				strcat(bin,"0111");
				break;
			case '8':
				strcat(bin,"1000");
				break;
			case '9':
				strcat(bin,"1001");
				break;
			case 'A':
			case 'a':
				strcat(bin,"1010");
				break;
			case 'B':
			case 'b':
				strcat(bin,"1011");
				break;
			case 'C':
			case 'c':
				strcat(bin,"1100");
				break;
			case 'D':
			case 'd':
				strcat(bin,"1101");
				break;
			case 'E':
			case 'e':
				strcat(bin,"1110");
				break;

			case 'F':
			case 'f':
				strcat(bin,"1111");
				break;
			default:
				break;
		}
	}

	int check = strlen(bin);

	int filler = 48-check;

	if (check != 48) {
		for(int i=0;i<filler;i++) {
			hex[i] = '0';
		}
	}

	for(int i=filler;i<48;i++) {
		hex[i]=bin[i-filler];
	}
		

	return hex;
}

/*printf("%d\n",argc);
printf("%s\n",argv[0]);
printf("%s\n",argv[1]);
printf("%s\n",argv[2]);
printf("%s\n",argv[3]);
printf("%s\n",argv[4]);
printf("%s\n",argv[5]);*/

if(argc < 6) {
	printf("Not enough arguments!\n");
	return -1;
}

char *filename = argv[5];

FILE *infile = fopen(filename,"r");

if(infile == NULL) {
	printf("Input file is NULL\n");
	return -1;
}

long int cacheSize = atoi(argv[1]);

if(!pow(cacheSize)) {
	printf("error\n");
	return 0;
}

long int blockSize = atoi(argv[2]);

if(!pow(blockSize)) {
	printf("error\n");
	return 0;
}

long int numSets = (cacheSize/blockSize);

char fifo[]="fifo";

char lre[]="lru";

char policy[5];

int lreC=0;

int fifoC=0;

strcpy(policy,argv[3]);

int cmp=1;

cmp = strcmp(policy,fifo);

if(cmp == 0) {
	fifoC=1;
}
else {
	cmp = strcmp(policy,lre);
	if(cmp == 0) {
		lreC=1;	
	}
	else {
 		printf("error");
		return 0;
	}	
}

cmp=1;

int size = strlen(argv[4]);

char type[size];

strcpy(type,argv[4]);

char direct[]="direct";

char assoc[]="assoc";

char assocn[]="assoc:n";

int dir=0;
int asscnum=0;
int assc=0;

cmp = strcmp(type,direct);

if(cmp == 0) {
	dir=1;
}
else {
	cmp = strcmp(type,assoc);
	if(cmp == 0) {
		assc=1;	
	}
	else {
 		if(type[5]==':') {
			asscnum = 1;
		}
		else {
			printf("error");
			return 0;
		}	
	}	
}

if(dir==1) {
	
	sbits = log(numSets)/log(2);
	offbits = log(blockSize)/log(2);
	tagbits = 48-(offbits+sbits);

	cache = (Line **)malloc(sizeof(Line)*numSets);

	for(int i=0;i<numSets;i++) {
		cache[i]=(Line*)malloc(sizeof(Line));
	}

	for(int i=0;i<numSets;i++) {	
		cache[i][0].valid=0;
		cache[i][0].tag=malloc(sizeof(char)*tagbits);
		cache[i][0].usage=0;
	}
}

if(assc==1) {

	sbits = log(numSets)/log(2);
	offbits = log(blockSize)/log(2);
	tagbits = 48-offbits;
	n=numSets;

	cache = (Line**)malloc(sizeof(Line)*1);
	
	cache[0]=(Line*)malloc(sizeof(Line)*numSets);

	for(int j=0;j<numSets;j++) {
		cache[0][j].valid=0;
		cache[0][j].tag=malloc(sizeof(char)*tagbits);
		cache[0][j].usage=0;
	}

}

if(asscnum==1) {
	
	n =(int)type[6];
	n = n-'0';
	numSets = (cacheSize/(blockSize*n));
	sbits = log(numSets)/log(2);
	offbits = log(blockSize)/log(2);
	tagbits = 48-(offbits+sbits);
	
	cache = (Line**)malloc(sizeof(Line)*numSets);

	for(int i=0;i<numSets;i++) {
		cache[i]=(Line*)malloc(sizeof(Line)*n);
	}

	for(int i=0;i<numSets;i++) {
		for(int j=0;j<n;j++) {
			cache[i][j].valid=0;
			cache[i][j].tag=malloc(sizeof(char)*tagbits);
			cache[i][j].usage=0;
		}
	}
}

int checkfull(Line **cache) {
	
	int check=0;
	
	for(int i=0;i<numSets;) {

		if(cache[0][i].valid==1) {
			i++;
			check=1;

		}
		else {
			check=0;
			break;	
		}
	}
	return check;
}

int checkline(Line **cache, long int index) {
	
	int check=0;
	
	for(int i=0;i<n;) {

		if(cache[index][i].valid==1) {
			i++;
			check=1;

		}
		else {
			check=0;
			break;	
		}
	}
	return check;
}

Line** LRUshifts(Line **cache, long int index, int i) {
	
	char temp[tagbits];
	
	for(int start=i;start>0;start--) {
		if(cache[index][start].valid==1) {
			if((i-1)<0) {break;}
			if(cache[index][start-1].valid==0) {break;}
			for(int j=0;j<tagbits;j++) {
				temp[j]=cache[index][start].tag[j];
			}
			for(int j=0;j<tagbits;j++) {
				cache[index][start].tag[j]=cache[index][start-1].tag[j];
			}
			for(int j=0;j<tagbits;j++) {
				cache[index][start-1].tag[j]=temp[j];
			}
		}
		else {
			break;	
		}
	}
	return cache;
}

Line** LRUFullshifts(Line **cache, long int index, char* tag) {
	
	char temp[tagbits];
	char temp2[tagbits];
	
	for(int i=0;i<n;i++) {
		if(cache[index][i].valid==1) {
			if(i==0) {
				for(int j=0;j<tagbits;j++) {
					temp[j]=cache[index][i].tag[j];
				}
				for(int j=0;j<tagbits;j++) {
					cache[index][i].tag[j]=tag[j];
				}
			}
			else {
				for(int j=0;j<tagbits;j++) {
					temp2[j]=cache[index][i].tag[j];
				}
				for(int j=0;j<tagbits;j++) {
					cache[index][i].tag[j]=temp[j];
				}
				for(int j=0;j<tagbits;j++) {
					temp[j]=temp2[j];
				}
			}
		}
		else {
			break;	
		}
	}
	return cache;
}



long int getAddress(char *hex,long int sbits,long int tagbits) {

	long int dec_value=0;

	int base = 1;
	
	for(int i=tagbits+sbits-1;i>=tagbits;i--) {
		if(hex[i] == '1') {
		dec_value += base;
		}
		base = base*2;
	}

	return dec_value;		
}

int checktag(char *tag,char *tagcmp) {

	int check=0;
	
	for(int i=0;i<tagbits;) {

		if(tag[i]==tagcmp[i]) {
			i++;
			check=1;

		}
		else {
			check=0;
			break;	
		}
	}
	return check;
}

Line** read(Line **cache, char *hex,long int sbits,long int tagbits, int n) {
	
	long int index=0;
	index = getAddress(hex,sbits,tagbits);
	char tag[tagbits];
	int full=0;

	for(int i=0;i<tagbits;i++) {
		tag[i]=hex[i];
	}

	if(dir==1) {
		
		if(fifoC==1) {
					
		if(cache[index][0].valid==0) {
			reads++;
			miss++;
			cache[index][0].valid=1;

			for(int i=0;i<tagbits;i++) {
				cache[index][0].tag[i]=tag[i];
			}
		}
		else {

			int check = checktag(cache[index][0].tag,tag);			
			if(check==1) {
				hits++;
			}			
			else {
				miss++;
				reads++;
				cache[index][0].valid=1;

				for(int i=0;i<tagbits;i++) {
					cache[index][0].tag[i]=tag[i];
				}	
			}		
		}
	}

	}

	if(assc==1) {

	if(fifoC==1) {
		
		full = checkfull(cache);

		if(full==0) {
			for(long int i=0;i<numSets;i++) {
				if(cache[0][i].valid ==0) {
					reads++;
					miss++;
					cache[0][i].valid=1;
					for(int j=0;j<tagbits;j++) {
						cache[0][i].tag[j]=tag[j];
					}
					break;	
				}
				else if(cache[0][i].valid == 1 && checktag(cache[0][i].tag,tag)==1) {
					hits++;
					break;
				}	
			}
		}
		else {
			int check1=0;
			for(long int i=0;i<numSets;i++) {
					if(checktag(cache[0][i].tag,tag) == 1) {
						//printf("fullhit\n");
						hits++;
						check1=1;
						break;
					}
			}
			if(check1==0) {
				reads++;
				miss++;
				for(int i=0;i<tagbits;i++) {
					cache[0][ptr].tag[i]=tag[i];
				}
				if(ptr==numSets-1) {
					ptr=0;
				}
				else {
					ptr++;	
				}						
			}	
		}
	}
	
	else if(lreC==1) { 
	
		full = checkfull(cache);

		if(full==0) {
			
			for(int i=(numSets-1);i>=0;i--) {
				if(cache[0][i].valid ==0) {
					reads++;
					miss++;
					cache[0][i].valid=1;
					for(int j=0;j<tagbits;j++) {
						cache[0][i].tag[j]=tag[j];
					}
					break;	
				}
				else if(cache[0][i].valid == 1 && checktag(cache[0][i].tag,tag)==1) {
					hits++;
					LRUshifts(cache,0,i);
					break;
				}				
			}
		}
		else {
			int check2=0;
			for(long int i=(numSets-1);i>=0;i--) {
					if(checktag(cache[0][i].tag,tag) == 1) {
						//printf("fullhit\n");
						hits++;
						LRUshifts(cache,0,i);
						check2=1;
						break;
					}
			}
			if(check2==0) {
				reads++;
				miss++;
				//printf("fullmiss\n");
				LRUFullshifts(cache,0, tag);
				
			}
		}
	}

	}
	
	if(asscnum==1) {

		int totalUsage=0;
		int check=0;
		int checklinefull=checkline(cache,index);
	
		if(fifoC==1) {
			if(cache[index][0].valid==0) {
				miss++;
				reads++;
				cache[index][0].valid=1;
				cache[index][0].usage=0;
				for(int i=0;i<tagbits;i++) {
					cache[index][0].tag[i]=tag[i];
				}
			}
			else {
				for(long int i=0;i<n;i++) {
					if(cache[index][i].valid==1 && checktag(cache[index][i].tag,tag)==1) {
						hits++;
						check=1;
						break;
					}
				}
				if(check==0) {
					miss++;
					reads++;
					if(checklinefull==0) {
						for(long int l=0;l<n;l++) {
							if(cache[index][l].valid==0) {
								cache[index][l].valid=1;
								cache[index][l].usage=0;
								for(int i=0;i<tagbits;i++) {
									cache[index][l].tag[i]=tag[i];
								}
								break;
							}
						}
					}
					else if(checklinefull==1) {
						for(int i=0;i<n;) {
							if(cache[index][i].usage==1){i++;}
							else { break; }					
							if(i==n) {totalUsage=1;}
						}
						if(totalUsage==1) {
							for(int i=0;i<n;i++) {
								cache[index][i].usage=0;
							}
							totalUsage=0;
						}
						for(long int l=0;l<n;l++) {
							if(cache[index][l].usage==0) {
								cache[index][l].usage=1;
								for(int i=0;i<tagbits;i++) {
									cache[index][l].tag[i]=tag[i];
								}
								break;
							}
						}
					}
				}
			}
		}	
		else if(lreC==1) { 
			for(long int i=(n-1);i>=0;i--) {
				if(cache[index][i].valid==1 && checktag(cache[index][i].tag,tag)==1) {
					LRUshifts(cache,index,i);
					hits++;
					check=1;
					break;
				}
			}
			if(check==0) {
				miss++;
				reads++;
				if(checklinefull==0) {
					for(long int l=(n-1);l>=0;l--) {
						if(cache[index][l].valid==0) {
							cache[index][l].valid=1;
							for(int i=0;i<tagbits;i++) {
								cache[index][l].tag[i]=tag[i];
							}
							break;
						}
					}
				}
				else if(checklinefull==1) {
					LRUFullshifts(cache,index,tag);
				}
			}
		}
	}
	return cache;
}

	
Line**write(Line **cache, char *hex,long int sbits,long int tagbits, int n) {

	long int index=0;
	index = getAddress(hex,sbits,tagbits);
	char tag[tagbits];

	for(long int i=0;i<tagbits;i++)  {

	tag[i]=hex[i];
	}	
	
	if(dir==1) {

		if(cache[index][0].valid==0) {
			writes++;
			read(cache,hex,sbits,tagbits,n);
		}
		else {
			int cmp = checktag(cache[index][0].tag,tag);

			if(cmp == 1) {
				hits++;
				writes++;
			}
			else {
				read(cache,hex,sbits,tagbits,n);
				writes++;
			}
		}
	}

	if(assc==1) {

		int check=0;
		writes++;

		for(long int i=0;i<numSets;i++) {
			if(cache[0][i].valid==1) {
				if(checktag(cache[0][i].tag,tag) == 1) {
					hits++;
					if(lreC==1) { LRUshifts(cache,0,i); }
					check=1;
					break;
				}
			}
		}

		if(check==0) {
			read(cache,hex,sbits,tagbits,n);
		}
	}

	if(asscnum==1) {

		writes++;
		
		int check=0;

		for(int i=0;i<n;i++) {
			if(checktag(cache[index][i].tag,tag) == 1) {
			if(lreC==1) { LRUshifts(cache,index,i); }
			hits++;
			check=1;
			break;
			}
		}
		if(check==0) {
			read(cache,hex,sbits,tagbits,n);
		}
	}
	return cache;	
}

void printstats() {

printf("Memory reads: ");
printf("%ld\n",reads);
printf("Memory writes: ");
printf("%ld\n",writes);
printf("Cache hits: ");
printf("%ld\n",hits);
printf("Cache misses: ");
printf("%ld\n",miss);

}

char op;
char hex[48];

while(fscanf(infile,"%c\t %s\n",&op,hex)) {
	     
	      hexbin(hex);

              switch(op) {
                  case 'R': 
			read(cache,hex,sbits,tagbits,n);
			break;
                  case 'W':
			write(cache,hex,sbits,tagbits,n); 
			break;
         	}

		if(feof(infile)) {
			break;
		}
}

if(dir==1) {
for(int i=0;i<numSets;i++) {
	free(cache[i][0].tag);
}

for(int i=0;i<numSets;i++) {
	free(cache[i]);
}
}

if(assc==1) {
for(int i=0;i<numSets;i++) {
	free(cache[0][i].tag);
}
	free(cache[0]);
}

if(asscnum==1) {
	for(int i=0;i<numSets;i++) {
		for(int j=0;j<n;j++) {
		free(cache[i][j].tag);
		}
		free(cache[i]);
	}
}

free(cache);

printstats();

return 0; 
    
}
