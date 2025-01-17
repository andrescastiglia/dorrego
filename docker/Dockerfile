FROM rust:alpine
RUN apk add build-base musl-dev libressl-dev 
RUN rustup target add wasm32-unknown-unknown
RUN cargo install trunk