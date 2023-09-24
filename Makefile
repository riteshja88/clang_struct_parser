all:
	g++ -fsanitize=address -static-libasan  -g clang_struct_parser.c -I. -I /usr/lib/llvm-14/include/ -L/usr/lib/llvm-14/lib/ -lclang -lLLVM 
	gcc -c struct.h -Wno-attributes
	./a.out struct.h

clean:
	rm -f a.out struct.h.gch

ast:
	clang -Xclang -ast-dump -fsyntax-only struct.h


docker_new:
	docker  run --rm -v /home/ritesh:/root --name clang -it ubuntu:22.04

docker_setup:
	apt update -y
	apt install -y gcc g++ clang git make libclang-dev

#docker_terminal:
#	docker container exec -it clang apt install -y gcc g++ /bin/bash


libpmjson:
	gcc -c -fsanitize=address -static-libasan -o libpmjson.o libpmjson.c -I./
