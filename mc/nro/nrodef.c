/*
 *	external "common" for NRO word processor
 *
 *	Stephen L. Browning
 *	5723 North Parker Avenue
 *	Indianapolis, Indiana 46220
 */

struct docctl dc;
struct page pg;
FILE *oub;
FILE *pout;
struct cout co;
FILE *sofile[NFILES];	/* input file buffers	*/
struct macros mac;
