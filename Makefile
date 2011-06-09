
.PHONY : all clean

all:
	cd ./doc; $(MAKE)
ifdef DEBUG
	cd ./src; $(MAKE) INCLUDE=$(PWD)/include
else
	cd ./src; $(MAKE) INCLUDE=$(PWD)/include DEBUG=$(DEBUG)
endif

clean:
	cd ./doc; $(MAKE) clean
	cd ./src; $(MAKE) clean

