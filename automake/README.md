# automake
Automake Compile On Write (ACOW)
# Usage
<p>This program automatically creates the binary executable from a given C source file after detecting any change in it over a time interval.</p>
# Install
chmod +x automake.sh
# Run (in foreground)
./automake.sh src dst cmd secs
<p><b>src</b> is the name of source C program</p>
<p><b>dst</b> is the name of output binary executable</p>
<p><b>cmd</b> is the make or any compiling tool command for compiling the source C program</p>
<p><b>secs</b> is the time interval to look for any change in the source C program</p>
<p>Automake works in foreground; one suggestion for debugging is that you could open two terminal windows. Run automake in one terminal and try editing your C file in the other one</p>
# Hello world example
## Sample command
<p>./automake.sh hello.c hello make 2</p>
<p>If there are any changes in <b>hello.c</b> within every period of <b>2 seconds</b>, the <b>hello</b> binary executable will be recompiled using <b>make</b> and then run</p>
## Sample output
<p>hello.c has been changned!</p>
<p>gcc  -o hello hello.c</p>
<p>Hello, World!</p>
# Extending the hello world example
<p>If you want to extend the hello.c example. Don't forget to make proper changes to the Makefile</p>
<p>One can pass arguments to automake by wrapping src or cmd with double quotes (e.g. "hello arg1 arg2 ...").</p>
