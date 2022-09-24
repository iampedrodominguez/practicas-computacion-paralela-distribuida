mpic++ p1.cpp -o p1

for p in {5..16}
do
   for i in {1..10}
   do
      mpirun -np $p ./p1
   done
done
