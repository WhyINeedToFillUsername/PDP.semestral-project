#!/bin/sh

g++ -O3 -fopenmp -std=c++11 main.cpp TaskInstance.cpp TaskInstance.h -o sem_v1

rm output/*

printf "\n01:\n"
./sem_v1 kralovna01.txt > output/01.txt
diff "output/01.txt" "reference/01.txt"

printf "\n02:\n"
./sem_v1 kralovna02.txt > output/02.txt
diff "output/02.txt" "reference/02.txt"

printf "\n03:\n"
./sem_v1 kralovna03.txt > output/03.txt
diff "output/03.txt" "reference/03.txt"

printf "\n04:\n"
./sem_v1 kralovna04.txt > output/04.txt
diff "output/04.txt" "reference/04.txt"

printf "\n05:\n"
./sem_v1 kralovna05.txt > output/05.txt
diff "output/05.txt" "reference/05.txt"

printf "\n06:\n"
./sem_v1 kralovna06.txt > output/06.txt
diff "output/06.txt" "reference/06.txt"

printf "\n07:\n"
./sem_v1 kralovna07.txt > output/07.txt
diff "output/07.txt" "reference/07.txt"

printf "\n08:\n"
./sem_v1 kralovna08.txt > output/08.txt
diff "output/08.txt" "reference/08.txt"

printf "\n09:\n"
./sem_v1 kralovna09.txt > output/09.txt
diff "output/09.txt" "reference/09.txt"

printf "\n10:\n"
./sem_v1 kralovna10.txt > output/10.txt
diff "output/10.txt" "reference/10.txt"

printf "\n11:\n"
./sem_v1 kralovna11.txt > output/11.txt
diff "output/11.txt" "reference/11.txt"

printf "\n12:\n"
./sem_v1 kralovna12.txt > output/12.txt
diff "output/12.txt" "reference/12.txt"
