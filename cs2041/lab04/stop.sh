#!/bin/bash
# Using kill -9, because we don't mess around
ps | grep "webserver-simpl" | cut -d " " -f 1 | xargs kill -9
ps | grep "netcat" | cut -d " " -f 1 | xargs kill -9

