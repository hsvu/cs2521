#!/bin/sh

for t in 01 02 03 04 05 06 07 08 09 10 11 12 ... and the rest of your test files
do
	echo === Test $t ===
	sort -n < tests/$t > tests/$t.expected
	./usel < tests/$t > tests/$t.observed
	diff tests/$t.expected tests/$t.observed
done
rm tests/*.expected tests/*.observed./
