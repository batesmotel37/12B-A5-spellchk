# $Id: README,v 1.1 2014-03-13 18:16:06-07 - - $

username:khigh
name:Kenneth High

Program purpose: to accept a text file and check the contents of the text file, reporting if any words in it are spelled incorrectly.

How it works: program accepts a text file which acts as a dictionary, and a text file to be checked. The contents of the dictionary file are loaded into a hashtable. The contents of the second text file are then checked against the contents of this hashtable, any words present in the second file which are not in the table are reported as misspelled.

Runs in O(n) where n is the number of words (strings separated by whitespace, punctuation or newline characters) in both of the arguement text files.

all code except for methods load_dictionary, spellchk and main in spellchk.c & methods free_hashset, put_hashset, has_hashset, dump_hashset, and cluster_hashset in hashset.c taken directly from code provided by Prof. Mackey The methods metioned above were completed as per Prof. Mackey's instructions from STUB code provided by him (except for the methods main and spellchk in spellchk.c which were provided with semi-complete STUB code)