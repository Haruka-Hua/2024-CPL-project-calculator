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
assignment assignments[128];
bool End=1;
bool grammar_check=1;
int num_of_assignments=0;
const char *operators[5]={"+","-","*","/","="};
const char *brackets[2]={"(",")"};

int Tokenizer();
bool BracketGrammarCheck();
int ExprTypeJudge();
int IntEval();
double FloatEval();
int AnswerType();
int VariableExist(const char *str);
int IntAssignment();
double FloatAssignment();
void IdentifyNegative();
bool PlusOrMinus();
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
        IdentifyNegative(num_of_tokens);
        if(!BracketGrammarCheck(num_of_tokens)) {
            printf("Error\n");
            continue;
        }
        int expr_type = ExprTypeJudge(0,num_of_tokens-1);
        if(expr_type==EXPR) {
            //needs to be revised when float appears;
            int expr_ans_type = AnswerType(0,num_of_tokens-1);
            if(expr_ans_type==INTEGER) {
                int ans = IntEval(0,num_of_tokens-1);
                if(grammar_check) {
                    printf("%d\n",ans);
                }
                else printf("Error\n");
            }
            if(expr_ans_type==FLOAT) {
                double ans = FloatEval(0,num_of_tokens-1);
                if(grammar_check) {
                    printf("%lf\n",ans);
                }
                else printf("Error\n");
            }
        }
        if(expr_type==ASSIGNMENT) {
            int answer_type = AnswerType(2,num_of_tokens-1);
            if(answer_type==INTEGER) {
                int ans = IntAssignment(0,num_of_tokens-1);
                if(grammar_check) {
                    printf("%d\n",ans);
                }
                else printf("Error\n");
            }
            if(answer_type==FLOAT) {
                double ans = FloatAssignment(0,num_of_tokens-1);
                if(grammar_check) {
                    printf("%lf\n",ans);
                }
                else printf("Error\n");
            }
        }
    }
    return 0;
}

void IdentifyNegative(int num_of_tokens) {
    for(int i=0;i<num_of_tokens;i++) {
        if(i==0 && tokens[i].str[0]=='-') {
            tokens[i].str[0]='#';
        }
        else {
            if(tokens[i].str[0]=='-' && (tokens[i-1].str[0]=='(' || tokens[i-1].type==OPERATOR)) {
                tokens[i].str[0]='#';
            }
        }
    }
}
bool BracketGrammarCheck(int num_of_tokens) {
    int cnt=0;
    for(int i=0;i<num_of_tokens;i++) {
        if(tokens[i].str[0]=='(') cnt++;
        if(tokens[i].str[0]==')') cnt--;
        if(cnt<0) return 0;
    }
    if(cnt!=0) return 0;
    else return 1;
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
            i++;
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
int AnswerType(int l,int r) {
    for(int i=l;i<=r;i++) {
        if(tokens[i].type==FLOAT) return FLOAT;
        if(tokens[i].type==VARIABLE) {
            for(int j=0;j<num_of_assignments;j++) {
                if(strcmp(tokens[i].str,assignments[j].name)==0) {
                    if(assignments[j].type==FLOAT) return FLOAT;
                }
            }
        }
    }
    return INTEGER;
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
        if(r-l<=3) {
            grammar_check=0;
            return 0;
        }
        return IntEval(l+1,r-1);
    }
    else {
        int op = GetMainOp(l,r);
        if(op<=l || op>=r) {
            if(tokens[l].str[0]=='#') {
                return -IntEval(l+1,r);
            }
            else {
                grammar_check=0;
                return 0;
            }
        }
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
double FloatEval(int l,int r) {
    if(l>r) {
        grammar_check=0;
        return 0;
    }
    else if(l==r) {
        if(tokens[l].type==FLOAT) {
            return strtod(tokens[l].str,NULL);
        }
        else if(tokens[l].type==VARIABLE) {
            for(int i=0;i<num_of_assignments;i++) {
                if(strcmp(tokens[l].str,assignments[i].name)==0) {
                    return assignments[i].float_val;
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
        if(r-l<=3) {
            grammar_check=0;
            return 0;
        }
        return FloatEval(l+1,r-1);
    }
    else {
        int op = GetMainOp(l,r);
        if(op<=l || op>=r) {
            if(tokens[l].str[0]=='#') {
                return -FloatEval(l+1,r);
            }
            else {
                grammar_check=0;
                return 0;
            }
        }
        int type1 = AnswerType(l,op-1), type2 = AnswerType(op+1,r);
        if(type1==INTEGER && type2==FLOAT) {
            int val1 = IntEval(l,op-1);
            double val2 = FloatEval(op+1,r);
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
        if(type1==FLOAT && type2==FLOAT) {
            double val1 = FloatEval(l,op-1);
            double val2 = FloatEval(op+1,r);
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
        if(type1==FLOAT && type2==INTEGER) {
            double val1 = FloatEval(l,op-1);
            int val2 = IntEval(op+1,r);
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
        else return 0;
    }
    return 0;
}
int VariableExist(const char *str) {
    for(int i=0;i<num_of_assignments;i++) {
        if(strcmp(assignments[i].name,str)==0) {
            return i;
        }
    }
    return -1;
}
int IntAssignment(int l,int r) {
    if(tokens[l].type==VARIABLE && tokens[l+1].str[0]=='=') {
        int ans;
        if(ExprTypeJudge(l+2,r)==EXPR) {
            ans = IntEval(l+2,r);
            if(grammar_check) {
                int pos=VariableExist(tokens[l].str);
                if(pos>=0) {
                    assignments[pos].int_val=ans;
                    assignments[pos].type=INTEGER;
                }
                else {
                    assignments[num_of_assignments].int_val=ans;
                    assignments[num_of_assignments].type=INTEGER;
                    strcpy(assignments[num_of_assignments].name,tokens[l].str);
                    num_of_assignments++;
                }
                return ans;
            }
            else return 0;
        }
        if(ExprTypeJudge(l+2,r)==ASSIGNMENT) {
            ans = IntAssignment(l+2,r);
            if(grammar_check) {
                int pos=VariableExist(tokens[l].str);
                if(pos>=0) {
                    assignments[pos].int_val=ans;
                    assignments[pos].type=INTEGER;
                }
                else {
                    assignments[num_of_assignments].int_val=ans;
                    assignments[num_of_assignments].type=INTEGER;
                    strcpy(assignments[num_of_assignments].name,tokens[l].str);
                    num_of_assignments++;
                }
                return ans;
            }
            else return 0;
        }
    }
    else {
        grammar_check=0;
        return 0;
    }
}
double FloatAssignment(int l,int r) {
    if(tokens[l].type==VARIABLE && tokens[l+1].str[0]=='=') {
        double ans;
        if(ExprTypeJudge(l+2,r)==EXPR) {
            ans = FloatEval(l+2,r);
            if(grammar_check) {
                int pos=VariableExist(tokens[l].str);
                if(pos>=0) {
                    assignments[pos].float_val=ans;
                    assignments[pos].type=FLOAT;
                }
                else {
                    assignments[num_of_assignments].float_val=ans;
                    assignments[num_of_assignments].type=FLOAT;
                    strcpy(assignments[num_of_assignments].name,tokens[l].str);
                    num_of_assignments++;
                }
                return ans;
            }
            else return 0;
        }
        if(ExprTypeJudge(l+2,r)==ASSIGNMENT) {
            ans = FloatAssignment(l+2,r);
            if(grammar_check) {
                int pos=VariableExist(tokens[l].str);
                if(pos>=0) {
                    assignments[pos].float_val=ans;
                    assignments[pos].type=FLOAT;
                }
                else {
                    assignments[num_of_assignments].float_val=ans;
                    assignments[num_of_assignments].type=FLOAT;
                    strcpy(assignments[num_of_assignments].name,tokens[l].str);
                    num_of_assignments++;
                }
                return ans;
            }
            else return 0;
        }
    }
    else {
        grammar_check=0;
        return 0;
    }
}