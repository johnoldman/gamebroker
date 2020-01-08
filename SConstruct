#
# $Id: SConstruct 3672 2007-12-17 13:59:33Z stephend $
#
# scons file to build Purse.
#
# Philosophy is to build 3 targets:
# a) debug target
# b) release target
# c) coverage target - used purely to provide coverage data for the (passing) unit tests
#
cppoptions = '-g -Wall -Werror'
debugcppoptions = cppoptions
releasecppoptions = cppoptions + ' -O3'
coveragecppoptions = cppoptions + ' --coverage'

cppledgeroptions = '-g -DWITH_NONAMESPACES'
releasecppledgeroptions = cppledgeroptions + ' -O3 -DWITH_NONAMESPACES'

includepath = ['gameside', 'activemq/include/activemq-cpp-2.1.1', 'common', '../include', 'wsdl', 'gsoap', '.', '/usr/include/mysql']

debugenv = Environment(CCFLAGS=debugcppoptions, CPPPATH=includepath)
releaseenv = Environment(CCFLAGS=releasecppoptions, CPPPATH=includepath)
coverageenv = Environment(CCFLAGS=coveragecppoptions, CPPPATH=includepath)
debugenvt = Environment(CCFLAGS=debugcppoptions, CPPPATH=includepath + ['serviceside/', 'test/'])
releaseenvt = Environment(CCFLAGS=releasecppoptions, CPPPATH=includepath + ['serviceside/', 'test/'])
coverageenvt = Environment(CCFLAGS=coveragecppoptions, CPPPATH=includepath + ['serviceside/', 'test/'])

debugLedgerEnv = Environment(CCFLAGS=cppledgeroptions, CPPPATH=includepath)
releaseLedgerEnv = Environment(CCFLAGS=releasecppledgeroptions, CPPPATH=includepath)

import os
libs = ['SglCommon', 'mysqlpp', 'activemq-cpp', 'log4cpp', 'boost_thread-mt', 'pthread', 'cppunit', 'dl']
libpath = ['../lib', '.', 'activemq/lib']

def makeFileList(dir, ext):
   gamefiles = []
   for file in os.listdir(dir):
      if file[-len(ext):] == ext:
         gamefiles.append(file[:-len(ext)])
   return gamefiles

gamefiles = makeFileList('gameside/', '.cpp')
commonfilenames = makeFileList('common/', '.cpp')
serverfilenames = makeFileList('serviceside/', '.cpp')
ledgerfilenames = makeFileList('wsdl/', '.cpp')
gsoapfilenames = makeFileList('gsoap/', '.cpp')

gamesidefiles = ['gameside/' + file + '.cpp' for file in gamefiles]
commonfiles = ['common/' + file + '.cpp' for file in commonfilenames]
serverfiles = ['serviceside/' + file + '.cpp' for file in serverfilenames]
ledgerfiles = ['wsdl/' + file + '.cpp' for file in ledgerfilenames]
gsoapfiles = ['gsoap/' + file + '.cpp' for file in gsoapfilenames]

# This test isn't really a unit test, so doesn't belong in the general running
# list
#                        testBroker
# This test doesn't really test much, and also relies on a running broker
#                        testMQFactory

stubfiles = Split("""test/StubConsumer
                     test/StubMessageConsumer
                     test/StubLedger
                     test/StubPersistentStore
                     test/StubKYR
                     test/StubTariff
                     test/SyncDelayedReply
                     test/StubProducerConsumerFactory""")

apitestfiles = Split("""testServiceBroker
                        testLedger
                        testGameInstance
                        testGameAPI
                        testConfig
                        testCommands
                        testResponses
                        testTsq
                        testPurse
                        testPurseDb
                        testXmlIterator
                        testMessageStack
                        testMessageIdGenerator
                        testReplyMessageRouting""")

debugfiles, releasefiles, coveragefiles, deployfiles, deploydebugfiles, serverfiles, serverdebugfiles = [], [], [], [], [], [], []
bmaindebug, bmainrelease = [], []

for file in commonfilenames:
   dbugobj = debugenv.Object(target = 'Debug/' + file, source = 'common/' + file + '.cpp')
   debugfiles.append(dbugobj)
   bmaindebug.append(dbugobj)
   releasefiles.append(releaseenv.Object(target = 'Release/' + file, source = 'common/' + file + '.cpp'))
   coveragefiles.append(coverageenv.Object(target = 'Coverage/' + file, source = 'common/' + file + '.cpp'))
   deployfiles.append(releaseenv.SharedObject(target = 'Release/' + file, source = 'common/' + file + '.cpp'))
   deploydebugfiles.append(debugenv.SharedObject(target = 'Debug/' + file, source = 'common/' + file + '.cpp'))
   serverfiles.append(releaseenv.Object(target = 'Release/' + file, source = 'common/' + file + '.cpp'))
   serverdebugfiles.append(debugenv.Object(target = 'Debug/' + file, source = 'common/' + file + '.cpp'))

for file in ledgerfilenames:
   dbugobj = debugLedgerEnv.Object(target = 'Debug/' + file, source = 'wsdl/' + file + '.cpp')
   debugfiles.append(dbugobj)
   bmaindebug.append(dbugobj)
   releasefiles.append(releaseLedgerEnv.Object(target = 'Release/' + file, source = 'wsdl/' + file + '.cpp'))
   coveragefiles.append(debugLedgerEnv.Object(target = 'Coverage/' + file, source = 'wsdl/' + file + '.cpp'))
   serverfiles.append(releaseLedgerEnv.Object(target = 'Release/' + file, source = 'wsdl/' + file + '.cpp'))
   serverdebugfiles.append(debugLedgerEnv.Object(target = 'Debug/' + file, source = 'wsdl/' + file + '.cpp'))

for file in gsoapfilenames:
   dbugobj = debugLedgerEnv.Object(target = 'Debug/' + file, source = 'gsoap/' + file + '.cpp')
   debugfiles.append(dbugobj)
   bmaindebug.append(dbugobj)
   releasefiles.append(releaseLedgerEnv.Object(target = 'Release/' + file, source = 'gsoap/' + file + '.cpp'))
   coveragefiles.append(debugLedgerEnv.Object(target = 'Coverage/' + file, source = 'gsoap/' + file + '.cpp'))
   serverfiles.append(releaseLedgerEnv.Object(target = 'Release/' + file, source = 'gsoap/' + file + '.cpp'))
   serverdebugfiles.append(debugLedgerEnv.Object(target = 'Debug/' + file, source = 'gsoap/' + file + '.cpp'))

for prodfile in gamefiles:
   dbugobj = debugenv.Object(target = 'Debug/' + prodfile, source = 'gameside/' + prodfile + '.cpp')
   debugfiles.append(dbugobj)
   bmaindebug.append(dbugobj)
   releasefiles.append(releaseenv.Object(target = 'Release/' + prodfile, source = 'gameside/' + prodfile + '.cpp'))
   coveragefiles.append(coverageenv.Object(target = 'Coverage/' + prodfile, source = 'gameside/' + prodfile + '.cpp'))
   deployfiles.append(releaseenv.SharedObject(target = 'Release/' + prodfile, source = 'gameside/' + prodfile + '.cpp'))
   deploydebugfiles.append(debugenv.SharedObject(target = 'Debug/' + prodfile, source = 'gameside/' + prodfile + '.cpp'))

for file in serverfilenames:
   for set, env, dir, xtra in ((debugfiles, debugenv, 'Debug/', bmaindebug),
                               (releasefiles,releaseenv,'Release/', None),
                               (coveragefiles,coverageenv,'Coverage/', None),
                               (serverdebugfiles,debugenv,'Debug/', None),
                               (serverfiles,releaseenv,'Release/', None)):
      obj = env.Object(target = dir + file, source = 'serviceside/' + file + '.cpp')
      set.append(obj)
      if xtra != None:
         xtra.append(obj)

dmainfiles, rmainfiles = [], []

for testfile in stubfiles:
   for set, env, dir, xtra in ((debugfiles, debugenvt, 'Debug/', bmaindebug),
                               (releasefiles,releaseenvt,'Release/', None),
                               (coveragefiles,coverageenvt,'Coverage/', None)):
      testobj = env.Object(target = dir + testfile, source = testfile + '.cpp')
      set.append(testobj)
      if xtra != None:
         xtra.append(testobj)

for testfile in apitestfiles:
   for set, env, dir, xtra in ((debugfiles, debugenvt, 'Debug/', None),
                               (releasefiles,releaseenvt,'Release/', None),
                               (coveragefiles,coverageenvt,'Coverage/', None)):
      testobj = env.Object(target = dir + testfile, source = testfile + '.cpp')
      set.append(testobj)
      if xtra != None:
         xtra.append(testobj)

for file in ['testmain']:
   for set, env, dir, xtra in ((debugfiles, debugenvt, 'Debug/',(dmainfiles, bmaindebug)),
                               (releasefiles,releaseenvt,'Release/',(rmainfiles, bmainrelease)),
                               (coveragefiles,coverageenvt,'Coverage/',None)):
      testobj = env.Object(target = dir + file, source = file + '.cpp')
      set.append(testobj)
      if xtra != None:
         for t in xtra:
           t.append(testobj)

sbmainsrc = 'ZinixEngineMain'
sbmain = releaseenv.Object(target = 'Release/' + sbmainsrc, source = sbmainsrc + '.cpp')
sbmain_debug = debugenv.Object(target = 'Debug/' + sbmainsrc, source = sbmainsrc + '.cpp')

# for deployment demo
demoMainSrc = 'demoGameApi'
demoMain = releaseenv.Object(target = 'Release/' + demoMainSrc, source = demoMainSrc + '.cpp')
demoMain_debug = debugenv.Object(target = 'Debug/' + demoMainSrc, source = demoMainSrc + '.cpp')

for env, dir, files, xlibs in ((debugenv,'Debug/',debugfiles,[]),(releaseenv,'Release/',releasefiles,[]),(coverageenv,'Coverage/',coveragefiles,['gcov'])):
   env.Program(dir + 'alltests',files, LIBS=['KYRClient'] + libs + xlibs, LIBPATH=libpath + ['../lib'])

# Actual releasable products
releaseenv.SharedLibrary('Release/ZinixClient', deployfiles)
debugenv.SharedLibrary('Debug/ZinixClient', deploydebugfiles)
releaseenv.Program('Release/ZinixEngine', serverfiles + [sbmain], LIBS=['KYRClient'] + libs, LIBPATH=libpath + ['../lib'])
debugenv.Program('Debug/ZinixEngine', serverdebugfiles + [sbmain_debug], LIBS=['KYRClient'] + libs, LIBPATH=libpath + ['../lib'])

releaseenv.Program('Release/demoGameApi', deployfiles + [demoMain], LIBS= libs, LIBPATH=libpath + ['../lib'])
debugenv.Program('Debug/demoGameApi', deploydebugfiles + [demoMain_debug], LIBS= libs, LIBPATH=libpath + ['../lib'])

for file in ['testPurseServer']:
   dmainfiles.append(debugenv.Object(target = 'Debug/' + file, source = file + '.cpp'))
   rmainfiles.append(releaseenv.Object(target = 'Release/' + file, source = file + '.cpp'))

debugenv.Program('Debug/maintests', dmainfiles, LIBS=['ZinixClient'] + libs, LIBPATH=libpath + ['Debug'])
releaseenv.Program('Release/maintests', rmainfiles, LIBS=['ZinixClient'] + libs, LIBPATH=libpath + ['Release'])

for file in ['testBroker']:
   bmaindebug.append(debugenvt.Object(target = 'Debug/' + file, source = file + '.cpp'))
   bmainrelease.append(releaseenvt.Object(target = 'Release/' + file, source = file + '.cpp'))

debugenvt.Program('Debug/brokertest', bmaindebug, LIBS=['KYRClient'] + libs, LIBPATH=libpath + ['../lib'])
#releaseenvt.Program('Release/brokertest', bmainrelease, LIBS=libs, LIBPATH=libpath + ['Release'])
