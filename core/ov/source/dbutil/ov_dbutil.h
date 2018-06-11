/*
 *   $Id: ov_dbutil.h, v1.0 2018-6-10$
 *
 *   Copyright (C) 1998
 *   Lehrstuhl fuer Prozessleittechnik,
 *   D-52056 Aachen, Germany.
 *   All rights reserved.
 *
 *   Author: Lars Nothdurft <l.nothdurft@plt.rwth-aachen.de>
 *
 *   This file is part of the ACPLT/OV Package
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 */
/*
 *	History:
 *	--------
 *	10-Jun-2018 Lars Nothdurft <l.nothdurft@plt.rwth-aachen.de>: File created.
 */

#ifndef OV_DBUTIL_H_
#define OV_DBUTIL_H_

OV_UINT ov_sizeof(OV_UINT vartype);
void ov_newline(FILE* handle,int rnum);

OV_RESULT ov_dump(OV_STRING name);
OV_RESULT ov_dumpFbd(OV_STRING name, OV_STRING startPath, OV_INT mode);

#endif /* OV_DBUTIL_H_ */
