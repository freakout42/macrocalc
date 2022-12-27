# MacroCALC - Lotus 1-2-3 compatible spreadsheet running on terminals with physics unit engine
# docker build -t mc:10.25 .
# Simplified BSD License
# Copyright (c) 2023, Axel K. Reinhold
# All rights reserved.
FROM scratch
COPY dist/arx /opt/arx/
CMD ["/opt/arx/bin/mc","-u"]
