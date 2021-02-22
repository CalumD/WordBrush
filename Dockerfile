FROM node:14-alpine

EXPOSE 3000
EXPOSE 8080

RUN apk update && \
    apk add make gcc libc-dev

ENV WORK_DIR=/wordbrush
RUN mkdir -p $WORK_DIR/resources
WORKDIR $WORK_DIR

COPY . $WORK_DIR

CMD /bin/sh