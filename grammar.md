
Sunflower program (or a single REPL entry):
program → declaration* EOF ;
## Declarations
declaration → funDecl
 | varDecl
 | statement ;
classDecl → "class" IDENTIFIER ( "<" IDENTIFIER )?
 "{" function* "}" ;
funDecl → "fn" function ;
varDecl → "let" IDENTIFIER ( "=" expression )? ";" ;

## Statements
statement → exprStmt
 | forStmt
 | ifStmt
 | printStmt
 | returnStmt
 | whileStmt
 | block ;

exprStmt → expression ";" ;
forStmt → "for" "(" ( varDecl | exprStmt | ";" )
 expression? ";"
 expression? ")" statement ;
ifStmt → "->" "(" expression ")" statement
 ( "else" statement )? ;
printStmt → "pout" "(" expression ")" ";" ;
returnStmt → "return" expression? ";" ;
whileStmt → "while" "(" expression ")" statement ;
block → "{" declaration* "}" ;
Note that block is a statement rule, but is also used as a nonterminal in a
couple of other rules for things like function bodies.


## Expressions
expression → assignment ;
assignment → ( call "." )? IDENTIFIER "=" assignment
 | logic_or ;
logic_or → logic_and ( "or" logic_and )* ;
logic_and → equality ( "and" equality )* ;
equality → comparison ( ( "!=" | "==" ) comparison )* ;
comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term → factor ( ( "-" | "+" ) factor )* ;
factor → unary ( ( "/" | "*" ) unary )* ;
unary → ( "!" | "-" ) unary | call ;
call → primary ( "(" arguments? ")" | "." IDENTIFIER )* ;
primary → "true" | "false" | "null" ;
 | NUMBER | STRING | IDENTIFIER | "(" expression ")";

## Utility Rules

function → IDENTIFIER "(" parameters? ")" block ;
parameters → IDENTIFIER ( "," IDENTIFIER )* ;
arguments → expression ( "," expression )* ;

## lexical grammar

NUMBER → DIGIT+ ( "." DIGIT+ )? ;
STRING → "\"" <any char except "\"">* "\"" ;
IDENTIFIER → ALPHA ( ALPHA | DIGIT )* ;
ALPHA → "a" ... "z" | "A" ... "Z" | "_" ;
DIGIT → "0" ... "9" ;