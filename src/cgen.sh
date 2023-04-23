#!/bin/bash
# 由flex,bison生成lexer.c,parser.c和parser.h文件
# 该过程在评测脚本grading[1-2].sh评测前执行

bison -d parser.y -o parser.c
flex -o lexer.c lexer.l 