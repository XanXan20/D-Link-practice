#include "ipdata.h"
#include <stdio.h>
#include <math.h>

#ifndef IPFUNC_H
#define IPFUNC_H

void apply_mask(ipdata data, int * net_addr);

void check_for_address(ipdata data);
void check_for_private(ipdata data);
void check_for_broadcast(ipdata data);

#endif