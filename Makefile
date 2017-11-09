all:learn

learn: learn.h learn.c
		gcc -Wall -Werror -fsanitize=address learn.c -o learn
		
clean:
		rm -rf learn
