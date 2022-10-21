/*
 *	Text processing portion of NRO word processor
 *
 *	Stephen L. Browning
 *	5723 North Parker Avenue
 *	Indianapolis, Indiana 46220
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "nro.h"
#include "nrocom.c"

text(p)
char *p;
{
	int i;
	char wrdbuf[MAXLINE];

	if (*p == ' ' || *p == '\n' || *p == '\r')
		leadbl(p);
	expesc(p,wrdbuf);
	if (dc.ulval > 0)
	{
		/*
		*	Because of the way underlining is handled,
		*	MAXLINE should be declared to be three times
		*	larger than the longest expected input line
		*	for underlining.  Since many of the character
		*	buffers use this parameter, a lot of memory
		*	can be allocated when it may not really be
		*	needed.  A MAXLINE of 180 would allow about
		*	60 characters in the output line to be
		*	underlined (remember that only alphanumerics
		*	get underlined - no spaces or punctuation).
		*/
		underl(p,wrdbuf,MAXLINE);
		--dc.ulval;
	}
	if (dc.cuval > 0)
	{
		underl(p,wrdbuf,MAXLINE);
		--dc.cuval;
	}
	if (dc.boval > 0)
	{
		bold(p,wrdbuf,MAXLINE);
		--dc.boval;
	}
	if (dc.ceval > 0)
	{
		center(p);
		put(p);
		--dc.ceval;
	}
	else if (*p == '\r' || *p == '\n')
		put(p); 			/* all blank line */
	else if (dc.fill == NO)
		put(p);				/* unfilled */
	else
	{
		while ((i = getwrd(p,wrdbuf)) > 0)
		{
			putwrd(wrdbuf);
			p += i;
		}
	}
}


/*
 *	insert bold face text
 */

bold(p0,p1,size)
char *p0, *p1;
int size;
{
	int i, j;

	j = 0;
	for (i=0; (p0[i] != '\n') && (j < size-1); ++i)
	{
		if (isalpha(p0[i]) || isdigit(p0[i]))
		{
			p1[j++] = p0[i];
			p1[j++] = '\b';
		}
		p1[j++] = p0[i];
	}
	p1[j++] = '\n';
	p1[j] = '\0';
	while (*p1 != '\0')
		*p0++ = *p1++;
	*p0 = '\0';
}




/*
 *	center a line by setting tival
 */

center(p)
char *p;
{
	dc.tival = max((dc.rmval + dc.tival - width(p)) >> 1,0);
}


/*
 *	expand title buffer to include character string
 */

expand(p0,c,s)
char *p0;
char c;
char *s;
{
	char tmp[MAXLINE];
	char *p, *q, *r;

	p = p0;
	q = tmp;
	while (*p != '\0')
	{
		if (*p == c)
		{
			r = s;
			while (*r != '\0')
				*q++ = *r++;
		}
		else
			*q++ = *p;
		++p;
	}
	*q = '\0';
	strcpy(p0,tmp);		/* copy it back */
}


/*
 *	get field from title
 */

char *getfield(p,q,delim)
char *p, *q;
char delim;
{
	while (*p != delim && *p != '\r' && *p != '\n' && *p != '\0')
	{
		*q++ = *p++;
	}
	*q = '\0';
	if (*p == delim)
		++p;
	return(p);
}



/*
 *	get non-blank word from p0 into p1.
 *	return number of characters processed.
 */

getwrd(p0,p1)
char *p0,*p1;
{
	int i;
	char *p, c;

	i = 0;
	while (*p0 == ' ' || *p0 == '\t')
	{
		++i;
		++p0;
	}
	p = p0;
	while (*p0 != ' ' && *p0 != '\0' && *p0 != '\t')
	{
		if(*p0 == '\n' || *p0 == '\r')
			break;
		*p1 = *p0++;
		++p1;
		++i;
	}
	c = *(p1-1);
	if(c == '"')
		c = *(p1-2);
	if (c == '?' || c == '!')
	{
		*p1++ = ' ';
		++i;
	}
	if (c == '.' && (*p0 == '\n' || *p0 == '\r' || islower(*p)))
	{
		*p1++ = ' ';
		++i;
	}
	*p1 = '\0';
	return(i);
}


/*
 *	convert integer to decimal ascii string
 */

itoda(value,p,size)
int value;
char *p;
int size;
{
	char c[7];
	int i, j, k;
	int aval;

	aval = abs(value);
	c[0] = '\0';
	i = 1;
	do
	{
		c[i++] = (aval % 10) + '0';
		aval /= 10;
	} while (aval > 0 && i <= size);
	if(value < 0 && i <= size)
		c[i++] = '-';
	for(j=0; j<i; ++j)
		*p++ = c[i-j-1];
	return(i);
}


/*
 *	center title text into print buffer
 */

justcntr(p,q,limit)
char *p, *q;
int limit[];
{
	int len;

	len = width(p);
	q = &q[(limit[RIGHT] + limit[LEFT] - len) >> 1];
	while(*p != '\0')
		*q++ = *p++;
}



/*
 *	left justify title text into print buffer
 */

justleft(p,q,limit)
char *p, *q;
int limit;
{
	q = &q[limit];
	while (*p != '\0')
		*q++ = *p++;
}


/*
 *	right justify title text into print buffer
 */

justrite(p,q,limit)
char *p, *q;
int limit;
{
	int len;

	len = width(p);
	q = &q[limit - len];
	while(*p != '\0')
		*q++ = *p++;
}




/*
 *	delete leading blanks, set tival
 */

leadbl(p)
char *p;
{
	int i,j;

	brek();
	for (i=0; p[i] == ' '; ++i)
		;
	if(p[i] != '\n' && p[i] != '\r')
		dc.tival = i;
	for(j=0; p[i] != '\0'; ++j)
		p[j] = p[i++];
	p[j] = '\0';
}
/*
 *	put out page footer
 */

pfoot()
{
	if (dc.prflg == TRUE)
	{
		skip(pg.m3val);
		if (pg.m4val > 0)
		{
			if ((pg.curpag % 2) == 0)
			{
				puttl(pg.efoot,pg.eflim,pg.curpag);
			}
			else
			{
				puttl(pg.ofoot,pg.oflim,pg.curpag);
			}
			skip(pg.m4val - 1);
		}
	}
}



/*
 *	put out page header
 */

phead()
{
	pg.curpag = pg.newpag;
	if(pg.curpag >= pg.frstpg && pg.curpag <= pg.lastpg)
	{
		dc.prflg = TRUE;
	}
	else
	{
		dc.prflg = FALSE;
	}
	++pg.newpag;
	if(dc.prflg == TRUE)
	{
		if(pg.m1val > 0)
		{
			skip(pg.m1val - 1);
			if((pg.curpag % 2) == 0)
			{
				puttl(pg.ehead,pg.ehlim,pg.curpag);
			}
			else
			{
				puttl(pg.ohead,pg.ohlim,pg.curpag);
			}
		}
		skip(pg.m2val);
	}
	/*
	*	initialize lineno for the next page
	*/
	pg.lineno = pg.m1val + pg.m2val + 1;
}


/*
 *	print character with test for printer
 */

prchar(c,fp)
char c;
FILE *fp;
{
/*	if (co.lpr == TRUE)
	{
		bdos(5,c);
	}
	else
*/	{
		putc(c,fp);
	}
}




/*
 *	put out line with proper spacing and indenting
 */

put(p)
char *p;
{
	char os[MAXLINE];
	int j;

	if (pg.lineno == 0 || pg.lineno > pg.bottom)
	{
		phead();
	}
	if(dc.prflg == TRUE)
	{
		if (!dc.bsflg)
		{
			if (strkovr(p,os) == TRUE)
			{
				for(j=0; j<pg.offset; ++j)
					prchar(' ',pout);
				for(j=0; j<dc.tival; ++j)
					prchar(' ',pout);
				putlin(os,pout);
			}
		}
		for(j=0; j<pg.offset; ++j)
			prchar(' ',pout);
		for(j=0; j<dc.tival; ++j)
			prchar(' ',pout);
		putlin(p,pout);
	}
	dc.tival = dc.inval;
	skip(min(dc.lsval-1,pg.bottom-pg.lineno));
	pg.lineno = pg.lineno + dc.lsval;
	if (pg.lineno > pg.bottom)
		pfoot();
}


/*
 *	output a null terminated string to the file
 *	specified by pbuf.
 */

putlin(p,pbuf)
char *p;
FILE *pbuf;
{
	while (*p != '\0')
		prchar(*p++,pbuf);
}



/*
 *	put out title or footer
 */

puttl(p,lim,pgno)
char *p;
int lim[];
int pgno;
{
	int i;
	char pn[8];
	char t[MAXLINE];
	char h[MAXLINE];
	char delim;

	itoda(pgno,pn,6);
	for(i=0; i<MAXLINE; ++i)
		h[i] = ' ';
	delim = *p++;
	p = getfield(p,t,delim);
	expand(t,dc.pgchr,pn);
	justleft(t,h,lim[LEFT]);
	p = getfield(p,t,delim);
	expand(t,dc.pgchr,pn);
	justcntr(t,h,lim);
	p = getfield(p,t,delim);
	expand(t,dc.pgchr,pn);
	justrite(t,h,lim[RIGHT]);
	for(i=MAXLINE-4; h[i] == ' '; --i)
		h[i] = '\0';
	h[++i] = '\n';
#ifdef MSDOS
	h[++i] = '\r';
#endif
	h[++i] = '\0';
	if(strlen(h) > 2)
	{
		for(i=0; i<pg.offset; ++i)
			prchar(' ',pout);
	}
	putlin(h,pout);
}



/*
 *	put word in output buffer
 */

putwrd(wrdbuf)
char *wrdbuf;
{
	int w;
	int last;
	int llval;
	char *p0, *p1;
	int nextra;

	w = width(wrdbuf);
	last = strlen(wrdbuf) + co.outp;
	llval = dc.rmval - dc.tival;
	if(((co.outp > 0) && ((co.outw + w) > llval)) || (last > MAXLINE))
	{
		last -= co.outp;
		if(dc.juval == YES)
		{
			nextra = llval - co.outw + 1;
			/*
			*	Check whether last word was end of
			*	sentence and modify counts so that
			*	it is right justified.
			*/
			if (co.outbuf[co.outp-2] == ' ')
			{
				--co.outp;
				++nextra;
			}
			spread(co.outbuf,co.outp-1,nextra,co.outwds);
			if((nextra > 0) && (co.outwds > 1))
			{
				co.outp += (nextra - 1);
			}
		}
		brek();
	}
	p0 = wrdbuf;
	p1 = co.outbuf + co.outp;
	while(*p0 != '\0')
		*p1++ = *p0++;
	co.outp = last;
	co.outbuf[co.outp++] = ' ';
	co.outw += w + 1;
	++co.outwds;
}


/*
 *	skips the number of lines specified by n.
 */

skip(n)
int n;
{
	int i;

	if (dc.prflg == TRUE && n > 0)
	{
		for(i=0; i<n; ++i)
		{
			prchar('\n',pout);
		}
#ifdef MSDOS
		prchar('\r',pout);
#endif
	}
}



/*
 *	spread words to justify right margin
 */

spread(p,outp,nextra,outwds)
char p[];
int outp,nextra,outwds;
{
	int i,j;
	int nb,ne,nholes;

	if((nextra <= 0) || (outwds <= 1))
		return;
	dc.sprdir = ~dc.sprdir;
	ne = nextra;
	nholes = outwds - 1;	/* holes between words */
	i = outp - 1;	/* last non-blank character */
#ifdef MSDOS
	j = min(MAXLINE-3,i+ne); /* leave room for CR, LF, '\0'  */
#else
	j = min(MAXLINE-2,i+ne); /* leave room for LF, '\0'  */
#endif
	while(i < j)
	{
		p[j] = p[i];
		if(p[i] == ' ')
		{
			if(dc.sprdir == 0) nb = (ne - 1)/nholes + 1;
			else nb = ne/nholes;
			ne -= nb;
			--nholes;
			for(; nb>0; --nb)
			{
				--j;
				p[j] = ' ';
			}
		}
		--i;
		--j;
	}
}



/*
 *	split overstrikes (backspaces) into seperate buffer
 */

strkovr(p,q)
char *p, *q;
{
	char *pp;
	int bsflg;

	bsflg = FALSE;
	pp = p;
	while (*p != '\0')
	{
		*q = ' ';
		*pp = *p;
		++p;
		if (*p == '\b')
		{
			if (*pp >= ' ' && *pp <= '~')
			{
				bsflg = TRUE;
				*q = *pp;
				++p;
				*pp = *p;
				++p;
			}
		}
		++q;
		++pp;
	}
#ifdef MSDOS
	*q++ = '\r';
#endif
	*q = *pp = '\0';
	return(bsflg);
}



/*
 *	underline a line
 */

underl(p0,p1,size)
char *p0,*p1;
int size;
{
	int i,j;

	j = 0;
	for (i=0; (p0[i] != '\n') && (j < size-1); ++i)
	{
		if (p0[i] >= ' ' && p0[i] <= '~')
		{
			if (isalpha(p0[i]) || isdigit(p0[i]) || dc.cuval > 0)
			{
				p1[j++] = '_';
				p1[j++] = '\b';
			}
		}
		p1[j++] = p0[i];
	}
	p1[j++] = '\n';
	p1[j] = '\0';
	while (*p1 != '\0') *p0++ = *p1++;
	*p0 = '\0';
}


/*
 *	compute width of character string
 */

width(s)
char *s;
{
	int w;

	w = 0;
	while (*s != '\0')
	{
		if(*s == '\b')
			--w;
		else if(*s != '\n' && *s != '\r')
			++w;
		++s;
	}
	return(w);
}
