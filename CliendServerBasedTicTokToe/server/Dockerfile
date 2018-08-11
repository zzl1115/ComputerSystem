FROM ubuntu:16.04

RUN apt-get update && apt-get install -y \
  libmicrohttpd-dev \
  libjansson-dev \
  libcurl4-gnutls-dev \
  libgnutls28-dev \
  libgcrypt20-dev 

# Copy the server C binary and the JSON library into the container
COPY ./ttt_server /ttt_server

COPY ./libs/libjansson.so.4 /usr/lib/libjansson.so.4
COPY ./libs/libulfius.so.2.3 /usr/lib/libulfius.so.2.3
#COPY ./libs/libmicrohttpd.so.10.34.0 /usr/lib/libmicrohttpd.so.10.34.0
#COPY ./libs/libmicrohttpd.so.10.34.0 /usr/lib/libmicrohttpd.so.10
#COPY ./libs/libmicrohttpd.a /usr/lib/libmicrohttpd.a
#COPY ./libs/libyder.so.1.3.3 /usr/lib/libyder.so.1.3.3
COPY ./libs/libyder.so.1.3.3 /usr/lib/libyder.so.1.3
COPY ./libs/liborcania.so.1.2.3 /usr/lib/liborcania.so.1.2

# Expose the port that the server will listen on
EXPOSE 1234

# Set the LD_LIBRARY_PATH so that the server binary will find the JSON library
ENV LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/usr/lib

CMD ["/ttt_server"]
