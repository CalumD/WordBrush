FROM node:15.9-slim

RUN groupadd -r -g 999 wordbrush && useradd -r -g wordbrush -u 999 wordbrush

EXPOSE 3000 8080

ENV WORK_DIR=/wordbrush NODE_ENV=Production
WORKDIR $WORK_DIR

RUN apt-get update && \
    apt-get -qq -y upgrade && \
    apt-get -qq -y install make gcc zsh iproute2 && \
    apt-get -q -y autoremove && \
    npm install 2> /dev/null -g npm@latest typescript serve && \
    mkdir -p resources

COPY compute compute
COPY frontend frontend

RUN chown -R wordbrush:wordbrush $WORK_DIR

VOLUME $WORK_DIR/resources

USER wordbrush

#RUN cd compute && \
#    mkdir bin && \
#    make && \
#    cd ../frontend/server && \
#    npm run build && \
#    cd ../ui && \
#    npm run build
#
#COPY dockerRunner.sh .
#
#ENTRYPOINT ["./dockerRunner.sh"]

CMD /bin/zsh