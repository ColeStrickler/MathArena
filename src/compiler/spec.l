%{
#include <string>
#include <limits.h>
#include <iostream>
#include <algorithm> // For std::count
#include <cstdlib>   // For std::exit

/* Get our custom yyFlexScanner subclass */
#include "eqscanner.h"
#undef  YY_DECL
#define YY_DECL int EquationScanner::yylex( Token** out)

/* define yyterminate as this instead of NULL */
#define yyterminate() return( TokenType::END )

/* Exclude unistd.h for Visual Studio compatability. */
#define YY_NO_UNISTD_H

/* typedef to make the reference to token kinds shorter */
%}


%option debug
%option nodefault
%option yyclass="EquationScanner"
%option noyywrap
%option c++



NUMBER [0-9]+(\.[0-9]+)?
VARIABLE [xyz]
LPAREN \(
RPAREN \)
CROSS \+
DASH \-
STAR \*
SLASH \/
POW ^
WHITESPACE " "

%%



{NUMBER} {
    printf("NUMBER\n");
    std::string text(yytext);
    *out = new Token(TokenType::NUMBER, text, m_colNum, m_colNum + text.size());
    m_colNum += strlen(yytext);
    return TokenType::NUMBER;
}


. {
    m_colNum++;
    out = nullptr;
    return TokenType::ERROR;
}


%%
