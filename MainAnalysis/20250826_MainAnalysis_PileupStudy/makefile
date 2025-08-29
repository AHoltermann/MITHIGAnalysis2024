default: ExecuteChargedHadrondNdpt

ExecuteChargedHadrondNdpt: ChargedHadrondNdpT.cpp
	g++ ChargedHadrondNdpT.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include \
	  -I./include \
		-o ExecuteChargedHadrondNdpt `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

ExecutePlotter: plotter1.cpp	
	g++ plotter1.cpp -O3 -I. -I$(ProjectBase)/CommonCode/include \
	  -I./include \
		-o ExecutePlotter `root-config --cflags --glibs` \
		$(ProjectBase)/CommonCode/library/Messenger.o

TestRun: ExecuteChargedHadrondNdpt
	./ExecuteChargedHadrondNdpt --Input skimexample.root --Output chargedhadronRAA.root

TestPlotter: ExecutePlotter
	./ExecutePlotter
