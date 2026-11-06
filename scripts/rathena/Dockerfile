FROM alpine:3.23 AS build
RUN apk add --no-cache git cmake make gcc g++ zlib-dev mariadb-dev pcre-dev linux-headers bash
ARG RATHENA_REF=master
ARG PACKETVER=20260219
RUN git clone --depth 1 --branch "${RATHENA_REF}" https://github.com/rathena/rathena.git /rathena
WORKDIR /rathena
RUN ./configure --enable-packetver=${PACKETVER} && make -j"$(nproc)" server

FROM alpine:3.23
RUN apk add --no-cache libstdc++ zlib mariadb-connector-c pcre netcat-openbsd \
 && wget -q https://raw.githubusercontent.com/eficode/wait-for/v2.2.4/wait-for -O /bin/wait-for \
 && chmod +x /bin/wait-for
COPY --from=build /rathena /rathena
WORKDIR /rathena
