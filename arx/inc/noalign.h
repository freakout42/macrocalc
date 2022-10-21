#define PACKED
#define PACK
#define PACK2

#ifdef __GNUC__
 #undef PACK
 #undef PACK2
 #define PACK __attribute__((packed))
 #if __GNUC__ >= 4
  #define PACK2
 #else
  #define PACK2 PACK
 #endif

#elif __INTEL_COMPILER
 #undef PACKED
 #define PACKED __declspec(align(1))

#else
 #ifdef __USLC__
  #pragma pack(1)
 #elif COHERENT
  #pragma align 1
 #elif __hpux
  #pragma HP_ALIGN NOPADDING PUSH
 #endif

#endif
