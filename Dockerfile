FROM node:15.9-slim

RUN groupadd -r wordbrush && useradd --create-home -r -g wordbrush wordbrush

EXPOSE 3000 8080

ENV WORK_DIR=/wordbrush NODE_ENV=Production
WORKDIR $WORK_DIR

RUN apt-get update && \
    apt-get -qq -y upgrade && \
    apt-get -qq -y install make gcc zsh iproute2 && \
    apt-get -q -y autoremove && \
    npm install 2> /dev/null -g npm@latest typescript serve && \
    mkdir -p resources

USER wordbrush

COPY --chown=wordbrush:wordbrush compute compute
COPY --chown=wordbrush:wordbrush frontend frontend
COPY --chown=wordbrush:wordbrush dockerRunner.sh .

VOLUME $WORK_DIR/resources

RUN cd compute && \
    mkdir bin && \
    make && \
    cd ../frontend/server && \
    npm run build && \
    cd ../ui && \
    npm run build

ENTRYPOINT ["./dockerRunner.sh"]
