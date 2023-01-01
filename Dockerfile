# MacroCALC - Lotus 1-2-3 compatible spreadsheet running on terminals with physics unit engine
# Simplified BSD License
# Copyright (c) 2023, Axel K. Reinhold
# All rights reserved.
FROM scratch
LABEL org.opencontainers.image.source=https://github.com/freakout42/macrocalc
COPY dockeroot /
CMD ["/opt/arx/bin/mc","-u"]
