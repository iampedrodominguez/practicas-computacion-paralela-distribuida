mpic++ p1.cpp -o p1

for i in {1..2}
do
   mpirun -np 2 ./p1
done