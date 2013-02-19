/* Copyright 2010 Antonie Jovanoski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Contact e-mail: Antonie Jovanoski <minimoog77_at_gmail.com>
 */

#ifndef QTWEETLIB_GLOBAL_H
#define QTWEETLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#ifndef QTWEETLIBSHARED_EXPORT 
# if defined (QTWEETLIB_MAKEDLL)
#  define QTWEETLIBSHARED_EXPORT Q_DECL_EXPORT
# else
#  define QTWEETLIBSHARED_EXPORT Q_DECL_IMPORT
# endif
#endif

#define AUTH_HEADER "Authorization"

#endif // QTWEETLIB_GLOBAL_H
