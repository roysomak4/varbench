ARG VARIANT=ubuntu-24.04
FROM mcr.microsoft.com/devcontainers/base:${VARIANT}
USER root

# Install needed packages. Use a separate RUN statement to add your own dependencies.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install build-essential \
    cmake \
    cppcheck \
    valgrind \
    clang-17 \
    clang-format-17 \
    clang-tidy-17 \
    lldb \
    llvm \
    gdb \
    git \
    wget \
    curl \
    ccache \
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

ARG USERNAME=vscode

