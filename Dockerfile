FROM alpine:3.14
RUN mkdir thecodeboy
RUN apk add --no-cache build-base
RUN apk add --no-cache curl-dev
COPY ./main.c /thecodeboy
RUN gcc /thecodeboy/main.c -lcurl -o thecodeboy/main
WORKDIR /thecodeboy
ENTRYPOINT ["./main"]
