mpic++ p2-a.cpp -o p2

for p in {2..16}
do
   echo "$a" >> p2-a.csv
   for i in {1..10}
   do
      mpirun -np $p ./p2
   done
done