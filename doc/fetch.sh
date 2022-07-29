#!/bin/sh

DIR="$(cd `dirname $0` && pwd)"

wget -i- -P "$DIR" <<EOF
https://eater.net/datasheets/w65c02s.pdf
https://eater.net/datasheets/w65c22.pdf
https://eater.net/datasheets/HD44780.pdf
https://eater.net/datasheets/74hc00.pdf
https://eater.net/datasheets/28c256.pdf
https://eater.net/datasheets/hm62256b.pdf
EOF
