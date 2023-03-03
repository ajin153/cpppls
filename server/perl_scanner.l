%option noyywrap nodefault yylineno

%{
#include <stdio.h>

void init_scanner();
void print_current_symbol();
void refresh_current_symbol();
void clear_current_symbol();

typedef enum {
    PACKAGE = 256,
    FUNCTION
} TokenType;

struct symbol {
    char *name;
    int kind; /* 这里的kind使用lsp协议中枚举类型SymbolKind对应的值 */
    int lineno;
    int character; /* 这里对应lsp协议中Range类型的character，返回匹配行的长度*/
} *current_symbol = 0;

struct symbol *current_symbol;
%}

/* Definitions */
%s PKG
%s FUNC

%%

"package"\x20+ {
    BEGIN PKG;
    current_symbol->kind = 4;
    current_symbol->character += yyleng;
}

"sub"\x20+ {
    BEGIN FUNC;
    current_symbol->kind = 12;
    current_symbol->character += yyleng;
}

<PKG>[_a-zA-Z0-9:]+ {
    BEGIN INITIAL;
    current_symbol->name = yytext;
    current_symbol->lineno = yylineno;
    current_symbol->character += yyleng;
    return PACKAGE;
}

<FUNC>[_a-zA-Z0-9]+ {
    BEGIN INITIAL;
    current_symbol->name = yytext;
    current_symbol->lineno = yylineno;
    current_symbol->character += yyleng;
    return FUNCTION;
}

\r\n|\n { BEGIN INITIAL; /* yylineno++ auto */ }

. { BEGIN INITIAL; /* ignore */ }

<<EOF>> { return 0; }

%%

int main(int argc, char *argv[]) {
    int token;
    if (argc < 2) {
        fprintf(stderr, "need filename!\n");
        return 1;
    }
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        fprintf(stderr, "file open failed!\n");
        return 2;
    }
    init_scanner();
    while (yylex()) {
        print_current_symbol();
        refresh_current_symbol();
    }
    clear_current_symbol();
    fclose(yyin);
    return 0;
}

void init_scanner() {
    yylineno = 1;
    struct symbol *cur = malloc(sizeof(struct symbol));
    if (!cur) {
        fprintf(stderr, "malloc failed!\n");
        exit(3);
    }
    cur->name = "";
    cur->kind = -1;
    cur->lineno = -1;
    cur->character = 0;
    current_symbol = cur;
}

void print_current_symbol() {
    printf("%s,%d,%d,%d\n", current_symbol->name, current_symbol->kind,
        current_symbol->lineno, current_symbol->character);
}

void refresh_current_symbol() {
    current_symbol->name = "";
    current_symbol->kind = -1;
    current_symbol->lineno = -1;
    current_symbol->character = 0;
}

void clear_current_symbol() {
    free(current_symbol);
}