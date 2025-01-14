#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<limits.h>
enum Type{INT,FLOAT,VARIABLE,OPERATOR,BRACKET};
typedef struct Token {
    int type;
    char str[32];
} token;
token tokens[32];
bool End=1;

const char operators[]="+-*/=";
const char brackets[]="()";

int main() {
    //todo
    return 0;
}

int TypeJudge(const char *str) {
    //todo;
}
int Tokenizer() {
    int num_of_tokens=0;
    char str[32];
    char ch;
    do {
        scanf("%s",str);
        if(num_of_tokens>=0) {
            strcpy(tokens[num_of_tokens].str,str);
            int token_type = TypeJudge(str);
            if(token_type==-1) num_of_tokens=-1;
            else {
                tokens[num_of_tokens].type = token_type;
                num_of_tokens++;
            }
        }
        ch=getchar();
    } while(ch!='\n' && ch!=EOF);
    if(ch==EOF) End=0;
    return num_of_tokens;
}