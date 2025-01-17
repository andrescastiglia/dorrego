FROM acastiglia/yew:latest AS builder
WORKDIR /workspace
COPY . .
RUN trunk build --release

FROM nginx:alpine
COPY --from=builder /workspace/dist /usr/share/nginx/html
CMD ["nginx", "-g", "daemon off;"]
