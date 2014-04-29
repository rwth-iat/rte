/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/mask.cpp,v 1.8 2007-04-25 12:57:20 martin Exp $ */
/*
 * Copyright (c) 1996, 1997, 1998, 1999
 * Lehrstuhl fuer Prozessleittechnik, RWTH Aachen
 * D-52064 Aachen, Germany.
 * All rights reserved.
 *
 * This file is part of the ACPLT/KS Package which is licensed as open
 * source under the Artistic License; you can use, redistribute and/or
 * modify it under the terms of that license.
 *
 * You should have received a copy of the Artistic License along with
 * this Package; see the file ARTISTIC-LICENSE. If not, write to the
 * Copyright Holder.
 *
 * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES
 * OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
/*
 * Author: Nicole Boemeke
 */

#include "ks/mask.h"
#include <string.h>
#include <ctype.h>


//////////////////////////////////////////////////////////////////////

int
 KsMask::matches(const char *name) const 
{
  return m_loop(name, 0, 0);
}

////////////////////////////////////////////////////////////////////

int
KsMask::m_loop(const char *name, int n, int m) const 
{
  int r;
  int k=strlen(name);
  int l=len();
  const char *mask = p->s;
 
  while (n<k && m<l) 
     switch (mask[m]) {
        case '[' : r=set(name,n,m);
                   if (!r) return r;
                   break;
        case '*' : r=joker(name,n,m);
                   return r;
                   //break;
        case '?' : m++; n++;
                   break;
        default  : if (mask[m]=='\\') m++;
                   #if (PLT_IGNOR_UPCASE)
                       if (m>=l || (mask[m]!=toupper(name[n]) && mask[m]!=tolower(name[n])))
                           return 0;
                   #else
                       if (m>=l || mask[m]!=name[n]) 
                           return 0;
                   #endif
                   m++; n++;
     }
  if (n==k && m==l) return 1;
  if (m<l && mask[m]=='*') return (joker(name,n,m));
  return 0;
}

//////////////////////////////////////////////////////////////////////

int
KsMask::set(const char *name, int &n, int &m) const 
{
  int l=len();
  const char *mask = p->s;
  if (l<=m+2) return (-(m+1));
  
  int i;
  if (mask[m+1]!='^') {
     if (m+5<=l && mask[m+2]=='-') {
        if ((( isdigit(mask[m+1]) && isdigit(mask[m+3]) ) ||
             ( isupper(mask[m+1]) && isupper(mask[m+3]) ) ||
             ( islower(mask[m+1]) && islower(mask[m+3]) )) && (mask[m+4]==']')) {         
           #if (PLT_IGNOR_UPCASE)
               for (i=toupper(mask[m+1]); i<=toupper(mask[m+3]); i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return 1;
                   } 
               for (i=tolower(mask[m+1]); i<=tolower(mask[m+3]); i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return 1; 
                   }
           #else
               for (i=mask[m+1]; i<=mask[m+3]; i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return 1;
                   }
           #endif 
           return false;
        } else if (isupper(mask[m+1]) && islower(mask[m+3]) && (mask[m+4]==']')) {
           #if (PLT_IGNOR_UPCASE)
               for (i=mask[m+1]; i<=toupper(mask[m+3]); i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return 1;
                   }
               for (i=tolower(mask[m+1]); i<=mask[m+3]; i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return 1;
                   }
           #else
               for (i=mask[m+1]; i<='Z'; i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return 1;
                   }
               for (i='a'; i<=mask[m+3]; i++)
                   if (i==name[n]) {
                      n++; m+=5;
                      return 1;
                   }
           #endif
           return false;
        } else if (islower(mask[m+1]) && isupper(mask[m+3]) && (mask[m+4]==']')) {
           #if (PLT_IGNOR_UPCASE)
               for (i=mask[m+1]; i<=tolower(mask[m+3]); i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return 1;
                   }
               for (i=toupper(mask[m+1]); i<=mask[m+3]; i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return 1;
                   }            
           #else
               for (i=mask[m+1]; i<='z'; i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return 1;
                   }
               for (i='A'; i<=mask[m+3]; i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return 1;
                   }
           #endif
           return 0;
        } else return (-(m+1));
     } else {
        for (i=m+1; i<l && mask[i]!=']'; i++)
            #if (PLT_IGNOR_UPCASE)
                if( (mask[i]==toupper(name[n])) || (mask[i]==tolower(name[n])) ) {
                    while (i<l && mask[i]!=']') i++;
                    if (mask[i]==']') {
                        n++; m=i+1;
                        return 1;
                    } else {
                        return (-(m+1));
                    }
                }
            #else
                if (mask[i]==name[n]) {
                    while (i<l && mask[i]!=']') i++;
                    if (mask[i]==']') {
                        n++; m=i+1;
                        return 1;
                    } else {
                        return (-(m+1));
                    }
                }
            #endif
        if (mask[i]==']') {
            return 0;
        }
        return (-(m+1));
     }
  } else {         
     if (m+6<=l && mask[m+3]=='-') {
        if ((( isdigit(mask[m+2]) && isdigit(mask[m+4]) ) ||
             ( isupper(mask[m+2]) && isupper(mask[m+4]) ) ||
             ( islower(mask[m+2]) && islower(mask[m+4]) )) && (mask[m+5]==']')) {         
           #if (PLT_IGNOR_UPCASE)
               for (i=toupper(mask[m+2]); i<=toupper(mask[m+4]); i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return 0;
                   }
               for (i=tolower(mask[m+2]); i<=tolower(mask[m+4]); i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return 0;
                   } 
           #else
               for (i=mask[m+2]; i<=mask[m+4]; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return 0;
                   }
           #endif
           return 1;
        } else if (isupper(mask[m+2]) && islower(mask[m+4]) && (mask[m+5]==']')) {
           #if (PLT_IGNOR_UPCASE)
               for (i=mask[m+2]; i<=toupper(mask[m+4]); i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return 0;
                   }
               for (i=tolower(mask[m+2]); i<=mask[m+4]; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return 0;
                   }
           #else
               for (i=mask[m+2]; i<='Z'; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return 0;
                   }
               for (i='a'; i<=mask[m+4]; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return 0;
                   }
           #endif
           return 1;
        } else if (islower(mask[m+2]) && isupper(mask[m+4]) && (mask[m+5]==']')) {
           #if (PLT_IGNOR_UPCASE)
               for (i=mask[m+2]; i<=tolower(mask[m+4]); i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return 0;
                   }
               for (i=toupper(mask[m+2]); i<=mask[m+4]; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return 0;
                   }
           #else 
               for (i=mask[m+2]; i<='z'; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return 0;
                   }
               for (i='A'; i<=mask[m+4]; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return 0;
                   }
           #endif
           return 1;
        } else {
            return (-(m+1));
        }
     } else {
        for (i=m+2; i<l && mask[i]!=']'; i++)
            #if (PLT_IGNOR_UPCASE)
                if (mask[i]==toupper(name[n]) || mask[i]==tolower(name[n])) {
                    while (i<l && mask[i]!=']') i++;
                    if (mask[i]==']') {
                        n++; m=i+1;
                        return 0;
                    } else {
                        return (-(m+1));
                    }
                }     
            #else
                if (mask[i]==name[n]) {
                    while (i<l && mask[i]!=']') i++;
                    if (mask[i]==']') {
                        n++; m=i+1;
                        return false;
                    } else {
                        return (-(m+1));
                    }
                }
            #endif  
        if (mask[i]==']') {
            return 1;
        }
        
        return (-(m+1));
     }
  }                     
}              

////////////////////////////////////////////////////////////////////////

int 
KsMask::joker(const char *name, int &n, int &m) const 
{
  int r;
  int l=len();
  const char *mask = p->s;

  int k=strlen(name);
  while (m+1<l && ((mask[m+1]=='?') || (mask[m+1]=='*')))
     if (mask[m+1]=='?') {
        if (n<k) { 
           n++;
           m++;
        } else {
            return 0;
        }
     }
     else m++;
  if ((n==k)&&(m+1!=l)) return 0;
  if (m+1>=l) return 1;

  int i;
  int l_rest=0;
  int set=0;
  for (i=m+1; i<l; i++)
      if (mask[i]=='[' && mask[i-1]!='\\')
         set=1;
      else if (mask[i]==']' && mask[i-1]!='\\') {
         set=0;
         l_rest++;
      } else if (!set && (mask[i]!='\\') && (mask[i] != '*' || mask[i-1]=='\\'))
         l_rest++;
  if (l_rest==0) {
    return (-(m+1));
  }
  if (mask[m+1]=='\\') m++;

  if (mask[m+1]!='[') {
     for (i=k-l_rest; i>=n; i--)
        if (mask[m+1]==name[i]) {
           r=m_loop(name,i+1,m+2);         
           if (r!=0) return r;
        }
     return 0;
  } else {
     for (i=k-l_rest; i>=n; i--) {
         int h1=i;
         int h2=m+1;
         int r=KsMask::set(name,h1,h2);
         if (r==1) {
           r=m_loop(name,h1,h2);
           if (r!=0) return r;
         } else if (r<0) return r;
     }
     return 0;
  } 
}               

///////////////////////////////////////////////////////////////////////
#if 0

#include <stdio.h>

int main(int, char**)
{ 
    char n[30], m[30];
    
    printf("Name ? "); scanf("%s",n);
    printf("Maske ? "); scanf("%s",m);          
    
    PltString str(m);
    KsMask p(str);      
    
    int r=p.matches(n);
    if (r==1) 
        printf("%s passt auf %s \n",(const char *)p,n);
    else if (r==0)
        printf("%s passt nicht auf %s \n",(const char*)p,n);
    else
        printf("Fehler an Position %d\n",-r);
    return 0;
} 
#endif

//////////////////////////////////////////////////////////////////////
// EOF ks/mask






