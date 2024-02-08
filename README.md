# 1921 Modularity Comparison

Here is (some) of the code for last year's programming project.

This was to do with some image file handling and compression - I've included a single main() file: `ebfEcho.c` and the modules needed to make this work (split into .c and .h files - more on that next week!).

Have a go at tracing the program execution through - and review the code as you do!

Pop your comments on Vevox.

## Makefile + unittestexample.c

I've included a very basic example of a unit test script using vanilla 'assert()' - just testing one of the simpler functions in this program. The whole program could be unit tested, but this requires quite a lot of set up mainly because it handles more complicated files.

You can also see a makefile with 2 different sets of rules - this allows me to compile either my executable or my test script (or both at the same time).