

/**********************logisches UND von vektorelementen *************/

#define STDFB_VEC_AND(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 	\
					pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] & pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
	}
	
	
/**********************logisches ODER von vektorelementen *************/

#define STDFB_VEC_OR(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 	\
					pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] | pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
	}
	
/**********************logische X-ODER von vektorelementen *************/

#define STDFB_VEC_XOR(idcap, idsmall) \
	pinst->v_OUT.value.vartype = OV_VT_##idcap##_VEC;	\
	if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
	{	\
		if(Ov_OK(Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen, idcap)))	\
		{	\
			for(i=0; i<pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen; i++)	\
			{	\
				pinst->v_OUT.value.valueunion.val_##idsmall##_vec.value[i] = 	\
					pinst->v_IN1.value.valueunion.val_##idsmall##_vec.value[i] ^ pinst->v_IN2.value.valueunion.val_##idsmall##_vec.value[i];	\
			}	\
		}	\
		else	\
		{	\
		ov_logfile_error("%s: allocation of memory failed, no operation performed", pinst->v_identifier);	\
		return;	\
		}	\
	}	\
	else	\
	{	\
		ov_logfile_error("%s: vectors have different lengths, operation not possible", pinst->v_identifier);	\
		Ov_SetDynamicVectorLength(&pinst->v_OUT.value.valueunion.val_##idsmall##_vec, 0, idcap);	\
	}
	