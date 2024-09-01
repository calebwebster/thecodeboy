FROM alpine:latest
RUN apk add build-base
RUN apk add curl-dev=8.9.1-r1
COPY . /thecodeboy
WORKDIR /thecodeboy
RUN mkdir build
RUN gcc main.c -lcurl -o build/main

FROM curlimages/curl:8.9.1
COPY --from=0 /thecodeboy/build /thecodeboy/build
WORKDIR /thecodeboy/build
CMD ["./main"]
