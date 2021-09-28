#!/bin/bash

echo
echo

printf 'const char* html_template = ' \
&& cat $1 \
| tr -d '\r' \
| tr -d '\n' \
| sed 's/\"/\\\"/g' \
| sed 's/^/\"/' \
| sed 's/%/%%/g' \
| sed 's/%%s/%s/g' \
| sed 's/>\s*/>/g' \
&& printf '";'

echo
echo

#| sed 's/$/\\n\"/' \qqqq