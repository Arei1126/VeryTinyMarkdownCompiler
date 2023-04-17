vtmc: vtmc.c
	gcc vtmc.c -o vtmc

install: vtmc
	install vtmc /usr/local/bin
