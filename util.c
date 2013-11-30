/*
 *  BitDiff simple helper. 
 *  Copyright (C) 2013, Andrew 'Necromant' Andrianov <spam@ncrmnt.org>
 *
 * This software has been developed for the LART computing board
 * (http://www.lart.tudelft.nl/). The development has been sponsored by
 * the Mobile MultiMedia Communications (http://www.mmc.tudelft.nl/)
 * and Ubiquitous Communications (http://www.ubicom.tudelft.nl/)
 * projects.
 *
 * The author can be reached at:
 *
 *  Jan-Derk Bakker
 *  Information and Communication Theory Group
 *  Faculty of Information Technology and Systems
 *  Delft University of Technology
 *  P.O. Box 5031
 *  2600 GA Delft
 *  The Netherlands
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void print_bitdiff(uint32_t a, uint32_t b)
{
	int i;
	uint32_t tmp = a ^ b;
	printf("     BitDiff 0x%x vs 0x%x\n", a, b);
	printf("------------------------------\n");
	while (tmp) {
		int pos = ffs(tmp) - 1;
		int change = (b & (1 << pos));
		printf("Bit %d changed from %s\n", 
		       pos, change ? "0 to 1" : "1 to 0");
		tmp &= ~(1<<pos);
	}
}
