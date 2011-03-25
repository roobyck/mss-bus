
.PHONY : all clean

all:
	cd ./doc; $(MAKE)
	cd ./src; $(MAKE) INCLUDE=$(PWD)/include

clean:
	cd ./doc; $(MAKE) clean
	cd ./src; $(MAKE) clean

