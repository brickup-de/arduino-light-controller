#ifndef Debug_h
#define Debug_h
//##########################################################
#define DEBUG true
//##########################################################
#if DEBUG == true
  #define initDebuggung() Serial.begin(115200); while(!Serial); DebugLn(""); DebugLn("SETUP")
  #define Debug(...)      Serial.print(__VA_ARGS__)
  #define DebugLn(...)    Serial.println(__VA_ARGS__)
#else
  #define initDebuggung() // nothing
  #define Debug(...)      // nothing
  #define DebugLn(...)    // nothing
#endif

#endif