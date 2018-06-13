/*
 * libreGrid - open (linux) software for the NZXT GRID+ V3
 * Copyright (C) 2018  Thomas Berger <loki@lokis-chaos.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBREGRID_USB_MACROS_H
#define LIBREGRID_USB_MACROS_H

// helper macro for simpler usb function calls
#define CHECK_OR_ACTION(call, ...) \
   if ( call < 0 ) { __VA_ARGS__; }
#define CHECK_OR_THROW(call, msg) \
  CHECK_OR_ACTION(call, throw(std::runtime_error(msg)))

#define CHECK_OR_ERRORRET(call, msg) \
  CHECK_OR_ACTION(call, qWarning(msg); return )

#endif // LIBREGRID_USB_MACROS_H
