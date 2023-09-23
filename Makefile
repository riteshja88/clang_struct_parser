all:
	g++ -fsanitize=address -static-libasan  -g clang_struct_parser.c -I /usr/lib/llvm-6.0/include/ -L/usr/lib/llvm-6.0/lib/ -lclang -lLLVM 
	./a.out struct.h

clean:
	rm -f a.out

ast:
	clang -Xclang -ast-dump -fsyntax-only struct.h
