FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y make gcc

ENV WORK_DIR=/tmp/code/

RUN mkdir -p $WORK_DIR

COPY dockerRunner.sh $WORK_DIR/dockerRunner.sh
COPY Makefile $WORK_DIR/Makefile
COPY src $WORK_DIR/src

WORKDIR $WORK_DIR

ENTRYPOINT ["./dockerRunner.sh"]
