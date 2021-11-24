all: main main2

main:
	@echo " Compile Os1 Parent...";
	
	gcc -I ./include -pthread -o build/os1_parent examples/main_os.c src/parent_processinput.c -lrt

main2:
	@echo " Compile Os1 Childs...";
	
	gcc -I ./include -pthread -o build/os1_child examples/child_os.c -lrt