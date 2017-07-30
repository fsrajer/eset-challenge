# Eset challenge

You can find the project here:
https://join.eset.com/en/challenges/c-developer

## Functionality
### Input
The application will receive two command line parameters. The first parameter is a path to a file or folder, while the second parameter is the string to be searched for (maximum length of 128 characters).

### Output
Print the output to stdout lines in format: \<file\>(\<position\>): \<prefix\>…\<suffix\>.

\<file\>: Name of a file containing the string

\<position\>: Offset within the file where the string started (zero based).

\<prefix\>: Contains three characters in front of the match

\<suffix\>: Contains three characters after the match.

## Compatibility
The code is written using cmake so you will need cmake to build your solution. Windows is required to run this as io functions are written using windows.h. The project was tested with Visual Studio 2017 Community edition but it should work also with other versions which support C++11.

## A note on multithreading
I ran a quick benchmark with 1.8GB file to compare single and multithreaded solutions. The multithreaded solution proved to be 15% faster. The benchmark was done on a single machine and the runtimes were averaged over 10 runs. A more extensive benchmark on several machines would be necessary in order to determine which version is faster.
