# Local
```bash
trunk serve
```

# Docker

## Container
See [README](docker/README.md) file

## Build
```bash
docker build -t dorrego .
```

## Run
```bash
docker run -p 8080:80 dorrego
```

## Push
```bash
docker tag dorrego acastiglia/dorrego:latest
docker push acastiglia/dorrego:latest
```