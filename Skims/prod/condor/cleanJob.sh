../../scripts/listDuplicates.sh . | sort | uniq > log/duplicates.txt
cat log/duplicates.txt
mkdir -p bad
for i in `find . -size -10 | grep trkhists.*root$`; do mv $i bad/; done
