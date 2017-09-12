#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define MAX 26


//Matt Noblett
//Breaks encrypted text
//Winter 2017 - Tao

void readFreq(float given[], char fname[]);
void calcFreq(float found[], char fname[]);
char rotate(char ch, int num);
int findKey(float given[], float found[]);
void decrypt(int key, char filein[], char fileout[]);

int main(int argc, char *fname[]){
    float givenFreq[MAX];
    float calculatedFreq[MAX];
    int key;
    int i;

    readFreq(givenFreq, "LetFreq.txt");
    calcFreq(calculatedFreq, fname[1]);
    key = findKey(givenFreq, calculatedFreq);
    decrypt(key, fname[1], fname[2]);
}



void readFreq(float given[], char fname[]){
    int i = 0;
    char str[20];
    char end[] = ";\n";

    FILE *filein = fopen(fname, "r");

    while(fgets(str, sizeof(str), filein) != NULL){
        strtok(str, end);
        given[i] = atof(strtok(NULL, end));
        strtok(NULL, end);
        i++;
    }
    fclose(filein);
}

void calcFreq(float found[], char fname[]){
    int c;
    int i;
    int count = 0;
    int letter = 0;

    FILE *filein = fopen(fname, "r");

    for(i =0; i < MAX; i++){
        found[i] = 0.0;
    }

    while((c = fgetc(filein)) != EOF){
        if(isalpha(c)){
            count++;
            letter = (islower(c)) ? (c- 'a') : (c - 'A');
            found[letter]++;
        }
    }
    for(i = 0; i < MAX; i++){
        found[i] /= count;
    }
    fclose(filein);
}


char rotate(char ch, int num){
    if(isalpha(ch)){
        return (islower(ch)) ? ((ch + num - 'a')%MAX)+'a' : ((ch+num-'A')%MAX)+'A';
    }
    return ch;
}


int findKey(float given[], float found[]){
    float sum;
    float dif;
    float lowest = 2;
    int i;
    int j;
    int key;

    for(i = 0; i < MAX; i++){
        sum = 0;
        for(j = 0; j < MAX; j++){
            dif = given[j] - found[(j+i)%MAX];
            sum += (dif * dif) / 2;
        }

        if(sum < lowest){
            lowest = sum;
            key = i;
        }
    }
    return MAX - key;
}

//This is used to decrypt the encoded text file that has been passed in.
void decrypt(int key, char filein[], char fileout[]){
    int c;
    FILE *in = fopen(filein, "r");
    FILE *out =fopen(fileout, "w");

    while((c = fgetc(in)) != EOF){
        fprintf(out, "%c", rotate(c, key));
    }
    fclose(in);
    fclose(out);
}


