/* -*-plt-c++-*- */
/* $Header: /home/david/cvs/acplt/ks/src/mask.cpp,v 1.2 1998-04-20 08:27:05 nicole Exp $ */
/*
 * Copyright (c) 1996, 1997
 * Chair of Process Control Engineering,
 * Aachen University of Technology.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must print or display the above
 *    copyright notice either during startup or must have a means for
 *    the user to view the copyright notice.
 * 3. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the Chair of Process Control Engineering nor the
 *    name of the Aachen University of Technology may be used to endorse or
 *    promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE CHAIR OF PROCESS CONTROL ENGINEERING
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE CHAIR OF PROCESS CONTROL
 * ENGINEERING BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
                   break;
        case '?' : m++; n++;
                   break;
        default  : if (mask[m]=='\\') m++;
                   #if (PLT_IGNOR_UPCASE)
                       if (m>=l || (mask[m]!=toupper(name[n]) && mask[m]!=tolower(name[n]))
                           return false;
                   #else
                       if (m>=l || mask[m]!=name[n]) 
                           return false;
                   #endif;
                   m++; n++;
     }
  if (n==k && m==l) return true;
  if (m<l && mask[m]=='*') return (joker(name,n,m));
  return false;
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
                       return true;
                   } 
               for (i=tolower(mask[m+1]); i<=tolower(mask[m+3]); i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return true; 
                   }
           #else
               for (i=mask[m+1]; i<=mask[m+3]; i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return true;
                   }
           #endif 
           return false;
        } else if (isupper(mask[m+1]) && islower(mask[m+3]) && (mask[m+4]==']')) {
           #if (PLT_IGNOR_UPCASE)
               for (i=mask[m+1]; i<=toupper(mask[m+3]); i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return true;
                   }
               for (i=tolower(mask[m+1]); i<=mask[m+3]; i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return true;
                   }
           #else
               for (i=mask[m+1]; i<='Z'; i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return true;
                   }
               for (i='a'; i<=mask[m+3]; i++)
                   if (i==name[n]) {
                      n++; m+=5;
                      return true;
                   }
           #endif
           return false;
        } else if (islower(mask[m+1]) && isupper(mask[m+3]) && (mask[m+4]==']')) {
           #if (PLT_IGNOR_UPCASE)
               for (i=mask[m+1]; i<=tolower(mask[m+3]); i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return true;
                   }
               for (i=toupper(mask[m+1]); i<=mask[m+3]; i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return true;
                   }            
           #else
               for (i=mask[m+1]; i<='z'; i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return true;
                   }
               for (i='A'; i<=mask[m+3]; i++)
                   if (i==name[n]) {
                       n++; m+=5;
                       return true;
                   }
           #endif
           return false;
        } else return (-(m+1));
     } else {
        for (i=m+1; i<l && mask[i]!=']'; i++)
            #if (PLT_IGNOR_UPCASE)
                if (mask[i]==toupper(name[n]) || mask[i]==tolower(name[n])) {
                    while (i<l && mask[i]!=']') i++;
                    if (mask[i]==']') {
                        n++; m=i+1;
                        return true;
                    }
                    else return (-(m+1));
                }
            #else
                if (mask[i]==name[n]) {
                    while (i<l && mask[i]!=']') i++;
                    if (mask[i]==']') {
                        n++; m=i+1;
                        return true;
                    }
                    else return (-(m+1));
                }
            #endif
        if (mask[i]==']') return false;
        else return (-(m+1));
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
                       return false;
                   }
               for (i=tolower(mask[m+2]); i<=tolower(mask[m+4]); i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return false;
                   } 
           #else
               for (i=mask[m+2]; i<=mask[m+4]; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return false;
                   }
           #endif
           return true;
        } else if (isupper(mask[m+2]) && islower(mask[m+4]) && (mask[m+5]==']')) {
           #if (PLT_IGNOR_UPCASE)
               for (i=mask[m+2]; i<=toupper(mask[m+4]); i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return false;
                   }
               for (i=tolower(mask[m+2]); i<=mask[m+4]; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return false;
                   }
           #else
               for (i=mask[m+2]; i<='Z'; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return false;
                   }
               for (i='a'; i<=mask[m+4]; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return false;
                   }
           #endif
           return true;
        } else if (islower(mask[m+2]) && isupper(mask[m+4]) && (mask[m+5]==']')) {
           #if (PLT_IGNOR_UPCASE)
               for (i=mask[m+2]; i<=tolower(mask[m+4]); i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return false;
                   }
               for (i=toupper(mask[m+2]); i<=mask[m+4]; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return false;
                   }
           #else 
               for (i=mask[m+2]; i<='z'; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return false;
                   }
               for (i='A'; i<=mask[m+4]; i++)
                   if (i==name[n]) {
                       n++; m+=6;
                       return false;
                   }
           #endif
           return true;
        } else return (-(m+1));
     } else {
        for (i=m+2; i<l && mask[i]!=']'; i++)
            #if (PLT_IGNOR_UPCASE)
                if (mask[i]==toupper(name[n]) || mask[i]==tolower(name[n])) {
                    while (i<l && mask[i]!=']') i++;
                    if (mask[i]==']') {
                        n++; m=i+1;
                        return false;
                    }
                    else return (-(m+1));
                }     
            #else
                if (mask[i]==name[n]) {
                    while (i<l && mask[i]!=']') i++;
                    if (mask[i]==']') {
                        n++; m=i+1;
                        return false;
                    }
                    else return (-(m+1));
                }
            #endif  
        if (mask[i]==']') return true;
        else return (-(m+1));
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
        }
        else return false;
     }
     else m++;
  if ((n==k)&&(m+1!=l)) return false;
  if (m+1>=l) return true;

  int i;
  int l_rest=0;
  int set=false;
  for (i=m+1; i<l; i++)
      if (mask[i]=='[' && mask[i-1]!='\\')
         set=true;
      else if (mask[i]==']' && mask[i-1]!='\\') {
         set=false;
         l_rest++;
      } else if (!set && (mask[i]!='\\') && (mask[i] != '*' || mask[i-1]=='\\'))
         l_rest++;
  if (l_rest==0) return (-(m+1));
  if (mask[m+1]=='\\') m++;

  if (mask[m+1]!='[') {
     for (i=k-l_rest; i>=n; i--)
        if (mask[m+1]==name[i]) {
           r=m_loop(name,i+1,m+2);         
           if (r!=false) return r;
        }
     return false;
  } else {
     for (i=k-l_rest; i>=n; i--) {
         int h1=i;
         int h2=m+1;
         int r=KsMask::set(name,h1,h2);
         if (r==true) {
           r=m_loop(name,h1,h2);
           if (r!=false) return r;
         } else if (r<0) return r;
     }
     return false;
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






