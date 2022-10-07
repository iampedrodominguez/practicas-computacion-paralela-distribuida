g++ -fopenmp p2.cpp -o p2

for p in 1 2 4 8 16 32 64
do
   echo "$a" >> p2.csv
   for i in {1..10}
   do
      ./p2 $p
   done
done