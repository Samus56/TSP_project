#stage 1
FROM gcc AS compile_stage

WORKDIR home/my_c_app

COPY main.c .

RUN ["gcc","main.c","-o","compiled","-lm"]

#stage 2
FROM ubuntu

WORKDIR home/my_c_app

COPY --from=compile_stage home/my_c_app/compiled .

ENTRYPOINT ["./compiled"]