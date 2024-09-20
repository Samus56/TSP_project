#stage 1
FROM gcc AS compile_stage

WORKDIR home/progetto/src/

COPY src/ .

RUN ["gcc","main.c","function.c","-o","compiled","-lm"]

#stage 2
FROM ubuntu

WORKDIR home/progetto/src/

COPY --from=compile_stage home/progetto/src/compiled .

ENTRYPOINT ["./compiled"]