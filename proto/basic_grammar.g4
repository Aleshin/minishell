grammar CommandLine;

// Main rule for parsing the entire command line input
commandLine
    : command (pipe command)* EOF
    ;

// Definition of a command with optional arguments
command
    : executable arguments?
    ;

// Executable file or command
executable
    : WORD
    | DOUBLE_QUOTED_STRING
    ;

// Command arguments
arguments
    : argument+
    ;

// Single argument which could be a word, a double-quoted string, or a single-quoted string
argument
    : WORD
    | DOUBLE_QUOTED_STRING
    ;

// Token definition for executables and arguments
WORD
    : [a-zA-Z0-9_/.-]+
    ;

// Strings enclosed in double quotes
DOUBLE_QUOTED_STRING
    : '"' ~["]* '"'
    ;

// Pipe symbol to chain commands
pipe
    : '|'
    ;

// Skip spaces and tabs
WS
    : [ \t\r\n]+ -> skip
    ;
