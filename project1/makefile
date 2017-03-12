all : asd

asd : asd.cc
	g++ -D asd.cc timer.cc main.cc -o asd 
	
del :
	rm asd
debug : asd.cc
	g++ -g -D asd.cc timer.cc main.cc -o asd_debug
	gdb asd_debug 
	
