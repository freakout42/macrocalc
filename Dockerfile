# MacroCALC - Lotus 1-2-3 compatible spreadsheet running on terminals with physics unit engine
# Simplified BSD License
# Copyright (c) 2023, Axel K. Reinhold
# All rights reserved.
FROM scratch
COPY dockeroot /
CMD ["/opt/arx/bin/mc","-u"]
# docker build -t macrocalc:10.25 .
# docker run -e COLUMNS="`tput cols`" -e LINES="`tput lines`" --tty --interactive --env TERM macrocalc:10.25
