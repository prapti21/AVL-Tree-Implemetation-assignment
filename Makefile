all:
	g++ -o avltree main.cpp -I.
clean: 
	rm -f avltree
	rm -f output_file.txt