for i in `cat log/badFileList.txt`; do ls | grep $i | sed 's/^/rm /'; done
