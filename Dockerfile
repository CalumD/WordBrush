FROM node:14-alpine

RUN apk update && \
    apk add make gcc libc-dev

EXPOSE 3000

ENV WORK_DIR=/tmp/code/
RUN mkdir -p $WORK_DIR
RUN mkdir -p $WORK_DIR/resources
WORKDIR $WORK_DIR


COPY dockerRunner.sh $WORK_DIR/dockerRunner.sh
COPY compute $WORK_DIR/compute
COPY frontend/src $WORK_DIR/frontend/src
COPY frontend/package.json $WORK_DIR/frontend/src/package.json

ENTRYPOINT ["./dockerRunner.sh"]
