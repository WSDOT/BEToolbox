/*//////////////////////////////////////////////////////////////////////
// BEToolbox - Bridge Engineering Toolbox
// Copyright (C) 2000, Washington State Department of Transportation
//                     Bridge and Structures Office
//
// This software was developed as part of the Alternate Route Project
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the Alternate Route Open Source License as
// published by the Washington State Department of Transportation,
// Bridge and Structures Office.
//
// This program is distributed in the hope that it will be useful,
// but is distributed AS IS, WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
// PURPOSE.  See the Alternate Route Open Source License for more details.
//
// You should have received a copy of the Alternate Open Source License
// along with this program; if not, write to the Washington State
// Department of Transportation, Bridge and Structures Office,
// 4500 3rd Ave SE, P.O. Box 47340, Olympia, WA 98503, USA or e-mail
// Bridge_Support@wsdot.wa.gov
//////////////////////////////////////////////////////////////////////*/

/***************************************************************************/
/*                                                                         */
/*  POLEDEP - Signal Pole Depth and Soil Pressure Calculations             */
/*                                                                         */
/*  Written by Paul Wocken        January 4, 1990                          */
/*                                                                         */
/***************************************************************************/

#define VAX /* use VAX or PC */

#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define CLRSCR printf("\033[2J")
#define DEF_SAFR 1.7 /* default safety factor */

extern int polyroot (double[], double[], int, double[], double[]);

extern void depthslp (double, double, double, double, double, double,
		      double, double, double *, double *, double *);

extern void readarms (double *, double *, double *, char, FILE *);

char wtdef[11] = {'D', 'E', 'F', 'A', 'U', 'L', 'T', 'S', ' ', ' ', ' '};
char bvals[11][4] = {{"B1 "}, {"B2 "}, {"B3 "}, {"B4 "}, {"B5 "}, {"B6 "},
		     {"B7 "}, {"B8 "}, {"B9 "}, {"B10"}, {"B11"}};

int main (int argc, char *argv[])
{
    FILE *in, *rpt;
    double a1, a2, c, slp, fa, safr;
    double arm1o[11], arm1a[11], arm1w[11];
    double arm2o[11], arm2a[11], arm2w[11];
    double armln[2], sumz[2], sumxy[2], sumxyz[2], sumwt[2];
    double totxy, pht, pwt, psh, ptq, wmla, wmsa, t1;
    double mx1, mx2, pbwmx, my1, my2, pbwmy, pmom;
    double pi, p, ww, wi, vi;
    double swtz[2], degrad;
    double s13r, s23r, d3r, s14r, s24r, d4r;
    int pole, arms, min, max, lrk;
    int i, j, k, page;
    char wts2[2], wts;
    char infil[52], outfil[52];
    time_t tod;

    if (argc < 2)               /* get input file */
    {
	infil[0] = '\0';
	while (infil[0] == '\0')
	{
	    CLRSCR;
	    printf ("\n\nEnter POLEDEP input file name:  ");
	    scanf ("%s",infil);
	}
    }
    else
	strcpy (infil, argv[1]);

/*    strlwr (infil); */

    if (strchr(infil,'.') == NULL)
	strcat (infil,".dat");

    if (argc < 3)              /* get output destination */
    {
	outfil[0] = '\0';
	while (outfil[0] == '\0')
	{
	    CLRSCR;
	    printf ("\n\nEnter POLEDEP output destination:  ");
	    scanf ("%s",outfil);
	}
    }
    else
	strcpy (outfil, argv[2]);

/*    strlwr (outfil); */

    if (strcmp(outfil,"screen") && strcmp(outfil,"lpt1") &&
	strcmp(outfil,"com1") && strcmp(outfil,"lpt2") &&
	strcmp(outfil,"com2") && strcmp(outfil,"com3") &&
	strcmp(outfil,"prn"))
    {
	if (strchr(outfil,'.') == NULL)
	    strcat (outfil,".out");
    }

    if ((in = fopen (infil, "r")) == NULL)        /* open input file */
    {
	CLRSCR;
	printf ("\n\n**** Error - Unable to open the Input file.");
	printf ("\n             Execution Terminating.");
	exit(0);
    }

    if (!strcmp(outfil,"screen"))       /* open output destination */
	rpt = stdout;
    else
	rpt = fopen(outfil, "w");

    if (rpt == NULL)
    {
	CLRSCR;
	printf ("\n\n**** Error - Unable to open the Output file.");
	printf ("\n             Execution Terminating.");
	exit (0);
    }

    if (strcmp(outfil,"screen"))       /* page eject if not screen output */
	fprintf (rpt,"\f");

    tod = time(NULL);                 /* get current date and time */

    p    = 28.0;                      /* set wind pressure */
    ww   = 10.0;                      /* set wind load on arms and pole */
    wi   = 13.0;                      /* set wind load on luminaire */
    vi   = 51.2;                      /* set wind force on luminaire */
    page = 0;                         /* initialize page counter */

    degrad = 57.295779513;           /* set degrees to radians constant */

    while (fscanf(in,"%d",&pole) != EOF)         /* process all poles */
    {
	fscanf (in,"%d%lf%lf%lf%lf%lf%lf%s",
                    &arms,&a1,&a2,&c,&slp,&fa,&safr,wts2);

	wts = tolower(wts2[0]);

	if (arms > 0)                          /* get arm 1 values */
	    readarms (arm1o, arm1a, arm1w, wts, in);
	else
	{
	    for (i=0; i < 11; i++)
		arm1o[i] = arm1a[i] = arm1w[i] = 0.0;
	}

	if (arms > 1)                          /* get arm 2 values */
	    readarms (arm2o, arm2a, arm2w, wts, in);
	else
	{
	    for (i=0; i < 11; i++)
		arm2o[i] = arm2a[i] = arm2w[i] = 0.0;
	}

	armln[0] = arm1o[0];                  /* set arm length */
	armln[1] = arm2o[0];
					      /* calculate total offsets */
	for (sumz[0]=0.0,i=0; i<11; sumz[0] += arm1o[i], i++);
	for (sumz[1]=0.0,i=0; i<11; sumz[1] += arm2o[i], i++);
					     /* calculate total areas */
	for (sumxy[0]=0.0,i=0; i<11; sumxy[0] += arm1a[i], i++);
	for (sumxy[1]=0.0,i=0; i<11; sumxy[1] += arm2a[i], i++);
					  /* calculate total offset * area*/
	for (sumxyz[0]=0.0,i=0; i<11; sumxyz[0] += arm1o[i] * arm1a[i], i++);
	for (sumxyz[1]=0.0,i=0; i<11; sumxyz[1] += arm2o[i] * arm2a[i], i++);
					  /* calculate total weight * offset */
	for (swtz[0]=0.0,i=0; i<11; swtz[0] += arm1o[i] * arm1w[i], i++);
	for (swtz[1]=0.0,i=0; i<11; swtz[1] += arm2o[i] * arm2w[i], i++);

	totxy = sumxy[0] + sumxy[1];    /* calculate total areas for pole */

	if (c > 0.0)              /* luminaire on pole? */
	{
	    lrk = 1;
	    pht = a2 - c * tan(30.0 / degrad);   /* pole hieght with lum. */
	    pwt = 10.0 * a2;                  /* pole weight with lum. */
	}
	else
	{
	    lrk = 0;
	    pht = a1 + 1.5;             /* pole height without lum. */
	    pwt = 10.0 * a1;            /* pole weight without lum. */
	}

	if (arm1o[0] >= arm2o[0])     /* determine maximum and minimum arm */
	{
	    min = 1;
	    max = 0;
	}
	else
	{
	    min = 0;
	    max = 1;
	}
					/* calculate pole shear */
	psh  = ww * (armln[max] + pht) + 1.2 * p * totxy;
	psh += lrk * (51.2 + wi * c / cos(30.0 / degrad));
				       /* calculate pole torsion */
	ptq = ww * armln[max] * armln[max] / 2.0 + 1.2 * p * sumxyz[max];
	ptq = (ptq + lrk * c * (51.2 + wi * c / 2.0)) * sin(75 / degrad);
				    /* calculate wind moment of long arm */
	t1    = armln[max] / 25.0 + a1;
	wmla  = ww * (armln[max] * t1 + pht * pht / 2.0);
	wmla += 1.2 * p * sumxy[max] * t1;
			       /* calculate wind moment of short arm */
	t1   = armln[min] / 25.0 + a1;
	wmsa = ww * armln[min] * t1 + 1.2 * p * sumxy[min] * t1;
		 /* calculate x component of pole base wind moment */
	mx1   = armln[max] * armln[max] * (3.75 + armln[max] / 8.0);
	mx1  += swtz[max];
	mx2   = lrk * c * (3.0 * c + 60.0);
	pbwmx = mx1 + wmla * cos(75.0 / degrad) + wmsa * sin(75.0 / degrad);
	pbwmx+= mx2;
		/* calculate y component of pole base wind moment */
	my1   = armln[min] * armln[min] * (3.75 + armln[min] / 8.0);
	my1  += swtz[min];
	my2   = 51.2 * a2 + wi * c * (a2 - c / 2.0 * tan(30.0 / degrad));
	my2  *= lrk;
	pbwmy = my1 + wmla + wmsa * cos(75.0 / degrad) + my2;
		     /* calculate wind moment of pole */
	pmom = sqrt(pbwmx * pbwmx + pbwmy * pbwmy);
		     /* calculate depth and soil pressures for 3ft rnd */
	depthslp (fa, slp, pmom, psh, ptq, pwt, 3.0, safr, &d3r, &s13r, &s23r);
		     /* calculate depth and soil pressures for 4ft rnd */
	depthslp (fa, slp, pmom, psh, ptq, pwt, 4.0, safr, &d4r, &s14r, &s24r);

	page++;            /* increment page counter */

	if (strcmp(outfil,"screen"))    /* print input if not screen */
	{
            fprintf (rpt,"\n%s",ctime(&tod));
	    fprintf (rpt,"                 Washington State Department of ");
	    fprintf (rpt,"Transportation         Page:  %2d",page);

	    fprintf (rpt,"\n                            ***  POLEDEP  ***");

	    fprintf (rpt,"\n\n\n*****  INPUT DATA  *****");
	    fprintf (rpt,"\n                                            ");
	    fprintf (rpt,"*** Design Parameters ***");

	    fprintf (rpt,"\nPole:  %3d    Arm  Height (A1):  %6.2f",pole,a1);
	    fprintf (rpt,"     Soil Pressure (SLP):  %4.0f",slp);

	    fprintf (rpt,"\nArms:  %3d    Lum. Height (A2):  %6.2f",arms,a2);
	    fprintf (rpt,"     Friction Angle     :  %2.0f",fa);

	    fprintf (rpt,"\n              Lum. Arm    (C) :  %6.2f",c);
            fprintf (rpt,"     Safety Factor      :  %5.3f",
                     ((safr < 0)?DEF_SAFR:safr));

	    if (arms > 0)
	    {
		fprintf (rpt,"\n\n                  OFFSETS(ft) ");
		fprintf (rpt,"AREAS(ft^2) WEIGHTS(lbs)");

		for (i=0; i<11; i++)
		{
		    fprintf (rpt,"\n              %s",bvals[i]);
		    fprintf (rpt,"    %5.2f",arm1o[i]);
		    fprintf (rpt,"      %5.1f        ",arm1a[i]);

		    if (wts == 'y')
			fprintf (rpt,"%5.1f",arm1w[i]);
		    else
			fprintf (rpt,"  %c",wtdef[i]);
		}
	    }

	    if (arms > 1)
	    {
		fprintf (rpt,"\n\n                    OFFSETS     ");
		fprintf (rpt,"AREAS     WEIGHTS");

		for (i=0; i<11; i++)
		{
		    fprintf (rpt,"\n              %s",bvals[i]);
		    fprintf (rpt,"    %5.2f",arm2o[i]);
		    fprintf (rpt,"      %5.1f      ",arm2a[i]);

		    if (wts == 'y')
			fprintf (rpt,"%5.1f",arm2w[i]);
		    else
			fprintf (rpt,"  %c",wtdef[i]);
		}
	    }
	}
	else
	{
	    CLRSCR;
	    fprintf (rpt,"Pole:  %3d",pole);
	}

	fprintf (rpt,"\n\n\n*****  FINAL RESULTS  *****");

        fprintf (rpt,"\n\n              Pole Moment:  ");
        fprintf (rpt,"%5.0f ft-lbs",pmom);
        fprintf (rpt,"\n              Pole Shear :  ");
        fprintf (rpt,"%5.0f lbs",psh);
        fprintf (rpt,"\n              Pole Torque:  ");
        fprintf (rpt,"%5.0f ft-lbs",ptq);
        fprintf (rpt,"\n              Pole Weight:  ");
        fprintf (rpt,"%5.0f lbs",pwt);

	fprintf (rpt,"\n\n              Foundation Depths:  ");
	fprintf (rpt,"3' RND = %2.0f ft",d3r);
	fprintf (rpt,"\n                                  ");
	fprintf (rpt,"4' RND = %2.0f ft",d4r);
	fprintf (rpt,"\n                                  ");
	fprintf (rpt,"3' SQ  = %2.0f ft",d4r);

	fprintf (rpt,"\n\n              Actual Soil Pressures:  ");
	fprintf (rpt,"3' RND S1 = %7.2f psf",s13r);
	fprintf (rpt,"\n                                      ");
	fprintf (rpt,"3' RND S2 = %7.2f psf",s23r);
	fprintf (rpt,"\n                                      ");
	fprintf (rpt,"4' RND S1 = %7.2f psf",s14r);
	fprintf (rpt,"\n                                      ");
	fprintf (rpt,"4' RND S2 = %7.2f psf",s24r);
	fprintf (rpt,"\n                                      ");
	fprintf (rpt,"3' SQ  S1 = %7.2f psf",s14r);
	fprintf (rpt,"\n                                      ");
	fprintf (rpt,"3' SQ  S2 = %7.2f psf",s24r);

	fprintf (rpt,"\n\n             Allowable soil pressures may be ");
	fprintf (rpt,"140%% of design soil pressure (SLP)\n");

	if (strcmp(outfil,"screen"))
	    fprintf (rpt,"\f");

	if (!strcmp(outfil,"screen"))
	{
	    fprintf (rpt,"\n\nHit any key to continue");
	    /*getch();*/
            getc(stdin);
	}
    }

    fclose (rpt);
    fclose (in);

    return 1;
}

int polyroot (xcof, cof, m, rootr, rooti)

/*
    POLYROOT - Calculate the roots of a polynomial

    xcof  - array of coefficients with m+1 elements
    cof   - work array same size as xcof
    m     - order of the polynomial
    rootr - real part of the roots of the polynomial with m elements
    rooti - imaginary part of the roots of the polynomial with m elements

    all passed values are DOUBLE except m which is INT.  The largest order
    of polynomial that can be processed is 36th order.

    The subroutine returns an integer value called IER for error code.
    the following codes may occur:

       0 = no errors occurred
       1 = m is less than 1
       2 = m is greater than 36
       3 = Unable to determine root with 500 iterations on 5
	   starting values
       4 = High order coefficient is 0

    This subroutine is taken from the manual SCIENTIFIC SUBROUTINES.  It
    has been converted from the original FORTRAN into C.  The original
    name of the routine is POLRT and is on page 182 of the manual.
*/

    double xcof[], cof[], rootr[], rooti[];
    int m;

{
    double xo, yo, x, y, xpr, ypr, ux, uy, v, yt, xt, u;
    double xt2, yt2, sumsq, dx, dy, temp, alpha, fi;
    int ifit, ier, n, nx, nxx, n2, kj1, l, i, mt, in, ict, itemp;

    ifit = 1;
    n = m;
    ier = 0;

    if (n <= 0) ier = 1;
    if (n > 36) ier = 2;
    if (xcof[n] == 0.0) ier = 4;
    if (ier != 0) return ier;

    nx = n;
    nxx = kj1 = n + 1;
    n2 = 0;

    for (l=1; l <= kj1; cof[kj1-l]=xcof[l-1],l++);

  sl45:
    xo = 0.00500101;
    yo = 0.01000101;
    in = 0;

  sl50:
    x = xo;

    xo = -10.0 * yo;
    yo = -10.0 * x;

    x = xo;
    y = yo;
    ++in;
    goto sl59;

  sl55:
    ifit = 1;
    xpr = x;
    ypr = y;

  sl59:
    ict = 0;

  sl60:
    ux = uy = v = yt = 0.0;
    xt = 1.0;
    u = cof[n];

    if (u == 0.0) goto sl130;

    for (i=1; i <= n; i++)
    {
	temp = cof[n-i];
	xt2 = x * xt - y * yt;
	yt2 = x * yt + y * xt;
	u += temp * xt2;
	v += temp * yt2;
	fi = (double)i;
	ux += fi * xt * temp;
	uy -= fi * yt * temp;
	xt = xt2;
	yt = yt2;
    }

    sumsq = ux * ux + uy * uy;
    if (sumsq == 0.0) goto sl110;

    dx = (v * uy - u * ux) / sumsq;
    x += dx;
    dy = -(u * uy + v * ux) / sumsq;
    y += dy;

    if ((fabs(dy) + fabs(dx) - 1.0e-5) < 0.0) goto sl100;

    ++ict;

    if ((ict - 500) < 0) goto sl60;
    if (ifit != 0) goto sl100;
    if ((in - 5) < 0) goto sl50;

    return 3;

  sl100:
    for (l=1; l <= nxx; l++)
    {
	temp = xcof[kj1-l];
	xcof[kj1-l] = cof[l-1];
	cof[l-1] = temp;
    }

    itemp = n;
    n = nx;
    nx = itemp;

    if (ifit == 0) goto sl55;
    else goto sl120;
  sl110:
    if (ifit == 0) goto sl50;

    x = xpr;
    y = ypr;

  sl120:
    ifit = 0;

    if ((fabs(y) - 1.0e-4 * fabs(x)) < 0.0) goto sl135;

    alpha = x + x;
    sumsq = x * x + y * y;
    n -= 2;
    goto sl140;

  sl130:
    x = 0.0;
    --nx;
    --nxx;

  sl135:
    y = sumsq = 0.0;
    alpha = x;
    --n;

  sl140:
    cof[1] = cof[1] + alpha * cof[0];

    for (l=2; l <= n; l++)
	cof[l] = cof[l] + alpha * cof[l-1] - sumsq * cof[l-2];

  sl155:
    rooti[n2] = y;
    rootr[n2] = x;
    ++n2;

    if (sumsq == 0.0) goto sl165;

    y = -y;
    sumsq = 0.0;
    goto sl155;

  sl165:
    if (n > 0) goto sl45;

    return 0;
}

void depthslp (fa, slp, pmom, psh, ptq, pwt, fdia, safr, d, s1, s2)

/*
    DEPTHSLP - Calculate Depth of Pole and Soil Pressure Values S1 and S2

    fa   - friction angle of soil
    slp  - design soil pressure
    pmom - wind moment of pole
    psh  - pole shear
    ptq  - pole torsion
    pwt  - weight of the pole
    fdia - diameter of pole
    safr - safety factor
    d    - calculated depth
    s1   - calculated soil pressure value s1
    s2   - calculated soil pressure value s2
*/

    double fa, slp, pmom, psh, ptq, pwt, fdia, safr;
    double *d, *s1, *s2;

{
    double capk, fard, htp, sl2p, depth, depth2;
    double pi, epco, slwt, tsafr, degrad, q1, q2;
    double qarr[4], tarr[4], rootr[3], rooti[3];
    int k, i;

    for (i=0; i<3; i++)                     /* initialize root arrays */
	rootr[i] = rooti[i] = 0.0;

    tsafr = (safr < 0) ? DEF_SAFR : safr;
                                 /* set safety factor to default if < 0 */
    slwt = 120.0;                /* set soil weight */
    epco = 0.7;                  /* set earth pressure coefficient */
    pi   = 3.141592654;          /* set value of pi */
    degrad = 57.295779513;       /* set degrees per radian constant */

    fard = fa / degrad;          /* convert friction angle to radians */
    sl2p = slp * 1.4;            /* calculate maximum soil pressure */
    htp  = pmom / psh;           /* calculate height of pressure */

    capk = tan(fard) * (0.7 * fdia) / tsafr;

    qarr[0] = -htp * psh;                /* build coefficients of 3rd */
    qarr[1] = -0.34 * psh;               /* order polynomial */
    qarr[2] = 0.1792 * fdia * sl2p;

    k = polyroot (qarr, tarr, 2, rootr, rooti);   /* calculate roots */

    if (k > 0)                     /* errors in root calculation? */
    {
	CLRSCR;
	printf ("\n\n**** Error - Processing of second order polynomial");
	printf ("\n             failed with error code %d returned.");
	printf ("\n             Execution Terminating.");
	exit(0);
    }

    for (i=0; i<2; i++)              /* eliminate imaginary roots */
    {
	if (rooti[i] != 0.0)
	    rootr[i] = 0.0;
    }
				       /* set depth to largest root */
    depth = (rootr[0]>=rootr[1])?rootr[0]:rootr[1];
			    /* build coefficients of 4th order polynomial */
    qarr[0] = 2.0 * psh * htp;
    qarr[1] = 1.24 * psh + 0.28 * pwt - 0.56 * ptq / capk;
    qarr[2] = 11.2 * pi * fdia * fdia;
    qarr[3] = 0.56 * epco * slwt * fdia;

    k = polyroot (qarr, tarr, 3, rootr, rooti);  /* calculate roots */

    if (k > 0)                   /* errors in root calculation */
    {
	CLRSCR;
	printf ("\n\n**** Error - Processing of third order polynomial");
	printf ("\n             failed with error code %d returned.");
	printf ("\n             Execution Terminating.");
	exit(0);
    }

    for (i=0; i<3; i++)              /* eliminate imaginary roots */
    {
	if (rooti[i] != 0.0)
	    rootr[i] = 0.0;
    }
				      /* set depth2 to largest root */
    depth2 = (rootr[0]>=rootr[1])?rootr[0]:rootr[1];
    depth2 = (rootr[2]>=depth2)?rootr[2]:depth2;

    depth  = (depth2 > depth)?depth2:depth;  /* set depth to largest depth */

    if (depth < 5.0) depth = 5.0;     /* minimum depth of 5 feet */

    depth += 0.95;                    /* allow for error */
    depth  = (double)((long)depth);   /* drop fractions */

    *d = depth;                       /* set return depth */

    q1  = psh * (htp + 0.9 * *d) / (0.56 * *d);
    *s1 = q1 / (0.68 * fdia * *d);      /* calculate s1 soil pressure */

    q2  = psh * (htp + 0.34 * *d) / (0.56 * *d);
    *s2 = q2 / (0.32 * fdia * *d);     /* calculate s2 soil pressure */

    return;
}

void readarms (armo, arma, armw, wts, in)

/*
   READARMS - read the arm data for a pole
*/

    FILE *in;
    double *armo, *arma, *armw;
    char wts;

{
    int i;

    for (i=0; i < 11; i++)
    {
	if (wts == 'y')
	    fscanf (in, "%lf%lf%lf", armo+i, arma+i, armw+i);
	else
	{
	    fscanf (in, "%lf%lf", armo+i, arma+i);
	    *(armw+i) = (i==5 || i==8)? 20.0: 60.0;
	}
    }

    return;
}
