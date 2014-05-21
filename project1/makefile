all : asd

asd : asd.cc
	g++ -D _SUNOS asd.cc timer.cc main.cc -o asd 
	
del :
	rm asd
debug : asd.cc
	g++ -g -D _SUNOS asd.cc timer.cc main.cc -o asd_debug
	gdb asd_debug 
	

view:
	lynx /home/common/dyd/aisdi/liniowe/info/index.html
