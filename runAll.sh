#!/bin/sh

g++ -O3 -fopenmp -std=c++11 main.cpp TaskInstance.cpp TaskInstance.h -o sem_v1

rm output/*

echo "01:"
./sem_v1 kralovna01.txt > output/01.txt
diff "output/01.txt" "reference/01.txt"

echo "02:"
./sem_v1 kralovna02.txt > output/02.txt
diff "output/02.txt" "reference/02.txt"

echo "03:"
./sem_v1 kralovna03.txt > output/03.txt
diff "output/03.txt" "reference/03.txt"

echo "04:"
./sem_v1 kralovna04.txt > output/04.txt
diff "output/04.txt" "reference/04.txt"

echo "05:"
./sem_v1 kralovna05.txt > output/05.txt
diff "output/05.txt" "reference/05.txt"

echo "06:"
./sem_v1 kralovna06.txt > output/06.txt
diff "output/06.txt" "reference/06.txt"

echo "07:"
./sem_v1 kralovna07.txt > output/07.txt
diff "output/07.txt" "reference/07.txt"

echo "08:"
./sem_v1 kralovna08.txt > output/08.txt
diff "output/08.txt" "reference/08.txt"

echo "09:"
./sem_v1 kralovna09.txt > output/09.txt
diff "output/09.txt" "reference/09.txt"

echo "10:"
./sem_v1 kralovna10.txt > output/10.txt
diff "output/10.txt" "reference/10.txt"

echo "11:"
./sem_v1 kralovna11.txt > output/11.txt
diff "output/11.txt" "reference/11.txt"

echo "12:"
./sem_v1 kralovna12.txt > output/12.txt
diff "output/12.txt" "reference/12.txt"
