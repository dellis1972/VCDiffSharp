#ifndef _VC_DIFF_H__
#define _VC_DIFF_H__

#ifdef _WIN32
    #define VC_DIFF_CALL __declspec(dllexport)
#else
    #define VC_DIFF_CALL
#endif

#ifdef __cplusplus  
extern "C" { 
#endif 
    int VC_DIFF_CALL Encode (char * sourceFile, char * targetFile, char * patchFile);
    int VC_DIFF_CALL Decode (char * targetFile, char * patchFile, char * outputFile);
#ifdef __cplusplus  
}
#endif 

#endif /* _VC_DIFF_H__ */