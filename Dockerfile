# MacroCALC - Lotus 1-2-3 compatible spreadsheet running on terminals with physics unit engine
# https://mc.freakout.de/
# Simplified BSD License
# Copyright (c) 2023, Axel K. Reinhold
# All rights reserved.
FROM scratch
LABEL org.opencontainers.image.source=https://github.com/freakout42/macrocalc
LABEL org.opencontainers.image.description="docker run -ti -e TERM ghcr.io/freakout42/macrocalc:latest"
LABEL org.opencontainers.image.licenses="BSD 2-Clause License"
COPY dockeroot /
CMD ["/opt/arx/bin/mc","-u"]
