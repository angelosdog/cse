#!/bin/sh
# try to kill any webserver already running
ps -ef|egrep 'sh.*webserver.sh|netcat'|grep -v egrep|cut -c10-14|xargs -r kill -9 2>/dev/null
while true
do
    netcat -vvl localhost -p 2041 -c '
        set -x
        read http_request
        echo HTTP/1.0 200 OK
        echo
        echo "Hi, I am a shell webserver and I received this HTTP request: $http_request"
    '   
done
