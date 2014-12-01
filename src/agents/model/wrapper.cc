/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#include "wrapper.h"

#include "ns3/log.h"
#include "ns3/uinteger.h"
#include "ns3/double.h"

using namespace std;

namespace ns3
{

NS_LOG_COMPONENT_DEFINE ("WrapperNs3Jade");
NS_OBJECT_ENSURE_REGISTERED (Wrapper);

TypeId
Wrapper::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::Wrapper")
    .SetParent<Object> ()
    .AddConstructor<Wrapper> ();

  return tid;
}

Wrapper::Wrapper()
  : is_operate(false)
{
}

Wrapper::~Wrapper() {
  Clear();
  NS_LOG_FUNCTION_NOARGS ();
}

void
Wrapper::Initialize(string classpath, string library_path, string main_class) {
    classpath = string(CLASS).append(classpath);
    library_path = string(LIBRARY).append(library_path);

    if(is_operate == false)
    {
      is_operate = true;
    } else {
      Clear();
    }

    options = new JavaVMOption[3];
    options[0].optionString = const_cast<char*>(D_JIT);           /* disable JIT */
    options[1].optionString = const_cast<char*>(classpath.c_str()); /* user classes */
    options[2].optionString = const_cast<char*>(library_path.c_str());  /* set native library path */

    vm_args.version = JNI_VERSION_1_6;
    vm_args.nOptions = 3;
    vm_args.options = options;
    vm_args.ignoreUnrecognized = JNI_FALSE;
    /* load and initialize a Java VM, return a JNI interface
     * pointer in env */
    jint res = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), &vm_args);
    if(res < 0) {
        cerr << "Failed to create the JVM\n" << endl;
    }

    cls = env->FindClass(main_class.c_str());
    if(!cls) {
        cerr << "Failed to find class " << main_class << endl;
    }

    jmethodID mid = env->GetMethodID(cls, "<init>", "()V");
    if(!mid) {
        cerr << "Failed to find method 'init'" << endl;
    }

    run = env->NewObject(cls,mid);
    if(!run) {
        cerr << "Failed to create object 'Run'" << endl;
    }
}

void
Wrapper::CreateAgent(const char * netI) {
    jmethodID mid = env->GetMethodID(cls, "createAgent", "(Ljava/lang/String;)V");
    if(!mid) {
        cerr << "Failed to find method 'create'" << endl;
    }

    jstring net = env->NewStringUTF(netI);
    env->CallVoidMethod(run, mid, net);
}

double
Wrapper::CaptureData(const char * netI) {
    jmethodID mid = env->GetMethodID(cls, "captureData", "(Ljava/lang/String;)D");
    if(!mid) {
        cerr << "Failed to find method 'captureData'" << endl;
    }
    jstring net = env->NewStringUTF(netI);
    double data = env->CallDoubleMethod(run, mid, net);

    return data;
}

void
Wrapper::CreateAgent(const char* netI, int type, double voltage, double phase, double active, double reactive)
{
  jmethodID mid = env->GetMethodID(cls, "createAgent", "(Ljava/lang/String;IDDDD)V");
  if(!mid) {
      cerr << "Failed to find method 'create'" << endl;
  }

  jstring net = env->NewStringUTF(netI);

  env->CallVoidMethod(run, mid, net, type, voltage, phase, active, reactive);

}

vector<double>
Wrapper::ExecuteControl(vector<string> nets, const char* action)
{
  vector<double> updates;
  jmethodID mid = env->GetMethodID(cls, "doAction", "([Ljava/lang/String;Ljava/lang/String;)[D");

  if(!mid) {
    cerr << "Failed to find method 'doAction'" << endl;
  } else {
    uint32_t size = nets.size();
    jclass stringClass = env->FindClass("Ljava/lang/String;");
    jobjectArray array = env->NewObjectArray(size, stringClass, NULL);

    for(uint32_t i = 0; i < size; i++)
    {
      jstring str = env->NewStringUTF(nets.at(i).c_str());
      env->SetObjectArrayElement(array, i, str);
    }
    jstring ac = env->NewStringUTF(action);

    jdoubleArray retVal = (jdoubleArray) env->CallObjectMethod(run, mid, array, ac);
    uint32_t count = env->GetArrayLength(retVal);
    jdouble *element = env->GetDoubleArrayElements(retVal, 0);


    for(uint32_t i = 0; i < count; i++)
    {
      updates.push_back(element[i]);
    }
  }

  return updates;
}

void
Wrapper::DeleteAgent(const char* netI)
{
  jmethodID mid = env->GetMethodID(cls, "unrefAgent", "(Ljava/lang/String;)V");
  if(!mid) {
      cerr << "Failed to find method 'unrefAgent'" << endl;
  }

  jstring net = env->NewStringUTF(netI);
  env->CallVoidMethod(run, mid, net);
}

void
Wrapper::Clear()
{
  delete[] options;
  env->DeleteLocalRef(run);
  jvm->DestroyJavaVM();
}

}

