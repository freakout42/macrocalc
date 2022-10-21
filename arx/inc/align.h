#undef PACK
#undef PACK2
#undef PACKED
#ifdef __GNUC__
#elif __INTEL_COMPILER
#else
#ifdef __USLC__
#pragma pack()
#elif COHERENT
#pragma align
#elif __hpux
#pragma HP_ALIGN POP
#endif
#endif
