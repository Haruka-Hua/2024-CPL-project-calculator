#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include<limits.h>
enum TokenType{INTEGER,FLOAT,VARIABLE,OPERATOR,BRACKET};
enum ExprType{EXPR,ASSIGNMENT,ERROR};
typedef struct Token {
    int type;
    char str[32];
} token;
typedef struct Assignment {
    char name[32];
    int type;
    int int_val;
    double float_val;
} assignment;
token tokens[128];
assignment assignments[64];
bool End=1;
bool grammar_check=1;
int num_of_assignments=0;
const char *operators[5]={"+","-","*","/","="};
const char *brackets[2]={"(",")"};

int Tokenizer();
int ExprTypeJudge();
int IntEval();
double FloatEval();
int VariableExist(const char *str);
int main() {
    FILE *fp=fopen("d:\\CLionProjects\\Calculator\\cal.in","r");
    while(End) {
        grammar_check=1;
        int num_of_tokens = Tokenizer(fp);
        if(num_of_tokens==-1) {
            printf("Error\n");
            continue;
        }
        if(num_of_tokens==0) {
            return 0;
        }
        int expr_type = ExprTypeJudge(0,num_of_tokens-1);
        if(expr_type==EXPR) {
            //needs to be revised when float appears;
            int ans = IntEval(0,num_of_tokens-1);
            if(grammar_check) {
                printf("%d\n",ans);
            }
            else printf("Error\n");
        }
        if(expr_type==ASSIGNMENT) {
            //needs to be revised when float appears;
            if(tokens[0].type==VARIABLE && tokens[1].str[0]=='=') {
                int ans = IntEval(2,num_of_tokens-1);
                if(grammar_check) {
                    printf("%d\n",ans);
                    int pos=VariableExist(tokens[0].str);
                    if(pos>=0) {
                        assignments[pos].int_val=ans;
                    }
                    else {
                        assignments[num_of_assignments].int_val=ans;
                        strcpy(assignments[num_of_assignments].name,tokens[0].str);
                        num_of_assignments++;
                    }
                }
                else printf("Error\n");
            }
            else printf("Error\n");
        }
    }
    return 0;
}

int TokenTypeJudge(const char *str) {
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
int Tokenizer(FILE *fp) {
    int num_of_tokens=0;
    char str[32];
    char ch;
    do {
        if(fscanf(fp,"%s",str)==EOF) {
            End=0;
            return 0;
        }
        if(num_of_tokens>=0) {
            strcpy(tokens[num_of_tokens].str,str);
            int token_type = TokenTypeJudge(str);
            if(token_type==-1) num_of_tokens=-1;
            else {
                tokens[num_of_tokens].type = token_type;
                num_of_tokens++;
            }
        }
        ch=fgetc(fp);
    } while(ch!='\n' && ch!=EOF);
    if(ch==EOF) End=0;
    return num_of_tokens;
}
int ExprTypeJudge(int l,int r) {
    for(int i=l;i<=r;i++) {
        if(strcmp("=",tokens[i].str)==0) return ASSIGNMENT;
    }
    return EXPR;
}
bool check_brackets(int l,int r) {
    if(tokens[l].str[0]=='(' && tokens[r].str[0]==')') {
        int cnt_bracket=0;
        for(int i=l+1;i<=r-1;i++) {
            if(tokens[i].str[0]=='(') {
                cnt_bracket++;
            }
            if(tokens[i].str[0]==')') {
                cnt_bracket--;
                if(cnt_bracket<0) return 0;
            }
        }
        if(cnt_bracket!=0) return 0;
        return 1;
    }
    else return 0;
}
int GetMainOp(int l,int r) {
    //in_bracket
    int in_bracket[r-l+1]={};
    for(int i=l;i<=r;i++) {
        if(tokens[i].str[0]=='(') {
            int cnt=1;
            while(cnt != 0 && i<=r) {
                if(tokens[i].str[0]=='(') {
                    cnt++;
                }
                else if(tokens[i].str[0]==')') {
                    cnt--;
                }
                in_bracket[i-l]=1;
                i++;
            }
        }
    }
    // +-
    for(int i=r;i>=l;i--) {
        if(in_bracket[i-l]!=1 && (tokens[i].str[0]=='+' || tokens[i].str[0]=='-')) {
            return i;
        }
    }
    for(int i=r;i>=l;i--) {
        if(in_bracket[i-l]!=1 && (tokens[i].str[0]=='*' || tokens[i].str[0]=='/'))
            return i;
    }
    return -1;
}
int IntEval(int l,int r) {
    if(l>r) {
        grammar_check=0;
        return 0;
    }
    else if(l==r) {
        if(tokens[l].type==INTEGER) {
            return atoi(tokens[l].str);
        }
        else if(tokens[l].type==VARIABLE) {
            for(int i=0;i<num_of_assignments;i++) {
                if(strcmp(tokens[l].str,assignments[i].name)==0) {
                    return assignments[i].int_val;
                }
            }
            grammar_check=0;
            return 0;
        }
        else {
            grammar_check=0;
            return 0;
        }
    }
    else if(check_brackets(l,r)) {
        return IntEval(l+1,r-1);
    }
    else {
        int op = GetMainOp(l,r);
        if(op<=l || op>=r) grammar_check=0;
        int val1=IntEval(l,op-1), val2=IntEval(op+1,r);
        if(grammar_check) {
            switch (tokens[op].str[0]) {
                case '+': {return val1+val2;}
                case '-': {return val1-val2;}
                case '*': {return val1*val2;}
                case '/': {return val1/val2;}
                default: {
                    printf("Error\n");
                    grammar_check=0;
                    return 0;
                }
            }
        }
        else return 0;
    }
}
double FloatEval() {
    //todo;
}
int VariableExist(const char *str) {
    for(int i=0;i<num_of_assignments;i++) {
        if(strcmp(assignments[i].name,str)==0) {
            return i;
        }
    }
    return -1;
}