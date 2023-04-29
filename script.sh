gcc -c adjusttree.c
gcc -c nodesplit.c
gcc -c insert.c 
gcc -c print.c 
gcc -c dsa_project.c
gcc -o exe adjusttree.o nodesplit.o insert.o print.o dsa_project.o
./exe

#To run the file just type sh script.sh on the terminal if you're using a Mac or just copy paste the above code if you're using Windows