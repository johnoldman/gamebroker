#!/bin/sh

# The directory for ZinixEngine 
ZINIX_BIN_DIR=$HOME/service/Zinix/Broker

# The directory ZinixEngine expects to find libKYRClient.so
KYR_LIB_DIR=$HOME/service/Zinix/Broker

# The directory ZinixEngine expects to find libactivemq-cpp-2.1.1.so.1
MQ_LIB_DIR=/usr/local/lib

# The full path to the ZinixEngine executable
ZINIX_BIN=$ZINIX_BIN_DIR/ZinixEngine

# Make sure the Zinix executable exists
test -f $ZINIX_BIN || exit 0

# Is the binary already running 
pid=`pidof $ZINIX_BIN` || true

case "$1" in
  start)
	# This needs something better, partly because pidof seems to not 
	# work properly and partly because eventually we want N ZinixEngines anyway.
        #if [ "$pid" ]; then
        #        echo "ZinixEngine is already running."
        #        exit 0
        #fi
        echo -n "Starting ZinixEngine ..."
        
        cd $ZINIX_BIN_DIR
        export LD_LIBRARY_PATH=/usr/local/lib:$KYR_LIB_DIR
        $ZINIX_BIN broker.xml &
        
        echo "started"
        
        ;;
  stop)
        echo -n "Stopping ZinixEngine ..."
        #if [ "$pid" ]; then
	#	kill -TERM $pid >/dev/null 2>&1
        #fi
	killall $ZINIX_BIN
	echo "stopped"
	        
        ;;
  restart|force-reload)
        $0 stop
        $0 start
        ;;
  *)
        echo "Usage: $0 {start|stop|restart|force-reload}" >&2
        exit 1
        ;;
esac

exit 0
