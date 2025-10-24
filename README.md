# DASM - a preprocessor for assembly made in c++.

Here in DASM we added such things as...
IF, ELIF, ELSE, WHILE with &'s as whitespace!

example:
`IF RCX > 10
&  mov rbx, -1
ELIF RCX < 1
&  mov rbx, 1
END`
