#include <iostream>
#include <dlfcn.h>
#include <cxxabi.h>
#include <fstream>

extern "C" void __cyg_profile_func_enter(void *this_fn, void *call_site) __attribute__((no_instrument_function));
extern "C" void __cyg_profile_func_exit(void *this_fn, void *call_site) __attribute__((no_instrument_function));

std::ifstream ifs("traceback.log");

void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
    static std::ofstream ofs("traceback.log");
    (void)this_fn;
    (void)call_site;
    Dl_info this_fn_info;
    if (dladdr(this_fn, &this_fn_info))
    {
        int status = 0;
        char *ret = abi::__cxa_demangle(this_fn_info.dli_sname,0, 0, &status);
        if (!ret)
            return ;
        int len = std::strlen(ret);
        if (ret && len >= 3
            && !(ret[0] == 's' && ret[1] == 't' && ret[2] == 'b')
            && !(ret[0] == 's' && ret[1] == 't' && ret[2] == 'd')
            && !(ret[0] == 'D' && ret[1] == 'e' && ret[2] == 'c'))
            ofs << ret << std::endl;
    }
} /* __cyg_profile_func_enter */

void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
    (void)this_fn;
    (void)call_site;
} /* __cyg_profile_func_enter */
