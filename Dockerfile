FROM node:16.13-slim

EXPOSE 3000 8080

ENV WORK_DIR=/wordbrush NODE_ENV=Production BACKEND_ADDRESS=localhost
WORKDIR $WORK_DIR

RUN groupadd -r wordbrush && useradd --create-home -r -g wordbrush wordbrush && \
    apt-get update && \
    apt-get -qq -y upgrade && \
    apt-get -qq -y install make gcc zsh iproute2 && \
    apt-get -q -y autoremove && \
    rm -rf /var/lib/apt/lists/* && \
    npm install 2> /dev/null -g npm@latest typescript serve && \
    chown wordbrush:wordbrush $WORK_DIR

USER wordbrush

COPY --chown=wordbrush:wordbrush compute compute
COPY --chown=wordbrush:wordbrush frontend frontend
COPY --chown=wordbrush:wordbrush dockerRunner.sh .

RUN mkdir -p resources && \
    cd compute && \
    mkdir bin && \
    make && \
    cd ../frontend/server && \
    npm run build && \
    cd ../ui && \
    npm run build

VOLUME $WORK_DIR/resources

ENTRYPOINT ["./dockerRunner.sh"]
