
/*
 * Automatically generated on Mon Sep 13 13:40:41 CEST 2010
 * by 'generate_ov_call_macros.tcl'. Do not edit.
 *
 * OV Call Macros - Simplified method calling in OV
 *
 * (c) 2010 Chair of Process Control Engineering, RWTH Aachen University
 *
 * ov_call_macros_10.h: OV method calling macros (max. 10 arguments)
 *
 * Implemented with multiple macros since ANSI C does not support macros
 * with variadic argument lists.
 *
 * Usage:    Ov_Call<N> (CLASS, OBJECT, METHOD, ARG_1, ..., ARG_N)
 *
 * Example:  Ov_Call2 (ov_object, pobj, setvar, pelem, pvarcurrprops)
 *
 * Note: 'OBJECT' is evaluated twice, therefore, it should be a variable.
 */

#ifndef Ov_Call0
#define Ov_Call0(CLASS, OBJECT, METHOD) \
        (((OV_VTBLPTR_ ## CLASS) \
          (Ov_GetClassPtr (OBJECT))->v_pvtable)->m_ ## METHOD \
         (Ov_PtrUpCast(CLASS, OBJECT)))
#endif

#ifndef Ov_Call1
#define Ov_Call1(CLASS, OBJECT, METHOD, \
                 ARG1) \
        (((OV_VTBLPTR_ ## CLASS) \
          (Ov_GetClassPtr (OBJECT))->v_pvtable)->m_ ## METHOD \
         (Ov_PtrUpCast(CLASS, OBJECT), \
          ARG1))
#endif

#ifndef Ov_Call2
#define Ov_Call2(CLASS, OBJECT, METHOD, \
                 ARG1, \
                 ARG2) \
        (((OV_VTBLPTR_ ## CLASS) \
          (Ov_GetClassPtr (OBJECT))->v_pvtable)->m_ ## METHOD \
         (Ov_PtrUpCast(CLASS, OBJECT), \
          ARG1, \
          ARG2))
#endif
//(OV_INSTPTR_ov_object)

#ifndef Ov_Call3
#define Ov_Call3(CLASS, OBJECT, METHOD, \
                 ARG1, \
                 ARG2, \
                 ARG3) \
        (((OV_VTBLPTR_ ## CLASS) \
          (Ov_GetClassPtr (OBJECT))->v_pvtable)->m_ ## METHOD \
         (Ov_PtrUpCast(CLASS, OBJECT), \
          ARG1, \
          ARG2, \
          ARG3))
#endif

#ifndef Ov_Call4
#define Ov_Call4(CLASS, OBJECT, METHOD, \
                 ARG1, \
                 ARG2, \
                 ARG3, \
                 ARG4) \
        (((OV_VTBLPTR_ ## CLASS) \
          (Ov_GetClassPtr (OBJECT))->v_pvtable)->m_ ## METHOD \
         (Ov_PtrUpCast(CLASS, OBJECT), \
          ARG1, \
          ARG2, \
          ARG3, \
          ARG4))
#endif

#ifndef Ov_Call5
#define Ov_Call5(CLASS, OBJECT, METHOD, \
                 ARG1, \
                 ARG2, \
                 ARG3, \
                 ARG4, \
                 ARG5) \
        (((OV_VTBLPTR_ ## CLASS) \
          (Ov_GetClassPtr (OBJECT))->v_pvtable)->m_ ## METHOD \
         (Ov_PtrUpCast(CLASS, OBJECT), \
          ARG1, \
          ARG2, \
          ARG3, \
          ARG4, \
          ARG5))
#endif

#ifndef Ov_Call6
#define Ov_Call6(CLASS, OBJECT, METHOD, \
                 ARG1, \
                 ARG2, \
                 ARG3, \
                 ARG4, \
                 ARG5, \
                 ARG6) \
        (((OV_VTBLPTR_ ## CLASS) \
          (Ov_GetClassPtr (OBJECT))->v_pvtable)->m_ ## METHOD \
         (Ov_PtrUpCast(CLASS, OBJECT), \
          ARG1, \
          ARG2, \
          ARG3, \
          ARG4, \
          ARG5, \
          ARG6))
#endif

#ifndef Ov_Call7
#define Ov_Call7(CLASS, OBJECT, METHOD, \
                 ARG1, \
                 ARG2, \
                 ARG3, \
                 ARG4, \
                 ARG5, \
                 ARG6, \
                 ARG7) \
        (((OV_VTBLPTR_ ## CLASS) \
          (Ov_GetClassPtr (OBJECT))->v_pvtable)->m_ ## METHOD \
         (Ov_PtrUpCast(CLASS, OBJECT), \
          ARG1, \
          ARG2, \
          ARG3, \
          ARG4, \
          ARG5, \
          ARG6, \
          ARG7))
#endif

#ifndef Ov_Call8
#define Ov_Call8(CLASS, OBJECT, METHOD, \
                 ARG1, \
                 ARG2, \
                 ARG3, \
                 ARG4, \
                 ARG5, \
                 ARG6, \
                 ARG7, \
                 ARG8) \
        (((OV_VTBLPTR_ ## CLASS) \
          (Ov_GetClassPtr (OBJECT))->v_pvtable)->m_ ## METHOD \
         (Ov_PtrUpCast(CLASS, OBJECT), \
          ARG1, \
          ARG2, \
          ARG3, \
          ARG4, \
          ARG5, \
          ARG6, \
          ARG7, \
          ARG8))
#endif

#ifndef Ov_Call9
#define Ov_Call9(CLASS, OBJECT, METHOD, \
                 ARG1, \
                 ARG2, \
                 ARG3, \
                 ARG4, \
                 ARG5, \
                 ARG6, \
                 ARG7, \
                 ARG8, \
                 ARG9) \
        (((OV_VTBLPTR_ ## CLASS) \
          (Ov_GetClassPtr (OBJECT))->v_pvtable)->m_ ## METHOD \
         (Ov_PtrUpCast(CLASS, OBJECT), \
          ARG1, \
          ARG2, \
          ARG3, \
          ARG4, \
          ARG5, \
          ARG6, \
          ARG7, \
          ARG8, \
          ARG9))
#endif

#ifndef Ov_Call10
#define Ov_Call10(CLASS, OBJECT, METHOD, \
                 ARG1, \
                 ARG2, \
                 ARG3, \
                 ARG4, \
                 ARG5, \
                 ARG6, \
                 ARG7, \
                 ARG8, \
                 ARG9, \
                 ARG10) \
        (((OV_VTBLPTR_ ## CLASS) \
          (Ov_GetClassPtr (OBJECT))->v_pvtable)->m_ ## METHOD \
         (Ov_PtrUpCast(CLASS, OBJECT), \
          ARG1, \
          ARG2, \
          ARG3, \
          ARG4, \
          ARG5, \
          ARG6, \
          ARG7, \
          ARG8, \
          ARG9, \
          ARG10))
#endif

