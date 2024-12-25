#include <stdio.h>
#include <stdlib.h>
#include "vpi_user.h"
#include "sv_vpi_user.h"
#include <vector>
#include <string>
#include <unistd.h>
#include <iostream>

using namespace std;

typedef vector<string>   StrVec;

string getDirectory(const string& filePath) {
    size_t pos = filePath.find_last_of("/\\");
    if (pos != string::npos) {
        return filePath.substr(0, pos);
    }
    return "";
}

class GateInst
{
public:
    GateInst(const char* pName)
    { 
        _instName = pName ? pName : "Unknown";
    }

    void setGateType(const char* pName)
    {
        _gateType = pName ? pName : "Unknown";
    }

    void addNodeName(const char* pName)
    {
        _instNodes.push_back(pName ? pName : "Unknown");
    }

    void dumpInst()
    {
        printf("%-5s  %s  ", _gateType.c_str(), _instName.c_str());
        for(unsigned ii = 0; ii != _instNodes.size(); ++ii)
        {
            printf("%s  ", _instNodes[ii].c_str());
        }
        printf("\n");
    }

    string  _gateType;
    string  _instName;
    StrVec  _instNodes;
};

typedef vector<GateInst*>  GateInstVec;

class NetlistModule
{
public:
    ~NetlistModule()
    {
        for(unsigned ii = 0; ii != _instVec.size(); ++ii)
        {
            delete _instVec[ii];
        }
    }

    void setModuleName(const char* pName)
    {
        if(pName)
            _moduelName = pName;
    }

    void addInst(GateInst* pInst)
    {
        _instVec.push_back(pInst);
    }

    void dumpInst()
    {
        for(unsigned ii = 0; ii != _instVec.size(); ++ii)
        {
            GateInst* pInst = _instVec[ii];
            if(pInst)
            {
                printf("%-3d: ", ii);
                pInst->dumpInst();
            }
        }
    }

    string      _moduelName;
    GateInstVec _instVec;
};

NetlistModule _glbNetlistModule;

void printModInfo()
{
    // Redirect stdout to a file
    FILE* file = freopen("modinfo.txt", "w", stdout);
    if (file == nullptr) {
        perror("Failed to redirect stdout");
        return;
    }

    printf("module name is %s\n", _glbNetlistModule._moduelName.c_str());
    _glbNetlistModule.dumpInst();

    fclose(file);
    stdout = stdout;
}



#ifdef  __cplusplus
extern "C" {
#endif

int get_module_info(char* ptr)
{
    (void)ptr;
    vpiHandle systf_h = vpi_handle( vpiSysTfCall, NULL );
    if( systf_h == NULL )
        return 0;

    vpiHandle modHdl = vpi_handle( vpiScope,systf_h );
    char *pModDefName = vpi_get_str(vpiDefName, modHdl);
    _glbNetlistModule.setModuleName(pModDefName);

    //vpiHandle instIter = vpi_iterate(vpiInstance, modHdl);
    vpiHandle instIter = vpi_iterate(vpiModule, modHdl);
    vpiHandle instHdl = NULL;
    while( ( instHdl = vpi_scan( instIter ) ) )
    {
        char* pInstName = vpi_get_str( vpiName, instHdl );
        GateInst* pGateInst = new GateInst(pInstName);

        pModDefName = vpi_get_str(vpiDefName, instHdl);
        pGateInst->setGateType(pModDefName);

        vpiHandle portIter = vpi_iterate(vpiPort, instHdl);
        vpiHandle portHdl = NULL;
        while ( (portHdl = vpi_scan(portIter)) )
        {
            vpiHandle highConnHdl = vpi_handle(vpiHighConn, portHdl);
            char* pPortName = vpi_get_str(vpiName, highConnHdl);
            pGateInst->addNodeName(pPortName);
        }

        _glbNetlistModule.addInst(pGateInst);
    }

    printModInfo();
    string pwd = getDirectory(__FILE__);
    string command = pwd + "/bin/logicLoopChecker modinfo.txt ";
    system(command.c_str());
    vpi_control(vpiFinish);
    return 0;
}

void vpi_func_register() 
{
	s_vpi_systf_data tf_data;

	tf_data.type      = vpiSysTask;
	tf_data.tfname    = const_cast<char*>("$get_module_info");
	tf_data.calltf    = get_module_info;
	tf_data.compiletf = NULL;
	tf_data.sizetf    = 0;
	tf_data.user_data = 0;
	vpi_register_systf(&tf_data);
}



void (*vlog_startup_routines[])() = {
	vpi_func_register,
	0
};

#ifdef  __cplusplus
}
#endif


