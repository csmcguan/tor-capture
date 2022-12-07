#!/bin/bash

close()
{
    DOCKER=$(docker ps -a | grep tor | grep -v grep | awk '{ print $7 }')
    if [ ! -z $DOCKER ]; then
        echo "controller: stopping tor"
        docker stop tor
    fi
    CLOGGER_PID=$(ps aux | grep clogger | grep -v grep | awk '{ print $2 }')
    if [ ! -z $CLOGGER_PID ]; then
        echo "controller: stopping cell logger"
        kill $CLOGGER_PID
    fi
    if [ -f $READY ]; then
        rm $READY
    fi
    exit 1
}

trap close INT

ROUNDS=4
CAPTURES=25
READY=".ready"
ADDON_DIR=./visit/addon/

for CONFIG in "undefended" $(ls $ADDON_DIR); do
    echo "beginning captures for $CONFIG"
    for ROUND in $(seq 0 $((ROUNDS-1))); do
        LABEL=0
        while IFS="" read -r WEBSITE || [ -n "$WEBSITE" ]; do
            for CAPTURE in $(seq $((ROUND*CAPTURES)) $((ROUND*CAPTURES+CAPTURES-1))); do
                GOOD_TRACE=false
                while ! $GOOD_TRACE; do
                    echo "controller: capture $CAPTURE for $WEBSITE"
                    # start tor in docker
                    echo "controller: starting tor"
                    docker run --name tor --rm -d --network="host" tor

                    # make sure tor is up
                    python3 ./visit/verify_connection.py

                    if [ $? -eq 0 ]; then
                        # start cell logger
                        echo "controller: starting cell logger"
                        ./cell-logger/clogger undefended $LABEL-$CAPTURE.cell &
                        # check that cell logger is ready
                        TRIES=0
                        while [ $TRIES -lt 5 ] && [ ! -f $READY ] && [ "$(cat $READY 2> /dev/null)" != "1" ]; do
                            TRIES=$((TRIES+1))
                            sleep 1
                        done
                        if [ -f $READY ] && [ "$(cat $READY 2> /dev/null)" = "1" ]; then
                            # visit website
                            echo "controller: visiting $WEBSITE"
                            python3 ./visit/visit.py http://$WEBSITE $CONFIG
                        else
                            echo "controller: failed to start cell logger in under 5 seconds"
                        fi
                    fi

                    # cleanup
                    echo "controller: stopping tor"
                    docker stop tor
                    echo "controller: stopping cell logger"
                    CLOGGER_PID=$(ps aux | grep clogger | grep -v grep | awk '{ print $2 }')
                    TRIES=0
                    while [ ! -z $CLOGGER_PID ] && [ $TRIES -lt 5 ]; do
                        echo -e -n "\x03" | nc -w 1 -u 0.0.0.0 7000
                        CLOGGER_PID=$(ps aux | grep clogger | grep -v grep | awk '{ print $2 }')
                        TRIES=$((TRIES+1))
                    done

                    # kill cell logger if it didn't close nicely
                    CLOGGER_PID=$(ps aux | grep clogger | grep -v grep | awk '{ print $2 }')
                    if [ ! -z $CLOGGER_PID ]; then
                        echo "controller: failed to stop cell logger elegantly, killing now"
                        kill $CLOGGER_PID
                    fi
                    # remove ready file if cell logger didn't close nicely
                    if [ -f $READY ]; then
                        rm $READY
                    fi

                    # check if we got a trace
                    FILE="./log/$CONFIG/$LABEL-$CAPTURE.cell"
                    if [ -f $FILE ]; then
                        GOOD_TRACE=true
                    fi
                    sleep 5
                done
            done
            LABEL=$((LABEL+1))
        done < websites.txt
    done
done
