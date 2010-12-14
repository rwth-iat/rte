

/******************* größer als ******************/

#define STDFB_GT(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall > pinst->v_IN2.value.valueunion.val_##idsmall)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
#define STDFB_GT_TIME(tim1, tim2)	\
	((tim1##.secs > tim2##.secs) || ((tim1##.secs == tim2##.secs) && (tim1##.usecs > tim2##.usecs)))
	
#define STDFB_GT_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen > pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
/******************* größer / gleich ******************/

#define STDFB_GE(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall >= pinst->v_IN2.value.valueunion.val_##idsmall)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
#define STDFB_GE_TIME(tim1, tim2)	\
	((tim1##.secs > tim2##.secs) || ((tim1##.secs == tim2##.secs) && (tim1##.usecs >= tim2##.usecs)))
	
#define STDFB_GE_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen >= pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
/******************* gleich ******************/

#define STDFB_EQ(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall == pinst->v_IN2.value.valueunion.val_##idsmall)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
#define STDFB_EQ_TIME(tim1, tim2)	\
	((tim1##.secs == tim2##.secs) && (tim1##.usecs == tim2##.usecs))
	
#define STDFB_EQ_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen == pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
/******************* kleiner / gleich ******************/

#define STDFB_LE(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall <= pinst->v_IN2.value.valueunion.val_##idsmall)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
#define STDFB_LE_TIME(tim1, tim2)	\
	((tim1##.secs < tim2##.secs) || ((tim1##.secs == tim2##.secs) && (tim1##.usecs <= tim2##.usecs)))
	
#define STDFB_LE_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen <= pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;

/******************* kleiner als ******************/

#define STDFB_LT(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall < pinst->v_IN2.value.valueunion.val_##idsmall)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
#define STDFB_LT_TIME(tim1, tim2)	\
	((tim1##.secs < tim2##.secs) || ((tim1##.secs == tim2##.secs) && (tim1##.usecs < tim2##.usecs)))
	
#define STDFB_LT_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen < pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
	
/******************* ungleich ******************/

#define STDFB_NE(idcap, idsmall)	\
	case OV_VT_##idcap:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall != pinst->v_IN2.value.valueunion.val_##idsmall)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
#define STDFB_NE_TIME(tim1, tim2)	\
	((tim1##.secs != tim2##.secs) || (tim1##.usecs != tim2##.usecs))
	
#define STDFB_NE_VEC(idcap, idsmall)	\
	case OV_VT_##idcap##_VEC:	\
		if(pinst->v_IN1.value.valueunion.val_##idsmall##_vec.veclen != pinst->v_IN2.value.valueunion.val_##idsmall##_vec.veclen)	\
			pinst->v_OUT = TRUE;	\
		else	\
			pinst->v_OUT = FALSE;	\
	break;
	
	
