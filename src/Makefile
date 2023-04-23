objs=parser.o lexer.o ast.o display.o
CC=gcc

parser:$(objs)
	$(CC) -o parser ${objs}
%.o:%.c
	$(CC) -c $<
clean:
	rm parser $(objs) parser.c lexer.c parser.h -f
