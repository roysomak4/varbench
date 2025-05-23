# Dockerfile for varbench
# Image: ghcr.io/cchmc-research-mgps/varbench:<tag>

FROM ubuntu:22.04 as builder

ENV VARBENCH_VER=0.5.0

# install mosdepth
RUN apt update && apt install -y wget && \
    wget https://github.com/cchmc-research-mgps/ngs_benchmarking_tools/releases/download/v${VARBENCH_VER}/varbench -O /usr/local/bin/varbench && \
    chmod a+x /usr/local/bin/varbench

# create minimal docker image
FROM ubuntu:22.04

LABEL maintainer="Somak Roy<somak.roy@cchmc.org>" \
    function="Docker image with mosdepth" \
    source="https://github.com/orgs/cchmc-research-mgps/packages" \
    org.opencontainers.image.source="https://github.com/cchmc-research-mgps/ngs_benchmarking_tools"

# set non root user to match the service account for GLIMPS app
ENV NON_ROOT_USER="srv-glimps" \
    NON_ROOT_UID="87417"

COPY --from=builder /usr/local/bin/varbench /usr/local/bin/varbench

RUN adduser --uid $NON_ROOT_UID --gecos '' --disabled-password $NON_ROOT_USER

USER ${NON_ROOT_USER}
WORKDIR /home/${NON_ROOT_USER}
