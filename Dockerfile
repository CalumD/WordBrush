FROM node:15.9-slim

EXPOSE 3000
EXPOSE 8080

RUN apt-get update && \
    apt-get -q -y upgrade && \
    apt-get -q -y install make gcc zsh && \
    apt-get -q -y autoremove && \
    npm install -g npm typescript serve


ENV WORK_DIR=/wordbrush
WORKDIR $WORK_DIR
RUN mkdir -p $WORK_DIR/resources


VOLUME ["$WORDK_DIR/resources"]

COPY . .

RUN cd compute && \
    mkdir bin && \
    make && \

    cd ../frontend/server && \
    npm run production && \

    cd ../ui && \
    npm run production

CMD /bin/zsh
