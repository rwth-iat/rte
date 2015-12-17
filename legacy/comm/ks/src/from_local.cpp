/*
 * Adapted from:
 * @(#) from_local.c 1.3 96/05/31 15:52:57 (Linux networking base)
 */

/*
 * Extended for the "NT operating system" by Harald Albrecht of the
 * Chair of Process Control Engineering. The idea of how to get all
 * host IP addresses on NT was independently developed and then proofed
 * by the NT port of nntp.
 *
 * Also, some minor changes made in the old functions to take advantage
 * of the PltLog objects.
 *
 * $Header: /home/david/cvs/acplt/ks/src/from_local.cpp,v 1.10 2007-04-25 12:57:20 martin Exp $
 */

/*
 * Check if an address belongs to the local system. Adapted from:
 * 
 * @(#)pmap_svc.c 1.32 91/03/11 Copyright 1984,1990 Sun Microsystems, Inc.
 * @(#)get_myaddress.c  2.1 88/07/29 4.0 RPCSRC.
 */

/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user or with the express written consent of
 * Sun Microsystems, Inc.
 *
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

/*
 * First, some PLT stuff which helps us to in the various os environments.
 */
#include "plt/debug.h"
#include "plt/log.h"


#ifdef TEST
#undef perror
#endif


#if PLT_SYSTEM_OPENVMS

#include <in.h>

/* 
 * Currently we have no means to get this information.
 * For OpenVMS > 7.0 there might be the right ioctl's in
 * SYS$LIBRARY:UCX$INETDEF.H and/or <ioctl.h>.
 */

int from_local(struct sockaddr_in *) 
{
  /* TODO */
	return 1;
}


#else


#if PLT_SYSTEM_NT

/*
 * The Windooze stuff...
 */
#include <windows.h>
#include <winsock.h>
#include "plt/string.h"

#else

/*
 * Now, finally for the Un*x stuff...
 */
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

#if PLT_SYSTEM_SOLARIS
#include <sys/sockio.h>
#endif

#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>

#endif /* of all that Un*x, NT stuff */

 /*
  * With virtual hosting, each hardware network interface can have multiple
  * network addresses. On such machines the number of machine addresses can
  * be surprisingly large.
  */
static int num_local;
static int num_addrs;
static struct in_addr *addrs;

/* grow_addrs - extend list of local interface addresses */

static bool grow_addrs()
{
    struct in_addr *new_addrs;
    int     new_num;

    /*
     * Keep the previous result if we run out of memory. The system would
     * really get hosed if we simply give up.
     */
    new_num = (addrs == 0) ? 1 : num_addrs + num_addrs;
    new_addrs = (struct in_addr *) malloc(sizeof(*addrs) * new_num);
    if (new_addrs == 0) {
	PltLog::Error("from_local: out of memory");
	return false;
    } else {
	if (addrs != 0) {
	    memcpy((char *) new_addrs, (char *) addrs,
		   sizeof(*addrs) * num_addrs);
	    free((char *) addrs);
	}
	num_addrs = new_num;
	addrs = new_addrs;
	return true;
    }
}

#if PLT_SYSTEM_NT

/*
 * This is the find_local() variant for Windooze NT. It is really
 * brain-damaged, but -- honestly -- what else do you expect from Bill's
 * operating systems after all? Interestingly enough, NT doesn't get along
 * very well with the concept of network interfaces which can be brought up
 * and down during normal operation without rebooting and other lousy
 * problems. If Bill just have had a look at Linux, he would have thrown
 * NT out of his "Windows"...
 * ...like Marianne 013 said: "What a lousy system!"
 */
int find_local()
{
    //
    // First, we must find out which devices (aka network cards) are
    // currently bound to the TCP/IP protocol. This is a really strange
    // way, because m$ has definetely NO CONCEPT about really how a
    // real network OS should be structured. If Billy Boy thinks that
    // the registry needs a change, everything will break, as there is
    // no official way to determine the IP addresses, etc. This is just
    // brain-damaged, thanks Bill!
    //
    HKEY   hLinkages;
    DWORD  RegValueType, RegValueLength;
    char  *TcpIpBindings;

    if ( RegOpenKey(HKEY_LOCAL_MACHINE,
		    "SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Linkage", &hLinkages) 
	 != ERROR_SUCCESS ) {
	PltLog::Error("Can't open TCP/IP linkage registry key");
	return 0;
    }
    //
    // Next, find out how long the value of the "Bind" entry will be and
    // allocate enough memory to hold the value. Then retrieve the value
    // from the registry. Yeah, once again, programming NT is "easy as a
    // pie"... well, HAL 9000 proves this.
    //
    if ( RegQueryValueEx(hLinkages, "Bind",
			 0, &RegValueType,
			 0, &RegValueLength) != ERROR_SUCCESS ) {
	RegCloseKey(hLinkages);
	PltLog::Error("Can't get TCP/IP linkage binding value from the registry");
	return 0;
    }
    if ( RegValueType != REG_MULTI_SZ ) {
	RegCloseKey(hLinkages);
	PltLog::Alert("The TCP/IP linkage binding value is not of type REG_MULTI_SZ. Looks like your registry was corrupted.");
	return 0;
    }
    TcpIpBindings = new char[RegValueLength];
    if ( TcpIpBindings == 0 ) {
	RegCloseKey(hLinkages);
	PltLog::Error("Out of memory to get TCP/IP linkage binding value");
	return 0;
    }
    if ( RegQueryValueEx(hLinkages, "Bind",
			 0, 0,
			 (LPBYTE) TcpIpBindings, &RegValueLength) != ERROR_SUCCESS ) {
	RegCloseKey(hLinkages);
	delete [] TcpIpBindings;
	PltLog::Error("Can't get TCP/IP linkage binding value from the registry");
	return 0;
    }
    RegCloseKey(hLinkages);
    //
    // Now that we've got the bindings, we must scan them to find out
    // what services are bound to TCP/IP...
    //
    char  *device = TcpIpBindings;
    char  *service;
    int    serviceLength;
    char  *IpAddresses = 0;
    DWORD  IpAddressesLength, IpAddressesAllocated = 0;
    HKEY   hService;

    num_local = 0;
    if ( addrs ) {
	free((char *) addrs);
    }
    addrs = 0;
    for ( ; *device ; device += serviceLength + 1 ) {
	serviceLength = strlen(device);
	service       = strrchr(device, '\\');
	if ( service ) {
	    //
	    // Skip NDISWANxxx services as these are temporary interfaces
	    // like RAS and we would not like to fight with all the
	    // problems associated with such temporary things.
	    //
	    if ( (serviceLength >= 7) &&
		 (strncmpi(service, "NDISWAN", 7) == 0) ) {
		continue;
	    }
	    //
	    // Okay, this seems to be a permanent service... We're silently
	    // ignoring a failed access because we then can assume that it
	    // is not a network card...
	    //
	    PltString TcpIp("SYSTEM\\CurrentControlSet\\Services", service);
	    TcpIp += "\\Parameters\\Tcpip";
	    if ( RegOpenKey(HKEY_LOCAL_MACHINE, TcpIp, &hService)
		 != ERROR_SUCCESS ) {
		continue;
	    }
	    //
	    // First, get the list of IP addresses bound to this interface
	    // (aka service, or whatever, who cares...). We do use here
	    // dynamically allocated memory to hold the registry information.
	    // This block of memory grows, if the current value is bigger
	    // than the last. This way we can cope with (almost) arbitrary
	    // sizes...
	    //
	    if ( RegQueryValueEx(hService, "IpAddress",
				 0, &RegValueType,
				 0, &IpAddressesLength) != ERROR_SUCCESS ) {
		RegCloseKey(hService);
		PltLog::Error("Can't get IP address for service");
		PltLog::Error(service);
		continue;
	    }
	    if ( RegValueType != REG_MULTI_SZ ) {
		RegCloseKey(hService);
		PltLog::Alert("The IP address' value is not of type REG_MULTI_SZ. Looks like your registry was corrupted.");
		continue;
	    }
	    if ( IpAddressesLength > IpAddressesAllocated ) {
		if ( IpAddresses ) {
		    delete [] IpAddresses;
		}
		IpAddressesAllocated = IpAddressesLength;
		IpAddresses = new char[IpAddressesAllocated];
		if ( IpAddresses == 0 ) {
		    break;
		}
	    }
	    if ( RegQueryValueEx(hService, "IpAddress",
				 0, 0,
				 (LPBYTE) IpAddresses, &IpAddressesLength)
		 != ERROR_SUCCESS ) {
		RegCloseKey(hService);
		PltLog::Error("Can't get IP address for service");
		PltLog::Error(service);
		continue;
	    }
	    RegCloseKey(hService);
	    //
	    // Now we can parse the IP addresses and store them into our
	    // list of local addresses.
	    //
	    char          *pIp;
	    int            IpLength;
	    unsigned long  ip;

	    for ( pIp = IpAddresses ; *pIp ; pIp += IpLength + 1 ) {
		IpLength = strlen(pIp);
		ip = inet_addr(pIp);
		if ( ip == INADDR_NONE ) {
		    continue;
		}
		if ( num_local >= num_addrs ) {
		    if ( !grow_addrs() ) {
			break;
		    }
		}
		addrs[num_local].s_addr = ip;
		num_local++;
	    }
	}
    }

    if ( IpAddresses ) {
	delete [] IpAddresses;
    }
    delete [] TcpIpBindings;
    return num_local;
} // find_local

#else

/*
 * find_local - find all IP addresses for this host the Un*x way. Compared
 *              to what we need to do on "NT", this is considerably more
 *              straight and secure against new os releases.
 */
int find_local()
{
    struct ifconf ifc;
    struct ifreq ifreq;
    struct ifreq *ifr;
    struct ifreq *the_end;
    int     sock;
    char    buf[BUFSIZ];

    /*
     * Get list of network interfaces. We use a huge buffer to allow for the
     * presence of non-IP interfaces.
     */

    if ((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
	PltLog::Error("from_local: can't allocate socket");
	return (0);
    }
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    if (ioctl(sock, SIOCGIFCONF, (char *) &ifc) < 0) {
	PltLog::Error("from_local: SIOCGIFCONF: can't get interface configurations");
	(void) close(sock);
	return (0);
    }
    /* Get IP address of each active IP network interface. */

    the_end = (struct ifreq *) (ifc.ifc_buf + ifc.ifc_len);
    num_local = 0;
    for (ifr = ifc.ifc_req; ifr < the_end; ifr++) {
	if (ifr->ifr_addr.sa_family == AF_INET) {	/* IP net interface */
	    ifreq = *ifr;
	    if (ioctl(sock, SIOCGIFFLAGS, (char *) &ifreq) < 0) {
		PltLog::Error("from_local: SIOCGIFFLAGS: can't get interface configurations");
	    } else if (ifreq.ifr_flags & IFF_UP) {	/* active interface */
		if (ioctl(sock, SIOCGIFADDR, (char *) &ifreq) < 0) {
		PltLog::Error("from_local: SIOCGIFADDR: can't get interface address");
		} else {
		    if (num_local >= num_addrs)
			if ( !grow_addrs() )
			    break;
		    addrs[num_local++] = ((struct sockaddr_in *)
					  & ifreq.ifr_addr)->sin_addr;
		}
	    }
	}
	/* Support for variable-length addresses. */
#ifdef HAS_SA_LEN
	ifr = (struct ifreq *) ((caddr_t) ifr
		      + ifr->ifr_addr.sa_len - sizeof(struct sockaddr));
#endif
    }
    (void) close(sock);
    return (num_local);
}

#endif /* Un*x from_local */

/* from_local - determine whether request comes from the local system */

bool from_local(struct sockaddr_in *addr)
{
    int     i;

    if (addrs == 0 && find_local() == 0)
	PltLog::Error("cannot find any active local network interfaces");

    for (i = 0; i < num_local; i++) {
	if (memcmp((char *) &(addr->sin_addr), (char *) &(addrs[i]),
		   sizeof(struct in_addr)) == 0)
	    return true;
    }
    return false;
}

#ifdef TEST

main()
{
    char   *inet_ntoa();
    int     i;

    find_local();
    for (i = 0; i < num_local; i++)
	printf("%s\n", inet_ntoa(addrs[i]));
}

#endif /* TEST */
#endif

