//Matt Noblett
//CIS 361 - Winter 2017
//Cipher Breaker


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


float givenFrequency[26];
float calcFrequency[26];

//This will load the array called given with the letter frequencies for english from the file letfreq.txt
void readFreq(float given[], char fname[]){
    char str[20];
    int x = 0;
    FILE *filein;
    //Open the file and read it
    filein = fopen(fname, "r");
    //Checking to see if the file exists
    if(filein == NULL){
        printf("Cannot Open File");
        exit(0);
    }
    //This converts each piece of the cipher to a double, and gives each number the correct value. 
    while(fgets(str,20, filein) != NULL){
        givenFrequency[x] = atof(str+2);
        x++;
    }
    fclose(filein);
}


//Calc Freq function reads the encoded text from an input file and accumulates the letter frequency data for the encoded text. 
//Then the frequency data is stored in the array called found
void calcFreq(float found[], char fname[]){
    FILE *filein;
    char c;
    int count = 0;

    filein = fopen(fname, "r");

    if(filein == NULL){
        printf("Cannot Open File");
        exit(0);
    }
    //grabs each character, checks to see if it is an alpha, and then calculates the frequency for each character. 
    while((c = fgetc(filein)) != EOF){
        if(isalpha(c)){
            if(c >= 'a' && c <= 'z'){
                found[c - 'a']++;
                count++;
            }
            if(c >= 'A' && c <= 'Z'){
                found[c - 'A']++;
                count++;
            }
        }
    }

    for(int i = 0; i < 26; i++){
        found[i] = (found[i]/count);
    }
    fclose(filein);
}

//Rotate function rotates the character in the ch parameter down the alphabet for the number of positions as given in parameter num 
//and it is returns the resulting character. 
char rotate( char ch, int num){

    if(islower(ch)){
        return((((ch - 'a') + num) % 26) + 'a');
    }else{
        return((((ch - 'A') + num) % 26) + 'A');
    }
    return ch;

}
//Figures out the the difference between the two values that are passed in from the two given values in 
//each position in the array.
float diffTwoSquares(float a, float b){
    float x = a - b;
    return ((x*x)/2);
}
void rotateOne(float a[], int n){
    int i;
    int temp;
    temp = a[0];
    for (i = 0; i < n-1; i++){
        a[i] = a[i+1];
        a[i] = temp;
    }
}

void rotateArray(float a[], int n, int d){
    int i;
    for(i = 0; i< d; i++){
        rotateOne(a,n);
    }
}

int findLeast(float array[], int size){
    int f;
    float temp;
    for(int i = 0; i < size; i++){
        if(array[i] < temp){
            temp = array[i];
            f = i;
        }
    }
    return f;
}

//findKey function takes in the givenFrequency array and the calcFrequency array to 
//put together a difference array.
int findKey(float given[], float found[]){

    float sum;
    float difference[26];
    //Loops through the calcFrequency array and takes each position in that array 
    //and sends it to the rotateArray funciton
    for(int x = 0; x < 26; x++){
        rotateArray(found, 26, x);
        sum = 0;
        //This send each postion in the givenFreq and calcFreq array to figure out
        //the difference of two squares and then is stored in sum. The difference array is then updated
        //at that position with the sum value. 
        for( int y = 0; y < 26; y++){
            sum += diffTwoSquares(given[y], found[y]);
        }
        difference[x] = sum;
    }
    return findLeast(difference, 26);
}


//Gets the input file and the output file, and does the decrypting 
void decrypt(int key, char *in, char *out){
    FILE *filein, *fileout;
    char ch;
    //Opens both the encrypted file and the output file that are going to be used
    filein = fopen(in, "r");
    fileout = fopen(out, "w");
    //Checks to see if the file exists
    if(filein == NULL || fileout == NULL){
        printf("Error, file doesn't exist, or trouble opening\n");
        exit(-1);
    }
    //Loops through the entire file, grabbing each character and then throws it into the rotate function to decrypt it correctly
    while(fscanf(filein, "%c", &ch) != EOF){
        fprintf(fileout, "%c", rotate(ch, key));
    }

    fclose(filein);
    fclose(fileout);
}

//Main of the program that is used to run the program. Calls each function in order that is required to correctly reverse the file
//encryption
int main(int argc, char *argv[]){
    int key;
    if (argc != 3){
        printf("REQUIRED INPUT AND OUTPUT FILE\n");
        exit(1);
    }
    readFreq(givenFrequency, "LetFreq.txt");
    calcFreq(calcFrequency, argv[1]);
    key = findKey(givenFrequency, calcFrequency);
    decrypt(key, argv[1], argv[2]);

    return 0;
}
