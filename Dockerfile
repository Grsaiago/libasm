FROM debian:latest

RUN apt-get update && apt-get install -y \
	gcc \
	nasm \
	libcriterion-dev \
	make

WORKDIR /app

COPY ./main.c ./Makefile ./my_asm.h /app/
COPY ./src/ /app/src

ENTRYPOINT [ "make", "run-tests" ]
