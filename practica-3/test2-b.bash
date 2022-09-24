mpic++ p2-b.cpp -o p2

for p in {1..16}
do
   echo "$a" >> p2-b.csv
   for i in {1..10}
   do
      mpirun -np $p ./p2
   done
done