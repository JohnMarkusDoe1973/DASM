# DASM - a C++ preprocessor for assembly.

```
  _____           _____ __  __ 
 |  __ \   /\    / ____|  \/  |
 | |  | | /  \  | (___ | \  / |
 | |  | |/ /\ \  \___ \| |\/| |
 | |__| / ____ \ ____) | |  | |
 |_____/_/    \_\_____/|_|  |_|

```
DASM (Doe'ian Assembly) is a simple preprocessor made in c++ that inputs .DASM files and outputs .ASM files!
It plays the nicest with NASM and it's heavily biased towards it. THIS IS NOT dasm FOR 6502 AND IS IN NO WAY RELATED TO IT!

Q: Why use DASM?
A: Because we added IF, ELIF, ELSE, WHILE with & as depth indicator! 
Q: Why &?
A: It's fancy. And simple.

TO RUN:
build DASM.cpp or get the .exe for windows (tested on windows 8.1) and run the following in CMD:
```bash
dasm.exe INPUTFILE.dasm
```
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
