## Description

A program that parses a book from Project Gutenburg and provides statistics based on it.

The program will provide the most common words from the text passed to the program,
however a CSV file can also be passed to the program to exclude any desired words from
the statistics, essentially blacklisting them.

## Usage

run with "Python SINEP_1.py"
or "Python3 SINEP_1.py"

#Valid Commands
 parse <dir path>  Parse the text in the files within the provided directory"
 stat <file path>  File to a csv 'banlist' print stats of parsed files"

#Examples
Python SINEP_1.py parse directory_of_books
Python3 SINEP_1.py stat bannedwords.csv

#Note
Parse command must be run before the stats command is used in order to preprocess books
