: !/bin/sh
# mcmsg:  get MicroCALC message
/bin/awk -F"	" '$1 == '$1' {print $2}'
