%token WORD NEWLINE IO_NUMBER
%token AND_IF OR_IF        /* && || */
%token DLESS DGREAT        /* << >> */
%token LESS GREAT          /* <  >  */

%start program
%%

program
    : line
    ;

line
    : and_or
    | line '|' and_or                    /* pipeline */
    | line AND_IF and_or                 /* && */
    | line OR_IF and_or                  /* || */
    | line NEWLINE                       /* multiple commands separated by newline */
    ;

and_or
    : command
    | subshell
    ;

subshell
    : '(' line ')'
    ;

command
    : simple_command
    ;

simple_command
    : cmd_prefix cmd_word cmd_suffix
    | cmd_prefix cmd_word
    | cmd_prefix
    | cmd_word cmd_suffix
    | cmd_word
    ;

cmd_prefix
    : io_redirect
    | cmd_prefix io_redirect
    ;

cmd_suffix
    : io_redirect
    | cmd_suffix io_redirect
    | WORD
    | cmd_suffix WORD
    ;

cmd_word
    : WORD
    ;

io_redirect
    : io_file
    | IO_NUMBER io_file
    | io_here
    | IO_NUMBER io_here
    ;

io_file
    : LESS filename
    | GREAT filename
    | DGREAT filename
    ;

io_here
    : DLESS here_end
    ;

filename
    : WORD
    ;

here_end
    : WORD
    ;

%%
