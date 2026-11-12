# syntax=docker/dockerfile:1.6

FROM ghcr.io/geode-sdk/tools:android-latest AS build-android32

ARG GEODE_COMMIT_HASH=""

WORKDIR /workspace
COPY . .

RUN curl -Lf https://raw.githubusercontent.com/geode-sdk/geode-docker/refs/heads/main/docker/CMakePresets.json -o CMakePresets.json

RUN --mount=type=cache,target=/cpm-cache,id=cpm-android32 \
    --mount=type=cache,target=/opt/sccache/cache,id=sccache-android32 \
    --mount=type=bind,from=bindings,target=/bindings \
    cmake --preset android32-cache -DGEODE_DISABLE_PRECOMPILED_HEADERS=On -DGEODE_COMMIT_HASH=${GEODE_COMMIT_HASH} \
    && cmake --build --preset android32-cache && \
    ls /workspace/bin/nightly && sccache --show-stats

FROM scratch AS export-android32
COPY --from=build-android32 /workspace/bin/nightly/ .


FROM ghcr.io/geode-sdk/tools:android-latest AS build-android64

ARG GEODE_COMMIT_HASH=""

WORKDIR /workspace
COPY . .

RUN curl -Lf https://raw.githubusercontent.com/geode-sdk/geode-docker/refs/heads/main/docker/CMakePresets.json -o CMakePresets.json

RUN --mount=type=cache,target=/cpm-cache,id=cpm-android64 \
    --mount=type=cache,target=/opt/sccache/cache,id=sccache-android64 \
    --mount=type=bind,from=bindings,target=/bindings \
    cmake --preset android64-cache -DGEODE_DISABLE_PRECOMPILED_HEADERS=On -DGEODE_COMMIT_HASH=${GEODE_COMMIT_HASH} \
    && cmake --build --preset android64-cache && \
    ls /workspace/bin/nightly && sccache --show-stats

FROM scratch AS export-android64
COPY --from=build-android64 /workspace/bin/nightly/ .


FROM ghcr.io/geode-sdk/tools:ios-latest AS build-ios

ARG GEODE_COMMIT_HASH=""

WORKDIR /workspace
COPY . .

RUN curl -Lf https://raw.githubusercontent.com/geode-sdk/geode-docker/refs/heads/main/docker/CMakePresets.json -o CMakePresets.json

RUN --mount=type=cache,target=/cpm-cache,id=cpm-ios \
    --mount=type=cache,target=/opt/sccache/cache,id=sccache-ios \
    --mount=type=bind,from=bindings,target=/bindings \
    cmake --preset ios-cache -DGEODE_DISABLE_PRECOMPILED_HEADERS=On -DGEODE_COMMIT_HASH=${GEODE_COMMIT_HASH} \
    && cmake --build --preset ios-cache && \
    ls /workspace/bin/nightly && sccache --show-stats

FROM scratch AS export-ios
COPY --from=build-ios /workspace/bin/nightly/ .


FROM ghcr.io/geode-sdk/tools:macos-latest AS build-mac

ARG GEODE_COMMIT_HASH=""

WORKDIR /workspace
COPY . .

RUN curl -Lf https://raw.githubusercontent.com/geode-sdk/geode-docker/refs/heads/main/docker/CMakePresets.json -o CMakePresets.json

RUN --mount=type=cache,target=/cpm-cache,id=cpm-mac \
    --mount=type=cache,target=/opt/sccache/cache,id=sccache-mac \
    --mount=type=bind,from=bindings,target=/bindings \
    cmake --preset mac-cache -DGEODE_DISABLE_PRECOMPILED_HEADERS=On -DGEODE_COMMIT_HASH=${GEODE_COMMIT_HASH} \
    && cmake --build --preset mac-cache && \
    ls /workspace/bin/nightly && sccache --show-stats

FROM scratch AS export-mac
COPY --from=build-mac /workspace/bin/nightly/ .


FROM ghcr.io/geode-sdk/tools:macos-latest AS build-macm1

ARG GEODE_COMMIT_HASH=""

WORKDIR /workspace
COPY . .

RUN curl -Lf https://raw.githubusercontent.com/geode-sdk/geode-docker/refs/heads/main/docker/CMakePresets.json -o CMakePresets.json

RUN --mount=type=cache,target=/cpm-cache,id=cpm-macm1 \
    --mount=type=cache,target=/opt/sccache/cache,id=sccache-macm1 \
    --mount=type=bind,from=bindings,target=/bindings \
    cmake --preset macm1-cache -DGEODE_DISABLE_PRECOMPILED_HEADERS=On -DGEODE_COMMIT_HASH=${GEODE_COMMIT_HASH} \
    && cmake --build --preset macm1-cache && \
    ls /workspace/bin/nightly && sccache --show-stats

FROM scratch AS export-macm1
COPY --from=build-macm1 /workspace/bin/nightly/ .


FROM ghcr.io/geode-sdk/tools:macos-latest AS build-macintel

ARG GEODE_COMMIT_HASH=""

WORKDIR /workspace
COPY . .

RUN curl -Lf https://raw.githubusercontent.com/geode-sdk/geode-docker/refs/heads/main/docker/CMakePresets.json -o CMakePresets.json

RUN --mount=type=cache,target=/cpm-cache,id=cpm-macintel \
    --mount=type=cache,target=/opt/sccache/cache,id=sccache-macintel \
    --mount=type=bind,from=bindings,target=/bindings \
    cmake --preset macintel-cache -DGEODE_DISABLE_PRECOMPILED_HEADERS=On -DGEODE_COMMIT_HASH=${GEODE_COMMIT_HASH} \
    && cmake --build --preset macintel-cache && \
    ls /workspace/bin/nightly && sccache --show-stats

FROM scratch AS export-macintel
COPY --from=build-macintel /workspace/bin/nightly/ .


FROM ghcr.io/geode-sdk/tools:windows-latest AS build-win

ARG GEODE_COMMIT_HASH=""

WORKDIR /workspace
COPY . .

RUN curl -Lf https://raw.githubusercontent.com/geode-sdk/geode-docker/refs/heads/main/docker/CMakePresets.json -o CMakePresets.json

RUN --mount=type=cache,target=/cpm-cache,id=cpm-win \
    --mount=type=cache,target=/opt/sccache/cache,id=sccache-win \
    --mount=type=bind,from=bindings,target=/bindings \
    cmake --preset win-cache -DGEODE_DISABLE_PRECOMPILED_HEADERS=On -DGEODE_COMMIT_HASH=${GEODE_COMMIT_HASH} \
    && cmake --build --preset win-cache && \
    ls /workspace/bin/nightly && sccache --show-stats

FROM scratch AS export-win
COPY --from=build-win /workspace/bin/nightly/ .


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
