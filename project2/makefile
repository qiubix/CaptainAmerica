all : asd

asd : asd.cc test.cc
	g++ -O2 -D asd.cc test.cc timer.cc -o asd 
	
del :
	rm asd
	rm asd3
debug :
	g++ -g -D asd.cc test.cc timer.cc -o asd_debug 
	gdb asd_debug

view:
	lynx /home/common/dyd/aisdi/hash/info/index.html
