FROM alpine:latest

WORKDIR /app
COPY ./*c /app/
COPY ./Makefile /app/Makefile

RUN apk add --no-cache \
    build-base \
    make \
    gcc
    

CMD ["/bin/sh"]
