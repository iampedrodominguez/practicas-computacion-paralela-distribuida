mpic++ p2.cpp -o p2

for p in {5..16}
do
   echo "$a" >> p1.csv
   for i in {1..10}
   do
      mpirun -np $p ./p1
   done
done