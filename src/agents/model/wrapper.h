/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef WRAPPER_H_
#define WRAPPER_H_

#include <jni.h>
#include <iostream>
#include <string>
#include <vector>

#include "ns3/ptr.h"
#include "ns3/type-id.h"
#include "ns3/object.h"

using namespace std;

namespace ns3
{

/*
 * Examples CLASSPATH, CLS
 */
#define D_JIT "-Djava.compiler=NONE"
#define CLASSPATH "-Djava.class.path=/home/thiago/workspace/Agente/agent-run.jar"
#define LIBRARY_PATH "-Djava.library.path=/home/thiago/jade/lib"
#define CLS "com/simulation/Run"
#define CLASS "-Djava.class.path="
#define LIBRARY "-Djava.library.path="

#define ACTION_UP "Up"
#define ACTION_DOWN "Down"

#define SLACK 3
#define GENERATION 2
#define LOAD 0

class Wrapper: public Object {
public:
    void Initialize(string classpath, string library_path, string main_class);
    void CreateAgent(const char* netI);
    void CreateAgent(const char* netI, int type, double voltage, double phase, double active, double reactive);
    vector<double> ExecuteControl(vector<string> nets, const char* action);
    double CaptureData(const char* netI);
    void DeleteAgent(const char* netI);

    static TypeId GetTypeId (void);

    Wrapper() ;
    virtual ~Wrapper() ;
private:
    bool is_operate;

    JavaVM *jvm;       /* denotes a Java VM */
    JNIEnv *env;       /* pointer to native method interface */
    JavaVMInitArgs vm_args; /* JDK/JRE 6 VM initialization arguments */
    JavaVMOption* options;
    jobject run;
    jclass cls;

    void Clear();
};
}

#endif /* WRAPPER_H_ */

