#include <iostream>
#include <Windows.h>

// D:\Tools\GH Injector V3.3\GH Injector - x64.dll
//    79DB1E80    InjectW     dll + 1E80
typedef __int64(__stdcall* InjectW)(DWORD* lpBuffer);

int main()
{
    HRESULT hrReturnVal;

    auto hDLL = LoadLibrary("GH Injector - x86");

    if (NULL != hDLL)
    {
        //InjectW pInjectW = (InjectW)(0x79DB1E80);
        InjectW pInjectW = (InjectW)GetProcAddress(hDLL, "InjectW");
        if (NULL != pInjectW)
        {
            //int result = injectW("ac_client.exe");
            hrReturnVal = pInjectW((DWORD*)0x40000);
        }
        else
        {
            hrReturnVal = ERROR_DELAY_LOAD_FAILED;
        }
        FreeLibrary(hDLL);
    }
    else
    {
        hrReturnVal = ERROR_DELAY_LOAD_FAILED;
    }

    std::cout << hrReturnVal << std::endl;
}
