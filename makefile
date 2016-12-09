semaphone: control.c
	gcc control.c -g -o control
	gcc client.c -g -o client
clean:
	rm control
