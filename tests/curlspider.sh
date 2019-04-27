#!/bin/sh

YELLOW='\033[1;36m'
NC='\033[1;0m'

printf "\n"
echo -e "${YELLOW} CURL - HostName ${NC}"
curl -i "localhost:8000"
printf "\n"


printf "\n"
echo -e "${YELLOW} CURL - with IP ${NC}"
curl -i "http://127.0.0.1:8000"
printf "\n"


printf "\n"
echo -e "${YELLOW} Netcat - Get ${NC}"
printf 'GET / HTTP/1.1\r\nHost: localhost\r\n\r\n' | nc localhost 8000
printf "\n"

printf "\n"
echo -e "${YELLOW} Netcat - Head ${NC}"
printf 'HEAD / HTTP/1.1\r\nHost: localhost\r\n\r\n' | nc localhost 8000
printf "\n"


printf "\n"
echo -e "${YELLOW} Netcat - Post ${NC}"
printf 'POST / HTTP/1.1\r\nHost: localhost\r\n\r\n' | nc localhost 8000
printf "\n"



printf "\n"
echo -e "${YELLOW} Netcat - Get with File ${NC}"
printf 'GET /index.txt HTTP/1.1\r\nHost: localhost\r\n\r\n' | nc localhost 8000
printf "\n"


printf "\n"
echo -e "${YELLOW} Netcat - Invalid file 404 ${NC}"
printf 'GET /index.text HTTP/1.1\r\nHost: localhost\r\n\r\n' | nc localhost 8000
printf "\n"

printf "\n"
echo -e "${YELLOW} Netcat - Known Request ${NC}"
printf 'OPTIONS /index.text HTTP/1.1\r\nHost: localhost\r\n\r\n' | nc localhost 8000
printf "\n"

printf "\n"
echo -e "${YELLOW} Netcat - Update Required ${NC}"
printf 'GET /index.text HTTP/3.1\r\nHost: localhost\r\n\r\n' | nc localhost 8000
printf "\n"

printf "\n"
echo -e "${YELLOW} Netcat - Bad Request ${NC}"
printf 'xx /index.text HTTP/1.1\r\nHost: localhost\r\n\r\n' | nc localhost 8000
printf "\n"

printf "\n"
echo -e "${YELLOW} Netcat - Headers too long ${NC}"
printf 'GET /index.text HTTP/3.1\r\nHost: localhost\r\n\r\n' | nc localhost 8000
printf "\n"

printf "\n"
echo -e "${YELLOW} Netcat - Uri too long ${NC}"
printf 'GET /index.texttttttttttttttttttttttttttttttttttttttttttttttttt HTTP/3.1\r\nHost: localhost\r\n\r\n' | nc localhost 8000
printf "\n"

printf "\n"
echo -e "${YELLOW} Netcat - Payload too long ${NC}"
printf 'GET /index.text HTTP/3.1\r\nHost: localhost\r\nContent-Length: 5\r\n\r\nbvoiusaboiuvaiuvbaosiuvbsaoiubvoisubvioausbviasvfdbsdbsdbdsubviusbvi' | nc localhost 8000
printf "\n"
