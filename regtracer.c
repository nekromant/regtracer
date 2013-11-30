/*
 *  Dumb and ugly memory raper with change history recorder. 
 *  Copyright (C) 2013, Andrew 'Necromant' Andrianov <spam@ncrmnt.org>
 *  Based on: 
 *  devmem2.c: Simple program to read/write from/to any location in memory.
 *
 *  Copyright (C) 2000, Jan-Derk Bakker (J.D.Bakker@its.tudelft.nl)
 *
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
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdint.h>


#define FATAL do { fprintf(stderr, "Error at line %d, file %s (%d) [%s]\n", \
			   __LINE__, __FILE__, errno, strerror(errno)); exit(1); } while(0)
 
#define MAP_SIZE 4096UL
#define MAP_MASK (MAP_SIZE - 1)



unsigned long history[8192];
int hpos = 0;
off_t target;
int detailed = 0;

void print_bitdiff(uint32_t a, uint32_t b);

void handle_signal(int s)
{
	int i;
	uint32_t prev = history[0];
	printf("Logged %d value change events\n", hpos);
	for (i=0; i<=hpos; i++)
	{
		printf("devmem 0x%X 32 0x%X\n", target, history[i]);
	}
	if (detailed) {
		for (i=1; i<=hpos; i++)
		{
			print_bitdiff(prev, history[i]);
			prev = history[i];
		}
	}
	exit(0);
}



int main(int argc, char **argv) {
	int fd;
	void *map_base, *virt_addr; 
	unsigned long read_result, writeval;
	int access_type = 'w';
	
	if(argc < 2) {
		fprintf(stderr, "\nDumb physical memory tracer\n");
		fprintf(stderr, "\n(c) Necromant 2013 :: ncrmnt.org \n");
		fprintf(stderr, "\nUsage:\t%s address [--detailed] \n"
			"\taddress : memory address to rape\n",
			argv[0]);
		exit(1);
	}

	target = strtoul(argv[1], 0, 0);
	if (argc > 2) 
		detailed++;
	
	if((fd = open("/dev/mem", O_RDWR | O_SYNC)) == -1) FATAL;
	printf("/dev/mem opened.\n"); 
	fflush(stdout);
    
	/* Map one page */
	map_base = mmap(0, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, target & ~MAP_MASK);
	if(map_base == (void *) -1) FATAL;
	printf("Raping mapped memory at address %p.\n", map_base); 
	fflush(stdout);
	virt_addr = map_base + (target & MAP_MASK);

	signal(SIGINT, handle_signal);

	history[0] = *((unsigned long *) virt_addr);
    
	while (1) {
		read_result = *((unsigned long *) virt_addr);
		if (read_result != history[hpos])
			history[++hpos] = read_result;

		/* yes, 8192 changes will screw us up,
		   but I wanted to keep loop contents as mininal as
		   I could. So NO BOUNDS CHECKING HERE.
		*/
	}

	
	if(munmap(map_base, MAP_SIZE) == -1) 
		FATAL;

	close(fd);
	return 0;
}

