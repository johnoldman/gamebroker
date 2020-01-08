#include <cppunit/TextTestRunner.h>
#include <cppunit/extensions/TestFactoryRegistry.h> 

#include <string>
#include <iostream>


int main()
{
   /// Get the top level suite from the registry
   CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

   /// Adds the test to the list of test to run
   CppUnit::TextTestRunner runner;
   runner.addTest( suite );

   bool bWasSuccessful = runner.run("",false,true,false);

   return bWasSuccessful ? 0 : 1;

}




