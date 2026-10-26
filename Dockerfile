# syntax=docker/dockerfile:1.6

ARG SDK_VERSION=5.9.0
ARG BINDINGS=geode-sdk/bindings
ARG GIT_INVALIDATE_CLONE=0

FROM prevter/geode-sdk:android-${SDK_VERSION} AS build-repositories
ARG BINDINGS
ARG GIT_INVALIDATE_CLONE

WORKDIR /workspace
COPY bindings bindings

WORKDIR /workspace/bindings/codegen

RUN --mount=type=cache,target=/workspace/cpm-cache \
    --mount=type=cache,target=/workspace/project/build-docker,id=android32 \
    cmake -G Ninja -B build-docker \
    -DCPM_SOURCE_CACHE=/workspace/cpm-cache \
    && cmake --build build --parallel \
    && cp build/Codegen /workspace/


FROM prevter/geode-sdk:android-${SDK_VERSION} AS build-android32

WORKDIR /workspace
COPY bindings bindings

COPY --from=build-repositories /workspace/Codegen /workspace/project/build-docker/bindings/codegen/Codegen

WORKDIR /workspace/project
COPY . .

RUN --mount=type=cache,target=/workspace/cpm-cache \
    --mount=type=cache,target=/workspace/project/build-docker,id=android32 \
    cmake --preset docker-a32 \
    && cmake --build --preset docker-a32

FROM scratch AS export-android32
COPY --from=build-android32 /workspace/project/bin/nightly/* .

# Android 64
FROM prevter/geode-sdk:android-${SDK_VERSION} AS build-android64

WORKDIR /workspace
COPY bindings bindings

COPY --from=build-repositories /workspace/Codegen /workspace/project/build-docker/bindings/codegen/Codegen

WORKDIR /workspace/project
COPY . .

RUN --mount=type=cache,target=/workspace/cpm-cache \
    --mount=type=cache,target=/workspace/project/build-docker,id=android64 \
    cmake --preset docker-a64 \
    && cmake --build --preset docker-a64

FROM scratch AS export-android64
COPY --from=build-android64 /workspace/project/bin/nightly/* .

# iOS
FROM prevter/geode-sdk:ios-${SDK_VERSION} AS build-ios

WORKDIR /workspace
COPY bindings bindings

COPY --from=build-repositories /workspace/Codegen /workspace/project/build-docker/bindings/codegen/Codegen

WORKDIR /workspace/project
COPY . .

RUN --mount=type=cache,target=/workspace/cpm-cache \
    --mount=type=cache,target=/workspace/project/build-docker,id=ios \
    cmake --preset docker-ios \
    && cmake --build --preset docker-ios

FROM scratch AS export-ios
COPY --from=build-ios /workspace/project/bin/nightly/* .


# MacOS
FROM prevter/geode-sdk:macos-${SDK_VERSION} AS build-macos
ARG CPM_CACHE_DIR

WORKDIR /workspace
COPY bindings bindings

COPY --from=build-repositories /workspace/Codegen /workspace/project/build-docker/bindings/codegen/Codegen

WORKDIR /workspace/project
COPY . .

RUN --mount=type=cache,target=/workspace/cpm-cache \
    --mount=type=cache,target=/workspace/project/build-docker,id=macos \
    cmake --preset docker-mac \
    && cmake --build --preset docker-mac

FROM scratch AS export-macos
COPY --from=build-macos /workspace/project/bin/nightly/* .

# Windows
FROM prevter/geode-sdk:windows-${SDK_VERSION} AS build-windows
ARG CPM_CACHE_DIR

WORKDIR /workspace
COPY bindings bindings

COPY --from=build-repositories /workspace/Codegen /workspace/project/build-docker/bindings/codegen/Codegen

WORKDIR /workspace/project
COPY . .

RUN --mount=type=cache,target=/workspace/cpm-cache \
    --mount=type=cache,target=/workspace/project/build-docker,id=windows \
    cmake --preset docker-win \
    && cmake --build --preset docker-win

FROM scratch AS export-windows
COPY --from=build-windows /workspace/project/bin/nightly/* .

FROM debian:bookworm-slim AS export-combine

WORKDIR /workspace/export
COPY --from=export-android32 /Geode.android32.so .
COPY --from=export-android32 /Geode.android32.so.sym .
COPY --from=export-android64 /Geode.android64.so .
COPY --from=export-android64 /Geode.android64.so.sym .
COPY --from=export-ios /Geode.ios.dylib .
COPY --from=export-macos /Geode.dylib .
COPY --from=export-macos /GeodeBootstrapper.dylib .
COPY --from=export-windows /Geode.dll .
COPY --from=export-windows /Geode.lib .
COPY --from=export-windows /Geode.pdb .
COPY --from=export-windows /GeodeUpdater.exe .
COPY --from=export-windows /GeodeUpdater.pdb .
COPY --from=export-windows /XInput1_4.dll .
COPY --from=export-windows /XInput1_4.lib .
COPY --from=export-windows /XInput1_4.pdb .
COPY --from=export-windows /resources .

WORKDIR /
COPY docker-extract.sh .
RUN chmod +x /docker-extract.sh
ENTRYPOINT ["/docker-extract.sh"]
