grammar CommandLine;

// Main rule for parsing the entire command line input
commandLine
	: command ('|' command)* EOF
	;

// Definition of a command with optional arguments
command
	: executable arguments?
	;

// Executable file or command
executable
	: WORD
	| SINGLE_QUOTED_STRING
	| DOUBLE_QUOTED_STRING
	;

// Command arguments
arguments
	: argument+
	;

// Single argument which could be a word, a double-quoted string, or a single-quoted string
argument
	: WORD
	| SINGLE_QUOTED_STRING
	| DOUBLE_QUOTED_STRING
	;

// Token definition for executables and arguments
WORD
	: [a-zA-Z0-9_/.-]+
	;

// Strings enclosed in single quotes
SINGLE_QUOTED_STRING
	: '\'' ~[']* '\''
	;

// Strings enclosed in double quotes
DOUBLE_QUOTED_STRING
	: '"' ~["]* '"'
	;

// Skip spaces and tabs
WS
	: [ \t\r\n]+ -> skip
	;
