# syntax=docker/dockerfile:1.6

FROM ubuntu:26.10 AS clone-bindings
ARG BINDINGS

RUN apt-get update; \
    apt-get install -y --no-install-recommends \
        ca-certificates curl jq git; \
    rm -rf /var/lib/apt/lists/*
RUN git clone https://github.com/geode-sdk/bindings.git --depth 1 "/bindings"


FROM ghcr.io/geode-sdk/tools:android-latest AS build-android32

WORKDIR /workspace
COPY . .

RUN curl -Lf https://raw.githubusercontent.com/geode-sdk/geode-docker/refs/heads/main/docker/CMakePresets.json -o CMakePresets.json

COPY --from=clone-bindings /bindings /bindings

RUN --mount=type=cache,target=/cpm-cache,id=cpm-android32 \
    --mount=type=cache,target=/workspace/build-android32,id=build-android32 \
    cmake --preset android32 \
    && cmake --build --preset android32

FROM scratch AS export-android32
COPY --from=build-android32 /workspace/bin/nightly/* .


FROM ghcr.io/geode-sdk/tools:android-latest AS build-android64

WORKDIR /workspace
COPY . .

RUN curl -Lf https://raw.githubusercontent.com/geode-sdk/geode-docker/refs/heads/main/docker/CMakePresets.json -o CMakePresets.json

COPY --from=clone-bindings /bindings /bindings

RUN --mount=type=cache,target=/cpm-cache,id=cpm-android64 \
    --mount=type=cache,target=/workspace/build-android64,id=build-android64 \
    cmake --preset android64 \
    && cmake --build --preset android64

FROM scratch AS export-android64
COPY --from=build-android64 /workspace/bin/nightly/* .


FROM ghcr.io/geode-sdk/tools:ios-latest AS build-ios

WORKDIR /workspace
COPY . .

RUN curl -Lf https://raw.githubusercontent.com/geode-sdk/geode-docker/refs/heads/main/docker/CMakePresets.json -o CMakePresets.json

COPY --from=clone-bindings /bindings /bindings

RUN --mount=type=cache,target=/cpm-cache,id=cpm-ios \
    --mount=type=cache,target=/workspace/build-ios,id=build-ios \
    cmake --preset ios \
    && cmake --build --preset ios

FROM scratch AS export-ios
COPY --from=build-ios /workspace/bin/nightly/* .


FROM ghcr.io/geode-sdk/tools:macos-latest AS build-mac

WORKDIR /workspace
COPY . .

RUN curl -Lf https://raw.githubusercontent.com/geode-sdk/geode-docker/refs/heads/main/docker/CMakePresets.json -o CMakePresets.json

COPY --from=clone-bindings /bindings /bindings

RUN --mount=type=cache,target=/cpm-cache,id=cpm-mac \
    --mount=type=cache,target=/workspace/build-mac,id=build-mac \
    cmake --preset mac \
    && cmake --build --preset mac

FROM scratch AS export-mac
COPY --from=build-mac /workspace/bin/nightly/* .


FROM ghcr.io/geode-sdk/tools:windows-latest AS build-win

WORKDIR /workspace
COPY . .

RUN curl -Lf https://raw.githubusercontent.com/geode-sdk/geode-docker/refs/heads/main/docker/CMakePresets.json -o CMakePresets.json

COPY --from=clone-bindings /bindings /bindings

RUN --mount=type=cache,target=/cpm-cache,id=cpm-win \
    --mount=type=cache,target=/workspace/build-win,id=build-win \
    cmake --preset win \
    && cmake --build --preset win

FROM scratch AS export-win
COPY --from=build-win /workspace/bin/nightly/* .


FROM debian:bookworm-slim AS export-combine

WORKDIR /workspace/export
COPY --from=export-android32 /Geode.android32.so .
COPY --from=export-android64 /Geode.android64.so .
COPY --from=export-ios /Geode.ios.dylib .
COPY --from=export-mac /Geode.dylib .
COPY --from=export-mac /GeodeBootstrapper.dylib .
COPY --from=export-mac /libfmod.dylib .
COPY --from=export-win /Geode.dll .
COPY --from=export-win /Geode.lib .
COPY --from=export-win /Geode.pdb .
COPY --from=export-win /GeodeUpdater.exe .
COPY --from=export-win /GeodeUpdater.pdb .
COPY --from=export-win /XInput1_4.dll .
COPY --from=export-win /XInput1_4.lib .
COPY --from=export-win /XInput1_4.pdb .
COPY --from=export-win /resources .
