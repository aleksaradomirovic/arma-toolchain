/*
 * This is a collection of Arma 3 modding tools.
 * Copyright (C) 2024  Aleksa Radomirovic
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, 51 Franklin Street,
 * Fifth Floor, Boston, MA 02110-1301, USA.
 */
#pragma once

#include <stdio.h>

#define COPYRIGHT_YEARS "2024"
#define COPYRIGHT_CONTRIBUTORS "Aleksa Radomirovic"

static void print_copyright_info() {
    printf(
        "pbo (Arma Toolchain)\n"
        "Copyright (C) %s %s\n"
        "This program is licensed under GNU GPL version 2 or later <https://www.gnu.org/licenses/old-licenses/gpl-2.0.html>\n"
        "This is free software; you are free to change and redistribute it.\n"
        "There is NO WARRANTY, to the extent permitted by law.\n",

        COPYRIGHT_YEARS, COPYRIGHT_CONTRIBUTORS
    );
}
