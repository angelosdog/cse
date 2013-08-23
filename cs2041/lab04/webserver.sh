#!/bin/sh
#ps | grep "webserver-simpl" | cut -d " " -f 1 | xargs kill -9
#ps | grep "netcat" | cut -d " " -f 1 | xargs kill -9

while true
do
	netcat -vvl localhost -p 2041 -c '
		set -x
		read http_request
        cd "$HOME/public_html"
        file=`echo "$http_request" | cut -d" " -f2 `
        if [ `echo $file | egrep "/$"` ]; 
        then
            if [ -e ."$file"index.html ]
            then
                echo HTTP/1.0 200 OK
                echo
                echo "`cat .${file}index.html`"
            elif [ -d ."$file" ]
            then
                echo HTTP/1.0 200 OK
                echo
                echo "<html><head></head><body>"
                echo "<ul><li><a href=\"http://localhost:2041${file:1}/..\">Parent Directory</a></li>"
                for f in ."$file"*;
                do
                    if [ -d $f ];
                    then
                        f="$f"/
                    fi
                    f=${f:1}
                    echo "<li><a href=\"http://localhost:2041${f}\">"$f"</a><li>"
                done
                echo "</ul></body></html>"
            else
                echo HTTP/1.0 404 File not found
                echo
                echo "<html><head><body><h1>ERROR 404</h1></body></head></html>"
            fi
        else
             if [ -e ."$file" ];
            then
                if [ test `cat .$file | egrep \.cgi$ ` ];
                then
                    echo HTTP/1.0 200 OK
                    echo 
                    echo "`sh .$file`"
                else
                    echo HTTP/1.0 200 OK
		            echo
		            echo "`cat .$file`"
                fi
            else
                echo HTTP/1.0 404 File not found
                echo
                echo "<html><head></head><body><h1>ERROR 404</h1></body></html>"
            fi
        fi
	'	
done
