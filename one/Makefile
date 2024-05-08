CC=ccache g++
INC= -I/usr/include/mysql -I/usr/include/mysql++ 
LIBDIR= -L/usr/lib64/mysql -L/usr/local/lib

OSID := $(shell grep '^ID=' /etc/os-release | cut -f2- -d= | sed -e 's/\"//g')
ifeq ($(OSID), ubuntu)
LIB+=-lmariadb
INC+=-I/usr/include/mariadb
LIBDIR+=-L/usr/lib/x86_64-linux-gnu
else
ifeq ($(OSID), rocky)
LIB+=-lmysqlclient
LIBDIR+=-L/usr/include/mysql
else
ifeq ($(OSID), centos)
LIB+=-lmysqlclient
LIBDIR+=-L/usr/include/mysql
endif # centos
endif # rocky
endif # ubuntu

HOST=host.cpp

.PHONY: gogo

gogo:host.cpp
	$(CC) $(INC) -o a.out host.cpp $(LIBDIR) $(LIB) $(RUNTIME)