FROM rust:latest

RUN apt-get update && apt-get install -y \
	gcc \
	nasm \
	libcriterion-dev \
	make

WORKDIR /app

COPY ./main.c ./Makefile ./libasm.h Cargo.lock Cargo.toml build.rs /app/
COPY ./src/ /app/src

ENTRYPOINT [ "make", "run-tests" ]
