#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<limits.h>
enum TokenType{INTEGER,FLOAT,VARIABLE,OPERATOR,BRACKET};
enum ExprType{EVAL,ASSIGNMENT,GRAMMARMISTAKE};
typedef struct Token {
    int type;
    char str[32];
} token;
typedef struct Assignment {
    char name[32];
    int type;
    int ival;
    float fval;
} assignment;
token tokens[32];
bool End=1;

const char *operators[5]={"+","-","*","/","="};
const char *brackets[2]={"(",")"};

int Tokenizer();
int main() {
    while(End) {
        int num_of_tokens = Tokenizer();
        if(num_of_tokens==-1) {
            printf("Error\n");
            continue;
        }
        //todo;
    }
    return 0;
}

int TypeJudge(const char *str) {
    //operator;
    for(int i=0;i<5;i++) {
        if(strcmp(operators[i],str)==0) {
            return OPERATOR;
        }
    }
    //bracket;
    for(int i=0;i<2;i++) {
        if(strcmp(brackets[i],str)==0) {
            return BRACKET;
        }
    }
    const int length = strlen(str);
    //variable;
    bool is_variable=1;
    for(int i=0;i<length;i++) {
        if(i==0) {
            if(str[i]!='_' && !isalpha(str[i])) {
                is_variable=0;
                break;
            }
        }
        else {
            if(str[i]!='_' && !isalnum(str[i])) {
                is_variable=0;
                break;
            }
        }
    }
    if(is_variable) return VARIABLE;
    //integer;
    bool is_integer=1;
    for(int i=0;i<length;i++) {
        if(!isdigit(str[i])) {
            is_integer=0;
            break;
        }
    }
    if(is_integer) return INTEGER;
    //float;
    if(length>2 && str[0]!='.' && str[length-1]!='.') {
        bool is_float=1;
        int cnt=0;
        for(int i=0;i<length;i++) {
            if(str[i]=='.') {
                cnt++;
                if(cnt>=2) {
                    is_float=0;
                    break;
                }
            }
            else if(!isdigit(str[i])) {
                is_float=0;
                break;
            }
        }
        if(is_float) return FLOAT;
    }
    return -1;
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