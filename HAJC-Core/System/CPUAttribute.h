#ifndef HAJC_CORE_SYSTEM_CPU_ATTRIBUTE_H_
#define HAJC_CORE_SYSTEM_CPU_ATTRIBUTE_H_

namespace HAJC {
    enum class CPUArch {
        ALL                                                             = 0x0000,
        x86_64                                                          = 0x0011, 
        x86                                                             = 0x0012
    };

    enum class CPUFeature : short {
        x64_86_MMX                                                      = 0x0001,
        x64_86_SSE                                                      = 0x0002,
        x64_86_SSE2                                                     = 0x0003,
        x64_86_SSE3                                                     = 0x0004,
        x64_86_SSSE3                                                    = 0x0005,
        x64_86_SSE4_1                                                   = 0x0006,
        x64_86_SSE4_2                                                   = 0x0007,
        x64_86_AVX                                                      = 0x0008,
        x64_86_AVX2                                                     = 0x0009,
        x64_86_FMA                                                      = 0x000A,
        x64_86_AVX512                                                   = 0x000B,

        x64_86_SVML                                                     = 0x000C, 
        x64_86_ADX                                                      = 0x000D,  
        x64_86_BMI1                                                     = 0x000E,
        x64_86_BMI2                                                     = 0x000F,
        x64_86_CLFLUSHOPT                                               = 0x0010,
        x64_86_CLWB                                                     = 0x0011,
        x64_86_FP16C                                                    = 0x0012,
        x64_86_FSGSBASE                                                 = 0x0013,
        x64_86_FXSR                                                     = 0x0014,
        x64_86_INVPCID                                                  = 0x0015,
        x64_86_LZCNT                                                    = 0x0016,
        x64_86_MONITOR                                                  = 0x0017,
        x64_86_MPX                                                      = 0x0018,
        x64_86_PCLMULQDQ                                                = 0x0019,
        x64_86_POPCNT                                                   = 0x001A,
        x64_86_PREFETCHWT1                                              = 0x001B,
        x64_86_RDPID                                                    = 0x001C,
        x64_86_RDRAND                                                   = 0x001D,
        x64_86_RDSEED                                                   = 0x001E,
        x64_86_RDTSCP                                                   = 0x001F,
        x64_86_RTM                                                      = 0x0020,
        x64_86_SHA                                                      = 0x0021,
        x64_86_TSC                                                      = 0x0022,
        x64_86_XSAVE                                                    = 0x0023,
        x64_86_XSAVEC                                                   = 0x0024,
        x64_86_XSAVEOPT                                                 = 0x0025,
        x64_86_XSS                                                      = 0x0026,
    };

    class CPUAttribute {
        CPUArch m_cpuArch;
    };


}

#endif