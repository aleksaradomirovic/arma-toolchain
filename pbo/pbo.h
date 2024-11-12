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

#include <limits.h>
#include <stdint.h>
#include <stdio.h>

struct pbo_property {
    char * key;
    char * value;

    struct pbo_property * next;
};

struct pbo_entry {
    char * path;

    uint32_t mime_type;
    uint32_t original_size;
    uint32_t offset;
    uint32_t time_stamp;
    uint32_t data_size;

    struct pbo_property * properties;
    
    struct pbo_entry * next;
};

_Static_assert(CHAR_BIT == 8);
_Static_assert(sizeof(uint32_t) == 4);

void free_pbo_entries(struct pbo_entry * list);
int list_pbo_entries(FILE * pbo, struct pbo_entry ** listptr);
