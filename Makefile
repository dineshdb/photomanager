CC = g++ -Wall 
SRC=src
BUILD_DIR=build
BUILD_ENV=debug

all: daemon camera manager docs

prepare:
	sudo apt install qtcreator

prepare-dev: prepare
	sudo apt install graphviz libopencv-dev libboost-dev mscgen
	
daemon:
	$(CC) -c $(SRC)/daemon/daemon.cc -o $(BUILD_DIR)/$(BUILD_ENV)/daemon.o

camera:

manager:

install: daemon camera manager

clean: clean-docs
	rm -rf *.o

clean-docs:
	rm -rf docs/

docs:
	doxygen doxy.config
