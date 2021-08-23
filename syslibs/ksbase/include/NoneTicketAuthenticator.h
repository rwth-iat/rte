
#ifndef KSBASE_NONETICKETAUTHENTICATOR_H_INCLUDED
#define KSBASE_NONETICKETAUTHENTICATOR_H_INCLUDED

/*
 * global pointer to the NoneTicketAuthenticator (this implies no problem since the authenticator is a sigleton)
 * it is needed for the ticket functions to find necessary data
 */
OV_DLLVARIMPORT OV_INSTPTR_ksbase_NoneTicketAuthenticator ksbase_NoneAuth;

#endif
