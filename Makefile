#
# $Id: Makefile 3766 2007-12-19 14:00:18Z stephend $
#
# Common Makefile for purse
#
all:purseTestsBuild

purseTestsBuild:
	scons

clean:
	scons -c
	$(RM) core core.*

test: purseTestsBuild
	export LD_LIBRARY_PATH=activemq/lib:../lib && Release/alltests
	export LD_LIBRARY_PATH=activemq/lib:../lib && Debug/alltests
	find Coverage/ -name '*.gcda' -exec rm -f {} \;
	export LD_LIBRARY_PATH=activemq/lib:../lib && Coverage/alltests
	../../gcov.py Coverage/ common/*.cpp serviceside/*.cpp gameside/*.cpp | ../../gcovanal.py
	mv *.gcov Coverage/

utest: purseTestsBuild
	find Coverage/ -name '*.gcda' -exec rm -f {} \;
	export LD_LIBRARY_PATH=activemq/lib:../lib && Coverage/alltests
	../../gcov.py Coverage/ common/*.cpp serviceside/*.cpp gameside/*.cpp | ../../gcovanal.py
	mv *.gcov Coverage/

mtest: purseTestsBuild
	export LD_LIBRARY_PATH=/usr/local/lib:activemq/lib:Debug && Debug/maintests

rmtest: purseTestsBuild
	export LD_LIBRARY_PATH=/usr/local/lib:activemq/lib:Release && Release/maintests

rrun: purseTestsBuild
	export LD_LIBRARY_PATH=../lib:/usr/local/lib:activemq/lib:Release && Release/ZinixEngine broker.xml

run: purseTestsBuild
	export LD_LIBRARY_PATH=../lib:/usr/local/lib:activemq/lib:Debug && Debug/ZinixEngine broker.xml

btest: purseTestsBuild
	export LD_LIBRARY_PATH=../lib:activemq/lib && Debug/brokertest

# deployment test
dtest : purseTestsBuild
	export LD_LIBRARY_PATH=/usr/local/lib:activemq/lib:Debug && Debug/demoGameApi apiBroker.xml apiTestScript.xml

rdtest : purseTestsBuild
	export LD_LIBRARY_PATH=/usr/local/lib:activemq/lib:Release && Release/demoGameApi apiBroker.xml apiTestScript.xml

engine: purseTestsBuild
	export LD_LIBRARY_PATH=../lib:activemq/lib && Debug/ZinixEngine ../zinix.xml

rengine: purseTestsBuild
	export LD_LIBRARY_PATH=../lib:activemq/lib && Release/ZinixEngine ../zinix.xml
