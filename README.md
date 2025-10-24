# DASM - a preprocessor for assembly made in c++.

Here in DASM we added such things as...
IF, ELIF, ELSE, WHILE with &'s as whitespace!

example:
```asm
IF RCX > 10
&  mov rbx, -1
ELIF RCX < 1
&  mov rbx, 1
END
```

to compile run the following: 
```bash
g++ -std=c++17 -O2 -Wall -Wextra -o dasm dasm.cpp
```
