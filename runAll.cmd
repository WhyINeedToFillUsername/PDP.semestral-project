g++ -O3 -fopenmp main.cpp TaskInstance.cpp TaskInstance.h -o cv1.exe

del /q output\*

cv1.exe kralovna01.txt > output/01.txt
FC "output\01.txt" "reference\01.txt"
cv1.exe kralovna02.txt > output/02.txt
FC "output\02.txt" "reference\02.txt"
cv1.exe kralovna03.txt > output/03.txt
FC "output\03.txt" "reference\03.txt"
cv1.exe kralovna04.txt > output/04.txt
FC "output\04.txt" "reference\04.txt"
cv1.exe kralovna05.txt > output/05.txt
FC "output\05.txt" "reference\05.txt"
cv1.exe kralovna06.txt > output/06.txt
FC "output\06.txt" "reference\06.txt"
cv1.exe kralovna07.txt > output/07.txt
FC "output\07.txt" "reference\07.txt"
cv1.exe kralovna08.txt > output/08.txt
FC "output\08.txt" "reference\08.txt"
cv1.exe kralovna09.txt > output/09.txt
FC "output\09.txt" "reference\09.txt"
cv1.exe kralovna10.txt > output/10.txt
FC "output\10.txt" "reference\10.txt"
cv1.exe kralovna11.txt > output/11.txt
FC "output\11.txt" "reference\11.txt"
cv1.exe kralovna12.txt > output/12.txt
FC "output\12.txt" "reference\12.txt"
