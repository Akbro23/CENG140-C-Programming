#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

char *Formula;
long Formula_length;
int Exists[26];
char *Used_chars;
int Char_vals[26];
int Count = 0;
char *Curpos;

void get_formula();
void get_used_chars();
void print_used_chars();
void get_values_and_evaluate(int pos);
void print_values();
void print_result();
int evaluate();


int main(){
    get_formula();
    get_used_chars();
    print_used_chars();
    get_values_and_evaluate(0);
    return 0;
}

void get_formula(){
    char ch;
    long ptr1 = 0;
    Formula = (char *)malloc(sizeof(char));
    Formula[ptr1] = '\0';
    while (ch = getchar(), ch != EOF){
        if (ch != ' ' && ch != '\n'){
            if (islower(ch) && Exists[ch - 97] == 0)
                Exists[ch - 97]++;

            Formula[ptr1] = ch;
            Formula = (char *)realloc(Formula, (++ptr1 + 1) *sizeof(char));
            Formula[ptr1] = '\0';
            Formula_length++;
        }
    }
}

void get_used_chars(){
    int i;
    Used_chars = (char*)malloc(sizeof(char));
    for (i=0; i < 26; i++){
        if (Exists[i]){
            Used_chars[Count] = i + 97;
            Used_chars = (char *)realloc(Used_chars, ++Count*sizeof(char));
        }
    }
}

void print_used_chars(){
    int i;
    for (i = 0; i < Count; i++)
        printf("%c ", Used_chars[i]);
    printf("R\n");
}

void get_values_and_evaluate(int pos){
    if (pos == Count){
        print_values();
        print_result();
    }
    else {
        Char_vals[Used_chars[pos] - 97] = 1;
        get_values_and_evaluate(pos + 1);
        Char_vals[Used_chars[pos] - 97] = 0;
        get_values_and_evaluate(pos + 1);
    }
}

void print_values(){
    int i;
    for (i = 0; i < Count; i++)
        if (Char_vals[Used_chars[i] - 97] == 1)
            printf("T ");
        else
            printf("F ");
}

void print_result(){
    int result;
    Curpos = Formula;
    result = evaluate();
    if (result == 1)
        printf("T\n");
    else
        printf("F\n");
}

int evaluate(){
    if (*Curpos == '-'){
        Curpos++;
        return !evaluate();
    }
    else if (*Curpos == '('){
        int left, right, result;
        char oper;

        Curpos++;
        left = evaluate();
        oper = *Curpos++;
        right = evaluate();
        Curpos++;
        switch (oper){
            case '&':
                result = left && right;
                break;
            case '|':
                result = left || right;
                break;
            case '>':
                result = left <= right;
                break;
            case '=':
                result = left == right;
                break;       
        }
        return result;
    }
    else
        return Char_vals[*Curpos++ - 97];
}