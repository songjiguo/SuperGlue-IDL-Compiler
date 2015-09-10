#!/usr/bin/sed -f

# The purpose of this script is to construct C code in string
# escape all special chars " and \ inside a string...
s/["\\]/\\&/g
# adds a "\ to the start of each line
s/^/\"/
# adds a \n"\ to the end of each line
s/$/\\n" \\/
#$s/\\n" //g

