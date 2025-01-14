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
    int i;
    float f;
    struct Token *next;
    struct Token *prev;
} token;
token *head=NULL, *tail=NULL;
const char operators[]="+-*/=";
const char brackets[]="()";
bool End=0;

int main() {
    //TODO
    return 0;
}

void AppCreate(int type, char *str) {
    //TODO
}

bool tokenizer() {
    bool flag=1;
    char str[32];
    char ch;
    do {
        scanf("%s",str);
        int length = strlen(str);
        //operators;
        if(length==1 && strchr(operators,str[0])) {
            AppCreate(OPERATOR,str);
        }
        //brackets;
        else if(length==1 && strchr(brackets,str[0])) {
            AppCreate(BRACKET,str);
        }
        //integer;
        for(int i=0;i<length;i++) {
            if(!isdigit(str[i])) {
                flag=0;
                break;
            }
        }
        if(flag==1)
        AppCreate(INT,str);
        //float;
        ch = getchar();
    }while(ch!='\n' && ch!=EOF);
    if(ch==EOF) End=1;
    return flag;
}