#stage 1
FROM gcc AS compile_stage

WORKDIR home/progetto

COPY main.c .

RUN ["gcc","main.c","-o","compiled","-lm"]

#stage 2
FROM ubuntu

WORKDIR home/progetto

COPY --from=compile_stage home/progetto/compiled .

ENTRYPOINT ["./compiled"]