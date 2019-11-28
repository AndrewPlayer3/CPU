<instruction> ::= <opcode>
                | <opcode> <register>
                | <opcode> <register> "," <integer>
                | <opcode> <register> "," <pointer>
                | <opcode> <register> "," <string>
                | <opcode> <pointer>  "," <integer>
                | <opcode> <pointer>  "," <pointer>
                | <opcode> <pointer>  "," <string>

<label> ::= <string> ":"

<comment> ::= "#" <string>


mov register, register  :   0xE1..
mov register, integer   :   0xE0..
mov register, pointer   :   
mov register, string    :   0xE30.


