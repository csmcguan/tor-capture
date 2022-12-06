FROM gcc
COPY tor.patch .

RUN apt update          \
    && apt upgrade -y   \
    && apt install -y python3-pip docbook-xsl docbook-xml xmlto

RUN pip3 install asciidoc

# build tor with patch
RUN git clone --branch release-0.4.5 https://github.com/torproject/tor.git
RUN mv tor.patch tor && cd tor
WORKDIR /tor
RUN git apply tor.patch
RUN sh autogen.sh && ./configure && make && make install
RUN useradd -rm tor

RUN cp /usr/local/etc/tor/torrc.sample /usr/local/etc/tor/torrc \
    && echo "SocksPort 0.0.0.0:9050" > /usr/local/etc/tor/torrc

EXPOSE 9050
USER tor
CMD ["tor"]
