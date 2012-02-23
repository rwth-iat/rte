//// some functions from xdr/rpc
//
//OV_BOOL xdr_u_long (register XDR *xdrs, long *var) {
//	if (xdrs->x_op == XDR_DECODE)
//		return (XDR_GETLONG(xdrs, (long *)var));
//	if (xdrs->x_op == XDR_ENCODE)
//		return (XDR_PUTLONG(xdrs, (long *)var));
//	if (xdrs->x_op == XDR_FREE)
//		return (TRUE);
//	return (FALSE);
//	
//}
