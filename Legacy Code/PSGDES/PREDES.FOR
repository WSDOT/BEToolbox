C///////////////////////////////////////////////////////////////////////
C// BEToolbox - Bridge Engineering Toolbox
C// Copyright (C) 2000, Washington State Department of Transportation
C//                     Bridge and Structures Office
C//
C// This software was developed as part of the Alternate Route Project
C//
C// This program is free software; you can redistribute it and/or modify
C// it under the terms of the Alternate Route Open Source License as
C// published by the Washington State Department of Transportation,
C// Bridge and Structures Office.
C//
C// This program is distributed in the hope that it will be useful,
C// but is distributed AS IS, WITHOUT ANY WARRANTY; without even the
C// implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
C// PURPOSE.  See the Alternate Route Open Source License for more details.
C//
C// You should have received a copy of the Alternate Open Source License
C// along with this program; if not, write to the Washington State
C// Department of Transportation, Bridge and Structures Office,
C// 4500 3rd Ave SE, P.O. Box 47340, Olympia, WA 98503, USA or e-mail
C// Bridge_Support@wsdot.wa.gov
C///////////////////////////////////////////////////////////////////////


      SUBROUTINE PREDES
C     ***  PRESTRESSED GIRDER DESIGN  ***  'PREDES'  ***
C
C
C    A      - "A" DIMENSION FROM STANDARD PLAN OR ULTIMATE CAMBER OF
C             NONCOMPOSITE GIRDERS IN INCHES
C    ADIM   - DISTANCE FROM TOP OF GIR TO BOTTOM OF ULT. STRESS BLOCK
C    ADIM1  - DISTANCE FROM TOP OF SLAB TO BOTTOM OF ULT. STRESS BLOCK
C    ADSLAB - ADDITIONAL SLAB AREA FOR STANDARD NONCOMPOSITE GIRDERS
C             OVER 49 INCH WIDE                                         .C.
C    ADY2G  - A DY ** 2 FOR 49 IN. STANDARD NONCOMPOSITE GIRDERS        .C.
C    ADY2S  - A DY ** 2 FOR ADDITIONAL SLAB ON STANDARD NONCOMPOSITE    .C.
C             GIRDERS OVER 49 IN. WIDE                                  .C.
C    AEB    - CROSS-SECTIONAL AREA OF END BLOCK (SQ. INCHES)
C    AG     - CROSS-SECTIONAL AREA OF GIRDER (SQ. INCHES)
C    AS     - AREA OF ONE STRAND
C    ASF    - SEE SPEC FOR ULTIMATE MOMENTS - AASHTO ARTICLE 9.17
C    ASP    - ASPHALT OVERLAY THICKNESS (INCHES)
C    ASPH   - WEIGHT OF ASPHALT OVERLAY PER GIRDER PER FOOT
C    ASR    - SEE SPEC FOR ULTIMATE MOMENTS - AASHTO ARTICLE 9.17
C    AVFY   - AVFY OF STIRRUPS USED  CHANGED DEF TO 37.2K                  .D.
C    A2     - MAXIMUM NUMBER OF GIRDERS PER CURB
C    A3     - WEB THICKNESS
C    B      - WORKING FLANGE WIDTH
C    BEFF   - EFFECTIVE FLANGE WIDTH OVERIDE (FOR COMPOSITE GIRDERS)
C    B1     - TEMP FLANGE WIDTH
C    B2     - TEMP FLANGE WIDTH
C    B3     - TEMP FLANGE WIDTH
C    BCGH   - SEE CGH
C    BLOCK  - HEADING CHARACTER VARIABLE
C    BTF    - WIDTH OF TOP FLANGE (INCHES)
C    BULB   - HEADING CHARACTER VARIABLE
C    CGH    - HEIGHT OF C.G. HARPED STRANDS AT CENTERLINE SPAN
C    CGHO   - CSG OVERIDE
C    CRAISE - AMOUNT HARPED STRANDS HAVE BEEN RAISED AT MID SPAN
C    CGS    - C.G. OF STRAIGHT STRANDS FROM BOTTOM
C    CGSO   - CGS OVERIDE
C    CGTEB  - HEIGHT OF C.G. OF TOTAL STRANDS AT D/2 FROM BOTTOM
C    CGTTS  - HEIGHT OF C.G. OF TOTAL STRANDS FROM TOP OF GIRDER (=D)
C    CGTT   - SAME AS CGTTS OR 0.8*HT WHICH EVER IS GREATER.
C    CHECK  - 0 = DESIGN PROBLEM, 1 = CHECK PROBLEM
C    CHK    - HEADING CHARACTER VARIABLE
C    CHKCG  - IF CHKCG = 1 HARPED STRANDS MAY NOT BE RAISED
C    CLOAD  - CHARACTER VARIABLE WITH SHEAR CONTROLLING LIVE LOAD
C    CLOC   - LOCATION OF SHEAR BEING CONSIDERED
C    CMUC   - ULT. MOM. CAP. OF COMPOSITE GIRDER
C    CMUF   - ULT. MOM. CAP. OF TOP FLANGE INCL. WEB WIDTH
C    CMUG   - ULT. MOM. CAP. OF GIRDER ALONE
C    CMUR   - ULT. MOM. CAP. OF RECT. BEAM OF FLANGE WIDTH
C    COMPA  - ALLOWED STRESS - TOP CENTERLINE
C    COMPI  - ALLOWED STRESS BOT AT HARP POINT AT RELEASE
C    CREEP  - CREEP
C    CSI    - COMPOSITE I OF GIRDER SECTION
C    CSTR   - CONSTANT FOR ALLOWABLE TENS. IN STRANDS (CSTR*USS-FLOS)
C    CURB   - NEW JERSEY CHARACTER VARIABLE
C    D      - D OF CONCRETE (COMPOSITE) SECTION
C    DCGH   - RISE IN HARPED STRANDS
C    DCGHI  - INITIAL RISE IN HARPED STRANDS
C    DCGHMX- XHARP/7.                                                   .B.
C    DCGOVR- DCGH-DCGHMX
C    DCGHO  - DCGH OVERIDE
C    DDCG   - DISTANCE ADJUSTMENT FROM TOP OF GIRDER TO TOP OF HARPED
C              STRANDS AT END ACCORDING TO SPAN LENGTH
C    DDP    - HEIGHT OF INTERMEDIATE DIAPHRAGM
C    DIAPH  - WEIGHT OF DIAPHRAGM PER FOOT OF GIRDER
C    DIF    - TEMPORARY VARIABLE
C    DLG    - GIRDER DEAD LOAD IN KIPS/FT
C    DLL    - LIVE LOAD DISTRIBUTION
C    DMOM   - MOMENT DUE TO INTERMEDIATE DIAPHRAGMS
C    DTMP1  - D USED FOR SHEAR AT END OF TAPER
C    DTMP2  - D USED FOR SHEAR AT HARPING PT.
C    DN     - NUMBER OF DIAPHRAGMS PER GIRDER
C    DY     - CHANGE IN N.A. FOR COMPOSITE SECTION
C    E      - STRAND ECCENTRICITY
C    EBI    - MOMENT OF INERTIA OF END BLOCK
C    EBT    - THICKNESS OF END BLOCK
C    EBTL   - END BLOCK TAPER LENGTH
C    EBW    - END BLOCK WEIGHT (IN EXCESS OF GIRDER WEIGHT)
C    ECSI   - COMPOSITE I OF END BLOCK SECTION
C    EDY    - DELTA Y OF COMPOSITE END BLOCK SECTION
C    EGIRD  - ELASTIC MODULUS OF GIRDER
C    ESLAB  - ELASTIC MODULUS OF SLAB
C    ESTL   - ELASTIC MODULUS OF STEEL
C    FCU    - ULTIMATE CONCRETE STRENGTH (GIRDER)
C    FCUDF  - ULTIMATE CONCRETE STRENGTH (GIRDER) PASSED TO DFLECT SUB  .C.
C    FD     - SHEAR VARIABLE (SEE AASHTO 9.20)
C    FHOLD  - FORCE REQUIRED TO HOLD DOWN HARPED STRANDS                .A.
C    FHMX   - MAX HOLD DOWN FORCE ALLOWED ( 45 KIPS AT CENTRAL PREMIX ) .A.
C    FICU   - F'CG AT RELEASE OF PRESTRESS
C    FILFT  - IMPACT FACTOR - LIFT
C    FISHP  - IMPACT FACTOR - SHIP
C    FLLFT  - LOSSES - LIFT
C    FLOS   - FINAL ESTIMATED LOSSES
C    FLSHP  - LOSSES - SHIP
C    FO     - DIST TOP OF GIRDER TO C.G. HARPED STRANDS AT END
C    FPC    - SHEAR VARIABLE (SEE AASHTO 9.20)
C    FPE    - SHEAR VARIABLE (SEE AASHTO 9.20)
C    FSS    - FINAL STRAND STRESS
C    FSUC   - ULTIMATE STEEL STRESS FOR ULT. MOMENT IN COMPOSITE GIR.
C    FSUG   - ULTIMATE STEEL STRESS FOR ULT. MOMENT IN GIRDER ONLY.
C    FT     - FINAL ALLOWED TENS. IN CONC. PRECOM. TENS. ZONE (BOT)
C    FTI    - INITIAL ALLOWED TENS. IN CONC. PRECOM. TENS. ZONE (BOT)
C    FTITOP - INITIAL ALLOWED TENSION IN CONCRETE ON THE GIRDER TOP
C    FTTOP  - FINAL ALLOWED TENSION IN CONCRETE ON THE GIRDER TOP
C             WHERE NO BONDED REINF. IS ASSUMED
C    FXL    - ADJUSTMENT FACTOR FOR POS. ALONG GIRD. (.5 AT MIDSPAN ETC)
C    GI     - MOMENT OF INERTIA OF GIRDER
C    GIR    - NUMBER OF GIRDERS
C    GR1    - TOTAL REACTION ONE GIRDER
C    GR2    - TOTAL REACTION ONE GIRDER WITHOUT CURB
C    GTS    - GROSS SLAB THICKNESS
C    H      - DISTANCE FROM TOP OF GIRDER TO C.G. HARPED STRANDS
C    HARP   - MOMENT COEFFICIENT @ HARPING POINT
C    HARPL  - LENGTH OF HARPED STRANDS FROM HARPING POINT TO END BLOCK
C    HF     - HEIGHT OF FORCES AT RELEASE AT D/2
C    HHARP  - HEIGHT OF HARPED STRANDS AT D/2
C    HHRTP  - HEIGHT OF HARPED STRANDS AT END OF TAPER
C    HT     - HEIGHT OF GIRDER
C    IBULBT - SWITCH FOR NONCOMPOSITE DESIGN 1 FOR NON, 0 FOR COMPOSITE
C    ICOUNT - A COUNTER TO PREVENT A LOOP
C    J      - LINE CODE
C    KCG    - IF = 1, HARPED STRANDS HAVE BEEN RAISED
C    KEB    - 0 IF NO END BLOCKS, 1 IF WITH END BLOCKS
C    KKONT  - 1 IF EXTRA STRANDS HAVE BEEN HARPED FOR NONCOMPOSITE
C             GIRDERS OR CONTINUOUS COMPOSITE
C             GIRDERS, 2 IF AFTER RAISING STRANDS MORE
C             STRAND PATTERN MANIPULATION WOULD HAVE BEEN REQUIRED
C             0 IF NO EXTRA STRANDS WERE HARPED
C    KSER   - GIRDER DESIGNATION                                        .C.
C    KSTY   - 1 IF STRESS RELIEVED STRANDS, 0 IF LOW RELAXATION.
C    KTD2A  - ERROR SIGNAL 0 = OK, 1 IF NEED TO HARP MORE STRANDS BUT
C             CANNOT
C    KTD2B  - ERROR SIGNAL 0 = OK, 1 IF NEED TO HARP LESS STRANDS BUT
C             CANNOT
C    KTEST  - DIRECTS ORDER OF READ COMMANDS
C    KUM    - IF = 1, THE CONCRETE IN THE GIRDER CANNOT WITHSTAND THE
C               ULTIMATE MOMENT REGARDLESS OF THE AMOUNT OF STEEL.
C    KUMC   - IF = 1, THE CONCRETE IN THE GIRDER AND SLAB CANNOT
C               WITHSTAND THE ULTIMATE MOMENT REGARDLESS OF THE AMOUNT
C               OF STEEL ADDED.
C    K2     - 1 IF TRACING STRAND MANIPULATION , 0 IF INACTIVE
C    L      - KEY FOR CONTINUOUS GIRDERS  0 - SIMPLE  1 - CONTINUOUS
C    LLDXI  - LIVE LOAD DISTRIBUTION AND INPACT FACTOR
C    LOSS   - CHARACTER TITLE VARIABLE
C    MAXNH  - MAXIMUM NUMBER OF HARPED STRANDS ALLOWED
C    MAXNS  - MAXIMUM NUMBER OF STRAIGHT STRANDS ALLOWED
C    MCR    - CRACKING MOMENT FOR SHEAR
C    MCRC   - 1.2*CRACKING MOMENT FOR ULT CAP OF COMPOSITE GIR.
C    MCRG   - 1.2*CRACKING MOMENT FOR ULT CAP OF NONCOMP. GIR.
C    MLL    - TEMP VARIABLE IN LIVLOD SUB
C    MMAX   - MAXIMUM MOMENT FOR SHEAR
C    NCE    - NUMBER OF CONTINUOUS ENDS
C    NDAYS  - TIME OF SLAB POUR (DAYS)
C    NH     - NUMBER OF HARPED STRANDS
C    NOVER  - INTEGER OF DCGOVR
C    NS     - NUMBER OF STRAIGHT STRANDS
C    NTS    - TOTAL NUMBER OF STRANDS
C    NTS2   - TEMPORARY VARIABLE
C    P      - TOTAL STRAND FORCE
C    PAR    - P AT RELEASE
C    PART   - P(HARP) AT RELEASE IN END BLOCK
C    PEACH  - FINAL LOAD IN EACH STRAND
C    PMAX   - STEEL RATIO, SEE AASHTO SECTION 9.17
C    PPT    - PICK POINT
C    QSLAB  - AREA MOMENT OF SLAB
C    RATIO  - ESLAB/EGIRD
C    RELAX  - HEADING CHARACTER VARIABLE
C    S      - GIRDER SPACING (FEET)
C    SBAH   - STRESS BOTTOM AT HARP POINT AT RELEASE
C    SBDC   - STRESS BOT C/L DUE TO CURB D.L. ON GIRDER
C    SBDCH  - SAME AS SBDC BUT AT HARPING POINT
C    SBDG   - STRESS BOTTOM DUE TO GIRDER D.L. ON GIRDER AT C/L
C    SBDGET - STRESS BOTTOM DUE TO ALL D.L. AT END OF TAPER
C    STDGET - STRESS TOP DUE TO ALL D.L. AT END OF TAPER
C    SBDGH  - SAME AS SBDG BUT AT HARPING POINT
C    SBDS   - STRESS BOTTOM DUE TO SLAB D.L. ON GIRDER
C    SBDSH  - SAME AS SBDS BUT AT HARPING POINT
C    SBEBI  - STRESS BOTTOM AT D/2 AT RELEASE
C    SBEB   - FINAL DESIGN STRESS BOTTOM AT D/2 = SBEBC + SBEBG + SBEBS
C    SBEBC  - STRESS BOTTOM AT D/2 DUE TO CURB D.L.
C    SBEBG  - STRESS BOTTOM AT D/2 DUE TO GIRDER D.L.
C    SBEBI  - STRESS BOTTOM AT END BLOCK (D/2) AT RELEASE
C    SBEBS  - STRESS BOTTOM AT D/2 DUE TO SLAB D.L.
C    SBF    - FINAL DESIGN STRESS, BOTTOM CENTERLINE
C    SBPRE  - STRESS BOTTOM CENTERLINE DUE TO PRESTRESS (FINAL)
C    SBTOT  - TOTAL STRESS BOTTOM CENTERLINE (LOADS)
C    SBXML  - STRESS BOTTOM AT CENTERLINE DUE TO LL+I
C    SDTCH  - SAME AS SBDCH BUT TOP STRESS
C    SIMPLE - HEADING CHARACTER VARIABLE
C    SLABA  - AREA OF SLAB ONLY
C    SLABF  - SLAB F(C)
C    SLABFDF- SLAB F(C) PASSED TO DFLECT SUB.                           .C.
C    SLABI  - MOMENT OF INERTIA OF EXTRA SLAB ON GIRDER TYPE 35 AND 53  .C.
C    SPECU  - ULTIMATE DL MOMENT
C    SPECUL - ULTIMATE DL AND LL MOMENT
C    SSDG   - STRESS OF STRANDS DUE TO D.L. OF GIRDER
C    SSPRE  - STRESS OF STRANDS RIGHT AFTER RELEASING OF PRESTRESSING
C    SSTIR  - STIRRUP SPACING
C    SSTRD  - CONCRETE STRESS AT STRANDS IN END BLOCK
C    STDGH  - STRESS TOP D.L. GIRDER AT HARPING POINT
C    STEB   - FINAL DESIGN STRESS TOP AT D/2 = STEBC + STEBG + STEBS
C    STEBC  - STRESS TOP AT D/2 DUE TO CURB D.L.
C    STEBG  - STRESS TOP AT D/2 DUE TO GIRDER D.L.
C    STEBI  - STRESS TOP AT END BLOCK (D/2) AT RELEASE
C    STEBS  - STRESS TOP AT D/2 DUE TO SLAB D.L.
C    STH    - STRESS TOP AT HARPING POINT AT RELEASE
C    STPRE  - STRESS TOP CENTERLINE DUE TO PRESTRESS (FINAL)
C    STTOT  - FINAL DESIGN STRESS, TOP CENTERLINE
C    STXML  - TOP STRESS AT CENTERLINE DUR TO LL+I
C    S1     - SHEAR SPACING FOR STD SHEETS.
C    TFLNG  - AVERAGE DEPTH OF GIRDER TOP FLANGE
C    TH     - NET SLAB THICKNESS
C    TWC    - TOTAL WEIGHT OF CURB PER FOOT
C    ULLDXI - LIVE LOAD DISTRIBUTION, IMPACT AND LL MOMENT FACTORS
C    USS    - ULTIMATE STRENGTH OF STRAND (KSI)
C    UXHARP - DISTANCE FROM CL BRG TO HARPING POINT
C    VC     - NOMINAL CONCRETE SHEAR STRENGTH
C    VCI    - NOMINAL CONCRETE FLEXURAL SHEAR STRENGTH
C    VCIMIN - MIN. ALLOWABLE SHEAR TAKEN BY CONCRETE
C    VCW    - NOMINAL CONCRETE WEB SHEAR STRENGTH
C    VD     - UNFACTORED DEAD LOAD SHEAR
C    VI     - FACTOR SHEAR ON SECTION EXCLUDING GIRDER
C    VLM    - MAXIMUM ULTIMATE LIVE LOAD SHEAR
C    VS     - NOMINAL SHEAR STRENGTH PROVIDED BY STIRRUPS
C    VSSTD  - VS PROVIDED BY STANDARD GIRDER END DETAILS (#5'S AT 6")
C    VST    - NOMINAL SHEAR STRENGTH PROVIDED BY STIRRUPS
C    VSTMP1 - STIRRUP SHEAR AT END OF TAPER
C    VSTMP2 - STIRRUP SHEAR AT HARPING PT.
C    VSL    - MAXIMUM ALLOWABLE SHEAR STRENGTH PROVIDED BY STIRRUPS
C    VSLT   - MAXIMUM ALLOWABLE SHEAR STRENGTH PROVIDED BY STIRRUPS
C    VSTH   - VERTICAL COMPONENT OF HARPED STRANDS
C    VS18   - STIRRUP SHEAR FOR 18" SPACING
C    VU     - TOTAL ULTIMATE SHEAR AT END OF END BLOCK
C    WC     - CURB WEIGHT TO GIRDER
C    WG     - GIRDER DEAD LOAD IN KIPS/FT
C    WS     - SLAB WEIGHT OF GIRDER
C    WT     - WEIGHT OF SLAB + DIAPH PER FOOT OF GIRDER
C    XCGTT  - TEMPORARY STORAGE FOR CGTT
C    XHALF  - DISTANCE TO D/2 POINT
C    XHARP  - DISTANCE FROM HARPING POINT TO CL BEARING
C    XI     - IMPACT MULTIPLIER
C    XIEB   - MOMENT OF INERTIA OF END BLOCK (INCHES**4)
C    XIG    - MOMENT OF INERTIA OF GIRDER (INCHES**4)
C    XL     - SPAN LENGTH (CL BRG TO CL BRG)
C    XLS    - DISTANCE FROM CL BRG TO 18" STIRRUP SPACING
C    XLEB   - LENGTH OF END BLOCK FROM CL BRG INCLUDING TAPER
C    XML    - LIVE LOAD MOMENT (HAND INPUT)
C    XNL    - VI/MMAX AT D/2. (SEE AASHTO 9.20) DUE TO LL+I AND CURB DL
C    XNH    - NUMBER OF HARPED STRANDS
C    XNS    - NUMBER OF STRAIGHT STRANDS
C    XNTS   - TOTAL NUMBER OF STRANDS
C    XNTS1  - TEMPORARY VARIABLE
C    XNTS2  - TEMPORARY VARIABLE
C    XS     - EXTRA SLAB WIDTH FOR D.L. ONLY!!! IN INCHES
C    XTMP   - XL/2.
C    XXL    - (X/2)(X-L) IN FORMULA MX=W(X/2)(X-L) FOR MOMENT AT
C               DISTANCE X FROM END OF BEAM
C    XXX    - SPECIAL (HS) LOADING
C    YB     - DISTANCE FROM BOTTOM OF GIRDER TO C.G. GIRDER (INCHES)
C    YBG    - DISTANCE FROM BOTTOM OF GIRDER TO C.G. GIRDER (INCHES)
C    YBC    - DISTANCE FROM BOTTOM OF GIRDER TO C.G. COMPOSITE SECTION
C    YBEB   - DISTANCE FROM BOTTOM OF GIRDER TO C.G. END BLOCK
C    YT     - DISTANCE FROM TOP OF GIRDER TO C.G. GIRDER (INCHES)
C    YTEB   - CENTROID DISTANCE TO TOP OF END BLOCK (INCHES)
C    YTCGH  - DISTANCE FROM TOP TO C.G. HARPED AT END
C    YTS    - DISTANCE OF C.G. TOTAL STRANDS FROM BOTTOM @ MIDSPAN
C    YTSEB  - HEIGHT OF C.G. OF STRANDS IN END BLOCK
C    ZB     - SECTION MODULUS, BOTTOM
C    ZBC    - SECTION MODULUS, BOTTOM, COMPOSITE
C    ZBEB   - SECTION MODULUS, BOTTOM, END BLOCK
C    ZBEBC  - SECTION MODULUS, BOTTOM, COMPOSITE END BLOCK
C    ZT     - SECTION MODULUS, TOP
C    ZTC    - SECTION MODULUS, TOP, COMPOSITE
C    ZTG    - SECTION MODULUS OF TOP FIBER OF GIRDER (INCHES**3)
C    ZTEB   - TOP GIRDER SECTION MODULUS OF END BLOCK
C    ZTEBC  - TOP GIRDER SECTION MODULUS OF COMPOSITE END BLOCK
C
C
C
      CHARACTER*32 CHK,BULB,BLOCK,RELAX,SIMPLE,CURB,CLOAD,CLOC,LOSS
      REAL LLDXI,MCR,MMAX,FILFT,FISHP,FLLFT,FLSHP,MCRG,MCRC
      REAL CGS(100),DCGH(100),DCGHI(100)
      INTEGER*4 U, CHKCG
      INTEGER*4 NONE,   NTWO,   NTHREE,   NFOUR
      CHARACTER*1 CARCNT
      CHARACTER*8 REVDAT, TIME                                          .C.
      CHARACTER*11 DATE                                                 .C. 
      INTEGER*4 ERUNIT,OUNIT,TUNIT,BGUNIT  
      LOGICAL DEFLCT,PPOINT
      COMMON /PARMIN/ IRS,IDEF,IPICK,OUNIT,ERUNIT,INUNIT,TUNIT,K2
      COMMON /REV/ TIME,REVDAT,CARCNT,DATE                              .C.
      CURB='BLANK'
      BGUNIT=2
      L=0
      NONE=1
      NTWO=2
      NTHREE=3
      NFOUR=4
      NFIVE=5
C
C
      DEFLCT=.FALSE.
      PPOINT=.FALSE.
      IF(IDEF.EQ.1.AND.IRS.EQ.1)DEFLCT=.TRUE.
      IF(IPICK.EQ.1.AND.IRS.EQ.1)PPOINT=.TRUE.
C
C
C
C
      IF (IRS.NE.1 .AND. IRS.NE.2) GO TO 2420
      IF (IRS.EQ.1) GO TO 10
      GO TO 30
C
C          READ - ERROR MESSAGES
C
 2420 IF(ERUNIT.EQ.3)WRITE(ERUNIT,2430)
      WRITE (OUNIT,2430)
 2430 FORMAT(//' ERRONEOUS PROGRAM NO. ENTERED - TERMINATING RUN',//)
      GO TO 2495
 2440 IF(ERUNIT.EQ.3)WRITE(ERUNIT,2450)
      WRITE (OUNIT,2450)
 2450 FORMAT(//' DATA IS ERRONEOUS - TERMINATING RUN',//)
      GO TO 2495
 2452 WRITE (OUNIT,2453)LTYPE
      IF(ERUNIT.EQ.3)WRITE(ERUNIT,2453)LTYPE
 2453 FORMAT (/' BAD RECORD TYPE ',I4,' - TERMINATING RUN.')
      GO TO 2495
 2460 IF(ERUNIT.EQ.3)WRITE(ERUNIT,2470)
      WRITE (OUNIT,2470)
 2470 FORMAT(//' WRONG LINE NUMBER - TERMINATING RUN',//)
      GO TO 2495
 2495 RETURN
C
C       WRITE RECORDS FOR DFLECT AND PICKPT SUBROUTINES
C
 4000 NCSI=CSI
C         YB=XIG/ZT
          YBSAVE=YB
 4010 IF (.NOT. DEFLCT) GO TO 4015
          WRITE (TUNIT,*)NTHREE,DLG,YB,AG,ZT,XIG,YT
          WRITE (TUNIT,*)NFOUR,NOPBM,KSER,XL,AS,USS,CGS(NS),
     1    CGH,DCGH(NH),NS,NH
          FCUDF=FCU*1.2
          SLABFDF = SLABF * 1.2                                         .C.
          WRITE(TUNIT,*)NCSI,YBC,GTS,S,SBXML,WT,WC,NDAYS,FCUDF,KSTY
          WRITE(TUNIT,*)NFIVE,XHARP,SLABFDF,CREEP                       .C. 
          REWIND TUNIT
          CALL DFLECT
          REWIND TUNIT
 4015 IF (.NOT. PPOINT) GO TO 4020
          WRITE (TUNIT,*)NONE,NOPBM,NH,NS,XL,XHARP,
     1    FICU,FCU,AG,WG,ZT,ZB,YBSAVE,CGH,CGS(NS)
          WRITE (TUNIT,*) NTWO,PPT,FILFT,FISHP,
     1    FLLFT,FLSHP,AS,USS,KSTY
      REWIND TUNIT
      CALL PICKPT
      REWIND TUNIT
 4020 GO TO 110
C
C
C     IRS = 1 -- PROGRAM PSGDES1 FREE FORMAT
C
C     FIRST FREE FORMAT READ FOR PSGDES1
C
10    READ(INUNIT,END =2495,ERR=2440,FMT=*)J,IDENT,NCE,NH,NS            .C.
      IF (J.NE.1) GO TO 2460
      IF(NCE.NE.0)L=1
C
C  INITIALIZE PSGDES VERSION 2 VARIABLES
C
      KEB=0                                                             .C.
      KSTY=0                                                            .C.
      KTEST=1
      KEND=0
      DLG=0.
      YBG=0.
      AG=0.
      ZTG=0.
      XIG=0.
      BTF=0.
      TFLNG=0.
      AEB=0.
      MAXNS=-1
      MAXNH=-1
      XIEB=0.
      YTEB=0.
      FCU=0.
      XXX=0.
      A2=0.
      FICU=0.
      FILFT=0.0
      FISHP=0.0
      FLLFT=0.0
      FLSHP=0.0
      SLABF=5.0                                                         .C.
      CREEP=0
      NDAYS=0                                                           .C.  
      PPT=0
      GO TO 50
C
C
   30 IF (IRS.EQ.1) GO TO 50
C
C     FIRST FREE FORMAT READ FOR PSGDES2
C
   40 LTYPE=1
      READ(INUNIT,END=2495,ERR=2452,FMT=*)LTYPE
      IF (LTYPE .NE. 1) THEN
         LTYPE=1
         GO TO 2452
      END IF
      BACKSPACE INUNIT
      READ(INUNIT,END=2452,ERR=2452,FMT=*)LTYPE,IDENT,KEB,DLG,YBG,
     1       AG,ZTG,XIG,BTF,TFLNG,MAXNH,MAXNS
      KTEST=1
      KEND=0
      LTYPE=2
      READ(INUNIT,END=2440,ERR=2452,FMT=*)LTYPE
      IF (LTYPE .NE. 2) THEN
         LTYPE=2
         GO TO 2452
      END IF
      BACKSPACE INUNIT
      READ(INUNIT,END=2440,ERR=2452,FMT=*)LTYPE,AEB,XIEB,YTEB,
     1      NCE,NH,NS,FICU,FCU,XXX,A2,KSTY
      READ(INUNIT,END=2440,ERR=2440,FMT=*)LTYPE
      BACKSPACE INUNIT
      IF(NCE.NE.0)L=1
C
C   SEE IF CHECK OR DESIGN AND SET DEFAULT CONC. FC
C
   50 CHECK=1
      IF (NS.EQ.0.AND.NH.EQ.0)CHECK=0
      IF (FICU.LE..01)FICU=6.                                           .C.
      IF (FCU.LE..01)FCU=7.                                             .C.
  110 ICOUNT=0
      IBULBT=0
      KKONT = 0
      U=0
      IF(CHECK.EQ.0)THEN
         NS=0
         NH=0
         NTS=0
      END IF
C
      IF (IRS.EQ.2) GO TO 160
C
C     SECOND FREE FORMAT READ FOR PSGDES1
C
      READ(INUNIT,END=115 ,ERR=2440,FMT=*)J
      BACKSPACE INUNIT
      IF(J.EQ.1)GO TO 10
      IF(J.NE.2)GO TO 2460
      READ(INUNIT,END=2440,ERR=2440,FMT=*)J,NOPBM,KSER,XL,DLL,GTS,S,
     1        A,GIR,WC,CGHO,ASPH
      IF ( KSER .EQ. 3 .OR. KSER .EQ. 53 .OR. KSER .EQ. 35 ) IBULBT = 1
      IF (GTS.LE.A.OR.IBULBT.EQ.1)   GO TO 120
      WRITE (OUNIT,2780) NOPBM
      IF(ERUNIT.EQ.3)WRITE(ERUNIT,2780) NOPBM
      GO TO 110
115   IF(KTEST.EQ.1)GO TO 2440
      GO TO 2495
C
C   INITIALIZE OTHER PSGDES VERSION 2 VARIABLES
C
120   WS=0.
      XNL=0.
      XML=0.
      AS=0.
      USS=0.
      CGSO=0.
      DCGHO=0.
      AVFY=0.
      A3=0.
      XS=0.
      UXHARP=0.
      BEFF=0.
      FLOS=0.
      XLEB=0.0                                                          .C.
      EBTL=0.0                                                          .C.
      EBT=0.0                                                           .C.
      GO TO 220
C
C
  160 IF (IRS.EQ.1) GO TO 220
C
C     SECOND FREE FORMAT READ FOR PSGDES2
C     IF DATA GIVEN FOR DEFLECTION AND PICKPT PROGRAMS
C     THEN SET VARIABLES TO EXECUTE THEM.
C
      IF(KEND.EQ.1)RETURN
      IF(KTEST.EQ.0)THEN
         READ(INUNIT,END=2495,ERR=2440,FMT=*)LTYPE
         BACKSPACE INUNIT
      END IF
      DEFLCT=.FALSE.
      PPOINT=.FALSE.
      IF (LTYPE .NE. 3) THEN
         IF (LTYPE .EQ. 1.AND.KTEST.NE.1)GO TO 30
         GO TO 2452
      END IF
      READ(INUNIT,ERR=2452,END=2440,FMT=*)LTYPE,NOPBM,KSER,XL,AS,USS,
     1         WS,XNL,XML,DLL,GTS
      LTYPE=4
      READ(INUNIT,END=2440,ERR=2452,FMT=*)LTYPE
      IF (LTYPE .NE. 4) THEN
         LTYPE=4
         GO TO 2452
      END IF
      BACKSPACE INUNIT
      READ(INUNIT,END=2440,ERR=2452,FMT=*)LTYPE,S,A,GIR,WC,CGSO,DCGHO,
     1      CGHO,AVFY,A3,XS,UXHARP,ASPH,BEFF,FLOS,SLABF,XLEB,EBTL,EBT   .C.
      READ(INUNIT,END=217,ERR=2440,FMT=*)LTYPE
      BACKSPACE INUNIT
      IF(LTYPE.EQ.3.OR.LTYPE.EQ.1)GO TO 220
      IF(LTYPE.EQ.5)THEN
         DEFLCT=.TRUE.
         READ(INUNIT,END=2440,ERR=203,FMT=*)LTYPE,CREEP,NDAYS           .C.
         GO TO 207
      END IF
      IF(LTYPE.NE.6)GO TO 2452
      GO TO 210
  203 IF(ERUNIT.EQ.3)WRITE(ERUNIT,204)
  204 FORMAT(/,' BAD DEFLECTION DATA  ... PROCESS CONTINUEING')
      DEFLCT=.FALSE.
  207 READ(INUNIT,END=217,ERR=2440,FMT=*)LTYPE
      BACKSPACE INUNIT
      IF(LTYPE.EQ.3.OR.LTYPE.EQ.1)GO TO 220
  210 IF(LTYPE.EQ.6)THEN
         PPOINT=.TRUE.
         READ(INUNIT,END=2440,ERR=212,FMT=*)LTYPE,PPT,FILFT,FISHP,FLLFT,
     1                                      FLSHP
         GO TO 220
      END IF
      GO TO 2452
  212 IF(ERUNIT.EQ.3)WRITE(ERUNIT,215)
  215 FORMAT (/,' BAD PICK POINT DATA ... PROCESS CONTINUEING')
      PPOINT=.FALSE.
      GO TO 220
  217 KEND=1
C
C   WRITE TITLE FIRST LINE
C
  220 WRITE (OUNIT,2530) CARCNT,DATE,TIME
C
C  SET DEFAULT FOR FINAL DESIGN LOSSES
C
      IF(FLOS.LE..001)FLOS=45.
C
C
C  SET UP TITLE CHARACTER VARIABLES FOR LINES 2 AND 3
C
      IF(KSER.EQ.3.OR.KSER.EQ.35.OR.KSER.EQ.53)IBULBT=1
      CHK=' DESIGN'
      IF (CHECK.NE.0)CHK=' CHECK'
      SIMPLE=' CONT. SPAN SHEAR DESIGN ***'
      IF (L.EQ.0) SIMPLE=' SIMPLE SPAN SHEAR DESIGN ***'
      IF (KSTY.EQ.1)THEN
            RELAX=' (STRESS RELIEVED STRANDS)'
            CSTR=.7
         ELSE
            RELAX=' (LOW RELAXATION STRANDS)'
            CSTR=.75
         END IF
      BULB=' NONCOMPOSITE PRESTRESSED GIRDER'
      IF (IBULBT.EQ.0) BULB='    COMPOSITE PRESTRESSED GIRDER'
      BLOCK=' WITH END BLOCKS'
      IF (KEB.EQ.0) BLOCK='  NO END BLOCKS '
      LOSS=' FINAL LOSSES = '
      WRITE (OUNIT,250) BULB,CHK,REVDAT,BLOCK,SIMPLE,LOSS,FLOS
  250 FORMAT(/,'        ***',A32,A7,'   ***  REV. ',A8,'  ***',//,
     1 '        ***',A16,' ***',A29,A16,F4.0)
C
C  CHECK FOR INVALID TYPE                                               .C.
C
      IF (KSER.EQ.35.AND.S.LT.4.08.OR.KSER.EQ.53.AND.S.LT.4.08)GO TO 270.C.
      IF (KSER.EQ.35.AND.S.GT.6.01.OR.KSER.EQ.53.AND.S.GT.6.01)THEN
          WRITE (OUNIT,255) KSER
          IF(ERUNIT.EQ.3)WRITE(ERUNIT,255) KSER
  255     FORMAT (//'        GIRDER TYPE',I3,' TRANSVERSE SLAB DESIGN MU.C.
     1ST BE',/,'        CHECKED FOR GIRDER SPACING GREATER THAN 6''-0"',.C.
     2//)                                                               .C.
      END IF
C
C   GO TO GIRDER PROPERTIES BY TYPE NUMBER                              .C.
C
      IF (KSER.EQ. 2) GO TO 385                                         .C.
      IF (KSER.EQ. 3) GO TO 406                                         .C.
      IF (KSER.EQ.42) GO TO 450                                         .C.
      IF (KSER.EQ.50) GO TO 470                                         .C.
      IF (KSER.EQ.58) GO TO 410                                         .C.
      IF (KSER.EQ.73) GO TO 395                                         .C.
      IF (KSER.EQ.35) GO TO 400                                         .C.
      IF (KSER.EQ.53) GO TO 403                                         .C.
      GO TO 370                                                         .C.
C
C   STANDARD NONCOMPOSITE GIRDERS WITH INVALID VALUES ERROR MESSAGES
C
  260 WRITE (OUNIT,265) NOPBM,KSER
      IF(ERUNIT.EQ.3)WRITE(ERUNIT,265) NOPBM,KSER
  265 FORMAT (//'        PROBLEM',I3,' GIRDER TYPE',I3,' IS INVALID',/  .C.
     1,         '                   WHEN FINAL CAMBER IS NEGATIVE   '
     2,//)
      GO TO 110
  270 WRITE (OUNIT,275) NOPBM,KSER
      IF(ERUNIT.EQ.3)WRITE(ERUNIT,275) NOPBM,KSER
  275 FORMAT (//'        PROBLEM',I3,' GIRDER TYPE',I3,' IS INVALID',/  .C.
     1,         '                   FOR GIRDER SPACING LESS THAN 49"'   .C.
     2,//)
      GO TO 110
C
C   INVALID TYPE MESSAGE                                                .C.
C
  370 WRITE (OUNIT,380) NOPBM,KSER
      IF(ERUNIT.EQ.3)WRITE(ERUNIT,380) NOPBM,KSER
  380 FORMAT (//'        PROBLEM',I3,' GIRDER TYPE',I3,' IS INVALID'//  .C.
     1)
      GO TO 110
C
C  ANY GIRDER PROPERTIES
C
  385 WG=DLG
      ZB=XIG/YBG
      YB=YBG
      ZT=ZTG
      ZTEB=XIEB/YTEB
      YT=XIG/ZTG
      YBEB=YT+YB-YTEB
C
C   ASSIGN SOME STD GIRDER PROPERTIES BY HEIGHT OF GIRDER               .C.
C                                                                       .C.
      IF (YT+YB.GT.50.)THEN                                             .C.
           IF(MAXNH.LT.0.)MAXNH=24                                      .C.
           XHARP=0.4*XL                                                 .C.
           GO TO 490                                                    .C.
      ELSE                                                              .C.
           IF (YT+YB.GT.42.)THEN                                        .C.
              IF(MAXNH.LT.0.)MAXNH=24                                   .C.
              XHARP=0.333*XL                                            .C.
              GO TO 490                                                 .C.
           ELSE                                                         .C.
              IF(MAXNH.LT.0.)MAXNH=16                                   .C.
              XHARP=0.333*XL                                            .C.
              GO TO 490                                                 .C.
           END IF                                                       .C.
      END IF                                                            .C.
C                                                                       .C.
C                       GIRDER TYPE W73G                                .C.
  395 WG=0.830                                                          .C.
      ZB=14342.                                                         .C.
      YB=38.08                                                          .C.
      AG=746.7                                                          .C.
      ZT=15417.                                                         .C.
      YT=35.42                                                          .C.
      BTF=43.                                                           .C.
      TFLNG=4.19                                                        .C.
      KEB=0                                                             .C.
      IF(MAXNH.LT.0)MAXNH=24                                            .C.
      IF ( NH .LE. MAXNH ) GO TO 399                                    .B.
  397      WRITE (OUNIT,2610) IDENT,NOPBM,NH,NS,MAXNH                   .B.
           WRITE (ERUNIT,2610) IDENT,NOPBM,NH,NS,MAXNH                  .B.
           IF (K2 .EQ. 1 ) WRITE ( BGUNIT, 2610 ) IDENT,NOPBM,NH,NS,    .B.
     1                                            MAXNH                 .B.
           GO TO 2495                                                   .B.
  399 XHARP=0.4*XL                                                      .C.
      GO TO 490                                                         .C.
C                       GIRDER TYPE W35DG                               .C.
  400 KEB = 0
      BTF = S * 12.
      IF ( BTF .GT. 78. ) BTF = 78.                                     .C.
      XHARP = 0.4 * XL
      WG = .758 + ( S - 4.083 ) * 0.08                                  .C.
      ADSLAB = ( BTF - 49. ) * 6.                                       .C.
      SLABI = ( BTF - 49. ) * 18.                                       .C.
      YB = ( 682.4 * 21.23 + ADSLAB * 32. ) / ( ADSLAB + 682.4 )        .C.
      AG = 682.4 + ADSLAB                                               .C.
      ADY2G = 682.4 * ( ABS ( YB - 21.23 ) ) ** 2.                      .C.
      ADY2S = ADSLAB * ( ABS ( YB - 32. ) ) ** 2.
      ZB = ( 101659.3 + SLABI + ADY2G + ADY2S ) / YB                    .C.
      YT = 35. - YB
      ZT = ZB * YB / YT
      GTS = 0.
      TFLNG = ( 380.9 + ADSLAB ) / BTF                                  .C.
      IF ( MAXNH .LT. 0 ) MAXNH = 24                                    .C.
      IF ( NH .GT. MAXNH ) GO TO 397                                    .B.
      IF ( MAXNS .LT. 0 ) MAXNS = 26                                    .C.
      IF ( NS .LE. MAXNS ) GO TO 490                                    .B.
  401      WRITE (OUNIT,2615) IDENT,NOPBM,NH,NS,MAXNS                   .B.
           WRITE (ERUNIT,2615) IDENT,NOPBM,NH,NS,MAXNS                  .B.
           IF (K2 .EQ. 1 ) WRITE ( BGUNIT, 2615 ) IDENT,NOPBM,NH,NS,    .B.
     1                                            MAXNS                 .B.
           GO TO 2495                                                   .B.
C                       GIRDER TYPE W53DG                               .C.
  403 KEB = 0
      BTF = S * 12.
      IF ( BTF .GT. 78. ) BTF = 78.                                     .C.
      XHARP = 0.4 * XL
      WG= .878 + ( S - 4.0833 ) * 0.08                                  .C.
      ADSLAB = ( BTF - 49. ) * 6.                                       .C.
      SLABI = ( BTF - 49. ) * 18.                                       .C.
      YB=( 790.4 * 31.88 + ADSLAB * 50. ) / ( ADSLAB + 790.4)           .C.
      AG= 790.4 + ADSLAB                                                .C.
      ADY2G = 790.4 * ( ABS ( YB - 31.88 ) ) ** 2.                      .C.
      ADY2S = ADSLAB * ( ABS ( YB - 50. ) ) ** 2.
      ZB = ( 295053.1 + SLABI + ADY2G + ADY2S ) / YB                    .C.
      YT = 53. - YB
      ZT = ZB * YB / YT
      GTS = 0.
      TFLNG = ( 380.9 + ADSLAB ) / BTF                                  .C.
      IF ( MAXNH .LT. 0 ) MAXNH = 24                                    .C.
      IF ( NH .GT. MAXNH ) GO TO 397                                    .B.
      IF ( MAXNS .LT. 0 ) MAXNS = 26                                    .C.
      IF ( NS .GT. MAXNS ) GO TO 401                                    .B.
      GO TO 490
C                       ANY NONCOMPOSITE GIRDER
  406 KEB=0
      IF ( TFLNG .LE. 0. ) TFLNG = 7.19
      XHARP=0.4*XL
      GTS=0.
      WG=DLG
      ZB=XIG/YBG
      YB=YBG
      ZT=ZTG
      YT=XIG/ZTG
      IF(YT+YB.GE.35.)GO TO 490
      GO TO 500
C                       GIRDER TYPE W58G                                .C.
  410 WG=0.671                                                          .C.
      ZB=9449.                                                          .C.
      YB=28.00                                                          .C.
      AG=603.5                                                          .C.
      ZT=8821.                                                          .C.
      YT=30.00                                                          .C.
      BTF=25.
      TFLNG=6.                                                          .C.
      KEB=0                                                             .C.
  420 IF(MAXNH.LT.0)MAXNH=18                                            .C.
      IF ( NH .GT. MAXNH ) GO TO 397                                    .B.
      XHARP=.4*XL
      GO TO 490
C                       GIRDER TYPE W42G                                .C.
  450 WG=0.415                                                          .C.
      ZB=4017.                                                          .C.
      YB=18.94                                                          .C.
      AG=373.3                                                          .C.
      ZT=3300.                                                          .C.
      YT=23.06                                                          .C.
      BTF=15.0                                                          .C.
      TFLNG=4.25
      KEB=0                                                             .C.
  460 IF(MAXNH.LT.0)MAXNH=14                                            .C.
      IF ( NH .GT. MAXNH ) GO TO 397                                    .B.
      XHARP=.333*XL
      GO TO 500
C                       GIRDER TYPE W50G                                .C.
  470 WG=0.584
      ZB=7232.
      YB=22.81
      AG=525.5
      ZT=6067.
      YT=27.19
      BTF=20.0
      TFLNG=6.
      KEB=0                                                             .C.
  480 IF(MAXNH.LT.0)MAXNH=12                                            .C.
      IF ( NH .GT. MAXNH ) GO TO 397                                    .B.
      XHARP=.333*XL
C
C                      FIND CG STRAIGHT STRANDS
C
C                         25" FLANGES                                   .C.
  490 DO 491 I=1,10
  491 CGS(I)=1.875                                                      .C.
      CGS(11)=2.057                                                     .C.
      CGS(12)=2.208                                                     .C.
      CGS(13)=2.337                                                     .C.
      CGS(14)=2.446                                                     .C.
      CGS(15)=2.542                                                     .C.
      CGS(16)=2.625                                                     .C.
      CGS(17)=2.699                                                     .C.
      CGS(18)=2.764                                                     .C.
      CGS(19)=2.928                                                     .C.
      CGS(20)=3.075                                                     .C.
      CGS(21)=3.208                                                     .C.
      CGS(22)=3.330                                                     .C.
      CGS(23)=3.353                                                     .C.
      CGS(24)=3.375                                                     .C.
      CGS(25)=3.475                                                     .C.
      CGS(26)=3.567                                                     .C.
      IF(MAXNS.LT.0)MAXNS=26                                            .C.
      IF ( NS .GT. MAXNS ) GO TO 401                                    .B.
      GO TO 520
C                          20" FLANGES                                  .C.
  500 DO 501 I=1,6                                                      .C.
  501 CGS(I)=1.875                                                      .C.
      CGS( 7)=2.018                                                     .C.
      CGS( 8)=2.125                                                     .C.
      CGS( 9)=2.319                                                     .C.
      CGS(10)=2.475                                                     .C.
      CGS(11)=2.602                                                     .C.
      CGS(12)=2.708                                                     .C.
      CGS(13)=2.798                                                     .C.
      CGS(14)=2.875                                                     .C.
      IF(MAXNS.LT.0)MAXNS=14                                            .C.
      IF ( NS .GT. MAXNS ) GO TO 401                                    .B.
      GO TO 520
C
C    SET DEFAULT CG HARPED IF NONE GIVEN
C
  520 NOVER=0
      CRAISE=0.
      CHKCG=1
      KCG=0
      IF (CGHO.LE..01)THEN
         CGH=3.0
         CHKCG=0.
      ELSE
         CGH=CGHO
      END IF
C
C   SET PROPERTIES FOR NO END BLOCK IF INDICATED
C
      IF (KEB.EQ.0)THEN
         AEB=AG
         ZTEB=ZT
         YBEB=YB
      END IF
C
C  SET WEB THICKNESS IF NONE GIVEN
C
      IF(A3.LT..01)A3=6.                                                .C.
C
C  FIND SOME GIRDER PROPERTIES
C
      YTEB=YT+YB-YBEB
      EBI=YTEB*ZTEB
      ZBEB=EBI/YBEB
C
C  OVERIDE CG STRAIGHT IF CGSO GIVEN
C
      IF(CGSO.GT..01)THEN
         DO 530 I=1,MAXNS
  530    CGS(I)=CGSO
      END IF
C
C  FIND RISE IN HARPED STRANDS
C
  540 H=YT+YB-CGH
      IF(K2.EQ.1)THEN
         IF(KCG.NE.0)WRITE(BGUNIT,*)' HARPED STRANDS RAISED'
      END IF
         DCGH(1)=H-2.0                                                  .C.
         DCGH(2)=H-2.0                                                  .C.
         DCGH(3)=H-2.667                                                .C.
         DCGH(4)=H-3.0                                                  .C.
         DCGH(5)=H-3.6                                                  .C.
         DCGH(6)=H-4.00                                                 .C.
         DCGH(7)=H-4.571                                                .C.
         DCGH(8)=H-5.0                                                  .C.
         DCGH(9)=H-5.56                                                 .C.
         DCGH(10)=H-6.0                                                 .C.
         DCGH(11)=H-6.545                                               .C.
         DCGH(12)=H-7.0                                                 .C.
         DCGH(13)=H-7.538                                               .C.
         DCGH(14)=H-8.0                                                 .C.
         DCGH(15)=H-8.533                                               .C.
         DCGH(16)=H-9.0                                                 .C.
         DCGH(17)=H-9.529                                               .C.
         DCGH(18)=H-10.0                                                .C.
         DCGH(19)=H-10.526                                              .C.
         DCGH(20)=H-11.0                                                .C.
         DCGH(21)=H-11.524                                              .C.
         DCGH(22)=H-12.0                                                .C.
         DCGH(23)=H-12.522                                              .C.
         DCGH(24)=H-13.0                                                .C.
         DO 550 I = 1 , MAXNH
 550     DCGHI(I) = DCGH(I)
C
C  OVERIDE RISE IN HARPED STRANDS IF DCGHO GIVEN
C
      IF(DCGHO.GT..01)THEN
         CHKCG=1
         DO 570 I=1,MAXNH
  570    DCGH(I)=DCGHO
      END IF
C
      IF(KCG) 1290, 660, 1290
C
  660 ESTL=28000.
C
C  OVERIDE STD HARPING PT IF ONE IS GIVEN
C
      IF (UXHARP.GE..01)XHARP=UXHARP
      IF(K2.EQ.1)WRITE(BGUNIT,*)' FINDING GIRDER CONSTANTS'
      IF (USS.LE..01)USS=270.
C
C  FIND WT OF ASPHALT OVERLAY
C
      ASP=ASPH
      ASPH=S*ASPH*.0125
C
C  FIND CURB LOAD ON GIRDER
C
      IF(WC.LT.0)THEN
         TWC=.427
         CURB=' NEW JERSEY'
         IF (ASPH.GT..001) TWC=.470
         IF (A2.LE..01) A2=3.
         IF (GIR.LE..01) GIR=2.*A2
         WC=2.*TWC/GIR
         IF(GIR.GT.2*A2)WC=TWC/A2
      END IF
C
C FIND SLAB LOAD ON GIRDER
C
      IF (WS.LE..01.AND.IBULBT.NE.1)THEN
         SLABA=((S+XS/12.)*GTS+.0833*BTF*(A-GTS))/12.
         IF (A.LE.GTS) SLABA=(S+XS/12.)*GTS/12.
         WS=SLABA*0.16
      END IF
      IF(IBULBT.EQ.1) THEN
           WS = 0.0
C ADD CAMBER THICKENING TO NONCOMPOSITE GIRDER WEIGHT (PARABOLIC SHAPE
C CAUSES 1/6TH OF THE MOMENT OF A UNIFORM LOAD  WL**2/8/6)
           WG = WG +  A * S * .0022222
           IF(A.LT.0) GO TO 260
      END IF
      WC=WC+ASPH
C
C  FIND NO. AND WT OF DIAPHRAGMS DEPENDING ON HEIGHT AND LENGTH OF
C  GIRDERS
C
      IF (XL.LE.40.) THEN
         DIAPH=0.
         DN=0
         DMOM=0
      ELSE
         IF ((YB+YT).LE.73.)DDP=2.583
         IF ((YB+YT).GT.73.)DDP=3.583
         DIAPH=((DDP+(A-GTS)/12.)*.667+.25)*.16*(S-A3/12.)
         IF(IBULBT.EQ.1)DIAPH=(YB+YT-TFLNG-6.)*(S*12.-A3)*.000740741
         IF (XL.LE.80.) THEN
            DMOM=DIAPH*XL/4.
            DN=1.
         ELSE
            DMOM=DIAPH*XL/3.
            DN=2.
         END IF
      END IF
C
C  SET SOME DEFAULTS AND CONSTANTS
C
      ESLAB = 63.6812 * SQRT ( SLABF * 1000. )                          .C.
      IF ( SLABF .LE. 0.0 ) ESLAB = 4503.                               .C.
      IF (AS.LE..01) AS = 0.153
      IF (AVFY.LE..01) AVFY = 37.2                                      .D.
C
C  SET END BLOCK THICK. TO WEB THICK.                                   .C.
C
      IF(KEB.EQ.0)EBT=A3/12.
      IF(KEB.EQ.1)EBT=EBT/12.
      EBW=(AEB-AG)/144.*.16*(XLEB-EBTL/2.)                              .C.
C
C  FIND REACTIONS
C
      GR1=WG*(XL/2.+1.25)+WS*(XL/2.+1.)+WC*(XL/2.+1.)+DN*DIAPH/2.+EBW
      GR2=GR1-WC*(XL/2.+1.)
C READ CONTROL VALUE SET TO 0
      KTEST=0
      IF(K2.EQ.1)WRITE(BGUNIT,*)' FINDING CONCRETE CONSTANTS'
      EGIRD = 63681.2 * SQRT ( FCU * 1000. ) / 1000.
      RATIO=ESLAB/EGIRD
C
C  FIND NET SLAB THICK.
C
      IF(ASPH.LE..01)THEN
         TH=GTS-0.5
         IF (TH.LT.0.) TH=0.
      ELSE
         TH=GTS
      END IF
      COMPA=-0.4*FCU
      COMPI=-0.6*FICU
      FT=0.003*SQRT(FCU*1000.)
      FTI=0.003*SQRT(FICU*1000.)
      FTTOP = FT
      FTITOP = FTI
      IF ( FT .GT. .200 ) FTTOP = .200
      IF ( FTI .GT. .200 ) FTITOP = .200
C
C  FIND EFFECTIVE FLANGE WIDTH
C
      B=BEFF
      IF(BEFF.LT..01)THEN
         B1=12.*XL/4.
         B2=S*12.
         B3=12.*TH+BTF/2.
         B=B1
         IF(B2.LT.B)B=B2
         IF(B3.LT.B)B=B3
      END IF
      IF(KSER .EQ. 3)B=0.00001
      IF(KSER .EQ. 35 .OR. KSER .EQ. 53) B = B2
C
C     COMPUTE COMPOSITE SECTION PROPERTIES
C
      DY=B*TH*RATIO*(YT+TH/2.)/(B*TH*RATIO+AG)
      GI=ZB*YB
      CSI=GI+(B*TH**3.)*RATIO/12.+B*TH*RATIO*(YT+TH/2.-DY)**2.+AG*DY**2.
      ZBC=CSI/(YB+DY)
      ZTC=CSI/(YT-DY)
      YBC=YB+DY
      YTC=YT-DY
      QSLAB=B*TH*RATIO*(YT-DY+TH/2.)
      EDY=B*TH*RATIO*(YT+YB-YBEB+TH/2.)/(B*TH*RATIO+AEB)
      EBI=ZTEB*(YT+YB-YBEB)
      ECSI=EBI+(B*TH**3.)*RATIO/12.+B*TH*RATIO*(YT+YB-YBEB+TH/2.-EDY)**2
     1   +AEB*EDY**2
      ZTEBC=ECSI/(YT+YB-YBEB-EDY)
      ZBEBC=ECSI/(YBEB+EDY)
C
C     COMPUTE LIVE LOAD MOMENTS
C
      IF (XML.LT..01.AND.DLL.GT..01) THEN
C
         XI=50./(XL+125.)+1.
         IF (XI .GT. 1.3) XI=1.3
         LLDXI=0.5*XI*(S/DLL)
         FXL = 0.5
         CLOAD = 'FLEX'
         CALL LIVLOD(DN,DIAPH,XL,FXL,LLDXI,WC,WG,WS,VI,XML,
     1   MMAX,VU,CLOAD,XXX)
C        IF (XXX.GE..01)XML=XML*XXX/25.                                 .D.
      END IF
C
C     COMPUTE  GIRDER LOAD STRESSES
C
      IF(K2.EQ.1)WRITE(BGUNIT,*)' BEGINNING STRESS COMPUTATIONS'
      XXL=1.5*XL*XL
      SBXML=12.*XML/ZBC
      STXML=12.*XML/ZTC
      SBDC=WC*XXL/ZBC
C  DELETED MOMENT AT CL GIR DUE TO END BLOCK WEIGHT (N.J.CAMPBELL)
      SBDG=WG*XXL/ZB
      SBDS=WS*XXL/ZB+DMOM*12./ZB
      WT=WS+DN*DIAPH/XL
      HARP=1.-(1.-XHARP/(XL/2.))**2.
      SBDCH=SBDC*HARP
      SBDGH=SBDG*HARP
      SBDSH=SBDS*HARP
      STDGH=-SBDGH*ZB/ZT
      XHALF=(YT+YB)/24.
      XXL=(XL/2.*XHALF-XHALF*XHALF/2.)*12.
      STEBC=-WC*XXL/ZTEBC
      STEBG=-WG*XXL/ZTEB
      STEBS=-WS*XXL/ZTEB
      SBEBC=WC*XXL/ZBEBC
      SBEBG=WG*XXL/ZBEB
      SBEBS=WS*XXL/ZBEB
      SBTOT=SBXML+SBDC+SBDG+SBDS
      FSS=CSTR*USS-FLOS
      PEACH=FSS*AS
      NTS=NS+NH
      IF (CHECK.NE.1)THEN
C
C  GET INITIAL ESTIMATE OF STRAND CONFIGURATION
C      (SKIP IF CHECK SECTION)
C
        E=YB-1.75
        P=(SBTOT-FT)/(1./AG+E/ZB)
        NTS=INT(P/PEACH)-1
        IF(NTS.LT.4)NTS=4
        IF(NTS.LE.MAXNS)THEN
          XNTS=NTS
          XNTS1=XNTS/2.
          NTS2=NTS/2
          XNTS2=NTS2
          DIF=XNTS1-XNTS2
          NS=XNTS-2.*DIF
          NTS=NS
        ELSE
          IF(NTS.LE.MAXNS+MAXNH)THEN
            NS=MAXNS
            NH=NTS-NS
          ELSE
            IF(ERUNIT.EQ.3)WRITE(ERUNIT,2600)NOPBM,NTS
            WRITE(OUNIT,2600)NOPBM,NTS
            GO TO 110
          END IF
        END IF
       END IF
      IF(K2.EQ.1)WRITE(BGUNIT,'(''  INITIAL STRANDS ARE'',I3,'' HARPED A
     1ND'',I3,'' STRAIGHT'')')NH,NS
C
C  INITIAL STRAND CONFIGURATION FOUND
C
C
C  STOP PROGRAM IF NOT CONVERGING
C
 1290 ICOUNT=ICOUNT+1
      IF (ICOUNT .GT. 100) GO TO 1500
      XNTS=NTS
      XNH=NH
      XNS=NS
      P=XNTS*PEACH
      BCGH=CGH
      IF (NH.GT.12.AND.CGHO.LE..01)CGH=CRAISE+6.-36./XNH
      IF (DCGHO.LE..01.AND.BCGH.NE.CGH)THEN
         DO 1370 I=1,MAXNH
         DCGHI(I)=DCGHI(I)-CGH+BCGH
 1370    DCGH(I)=DCGH(I)-CGH+BCGH
      END IF
C
C  ADJUST DELTA HARP CG IF SLOPE OVER 7 TO 1 FOR BULB TEES ONLY         .A.
C
      DCGHMX=XHARP*12./7.                                               .A.
      IF(DCGHO.LE..01.AND.NH.GT.0.AND.DCGHMX.LE.DCGH(NH))THEN           .C.
         DCGOVR=DCGHI(NH)-DCGHMX
         NOVER=INT(DCGOVR)+1
         IF(NOVER.LE.1)NOVER=0
         IF(NOVER.GT.0.AND.CHECK.EQ.0)THEN
             DCGH(NH)=DCGHI(NH)-NOVER
             WRITE(BGUNIT,*)'  LOWERING HARPED STRANDS AT D/2'
         END IF
      END IF
C                                                                       .A.
C  CHECK MAXIMUM HOLD DOWN FORCES (LIMIT TO 45 K FOR OPTIONAL HARPING)  .A.
C                 45 K IS MAXIMUM CENTRAL PREMIX CAN HANDLE             .A.
      IF (NH.GT.0)THEN                                                  .A.
         FHMX = 45.                                                     .A.
         FHOLD=DCGH(NH)/SQRT((12.*XHARP)**2+DCGH(NH)**2)                .C.
     1         *AS*NH*CSTR*USS                                          .A.
         IF ( CHECK .EQ. 0 .AND. FHOLD .GT. FHMX .AND. KKONT .EQ. 1)THEN.A.
             CALL RESET ( NS, NH, KKONT, K2, BGUNIT )                   .A.
             GO TO 1290                                                 .A.
         END IF                                                         .A.
      END IF                                                            .A.
C
C  FIND STRESSES AT D/2 FINAL
C
      IF (NH.LE.0)THEN
         STEB=STEBC+STEBG+STEBS-P/AEB+(YBEB-CGS(NS))*P/ZTEB
         SBEB=SBEBC+SBEBG+SBEBS-P/AEB-(YBEB-CGS(NS))*P/ZBEB
         CGTEB=CGS(NS)
         HHARP=0.
         HARPL=1.
      ELSE
         HHARP=(CGH+DCGH(NH))-DCGH(NH)*(12.*XHALF)/(12.*XHARP)          .C.
         HARPL=SQRT((12.*XHARP)**2+DCGH(NH)**2)/12.                     .C.
         CGTEB=(HHARP*(XHARP/HARPL)*XNH+CGS(NS)*XNS)/XNTS
         STEB=STEBC+STEBG+STEBS-P/AEB+(YBEB-CGTEB)*P/ZTEB
         SBEB=SBEBC+SBEBG+SBEBS-P/AEB-(YBEB-CGTEB)*P/ZBEB
      END IF
C
C TESTS FOR FINAL D/2 STRESSES
C
      KTD2A=0
      IF ( STEB .GT. FTTOP  .AND.  CHECK .EQ. 0  .OR.
     1                SBEB .LT. COMPA  .AND.  CHECK .EQ. 0 )THEN
          IF(K2.EQ.1)WRITE(BGUNIT,*)'  FINAL D/2'
          IF(K2.EQ.1)WRITE(BGUNIT,'(''   STRESS TOP = '',F9.4,''   ALLOW
     1.= '',F9.4,/,''   STRESS BOT = '',F9.4,''   ALLOW.= '',F9.4)')
     2    STEB,FTTOP,SBEB,COMPA
          CALL HPMR(NH,NS,NTS,BGUNIT,MAXNH,KTD2A,K2)
          IF(KTD2A.EQ.0)GO TO 1290
          GO TO 1500
      END IF
      KTD2B=0
      IF(SBEB.GT.FT.AND.CHECK.EQ.0.OR.STEB.LT.COMPA.AND.CHECK.EQ.0)THEN
             IF ( KKONT .EQ. 1 ) THEN
                 CALL RESET ( NS, NH, KKONT, K2, BGUNIT )               .A.
                 GO TO 1290
             END IF
          IF(K2.EQ.1)WRITE(BGUNIT,*)'  FINAL D/2'
          IF(K2.EQ.1)WRITE(BGUNIT,'(''   STRESS BOT = '',F9.4,''   ALLOW
     1.= '',F9.4,/,''   STRESS TOP = '',F9.4,''   ALLOW.= '',F9.4)')
     2  SBEB,FT,STEB,COMPA
           CALL HPLS(NH,NS,BGUNIT,MAXNS,KTD2B,K2)
           IF(KTD2B.EQ.0)GO TO 1290
           GO TO 1500
      END IF
C
C
C
C
      IF (NH.EQ.0)YTS=CGS(NS)
      IF (NH.NE.0)YTS=(CGS(NS)*XNS+CGH*XNH)/(XNTS)
      E=YB-YTS
      SBPRE=P*(1./AG+E/ZB)
      STPRE=P*(1./AG-E/ZT)
C
C     COMPUTE INITIAL STRESS AT HARPING PT
C     ASSUME 20KSI LOSSES AT RELEASE TO COMPUTE ELASTIC SHORTENING
C
      SSPRE=(SBPRE-(YTS/(YT+YB))*(SBPRE-STPRE))
     1        *(CSTR*USS-20.)/(CSTR*USS-FLOS)
      SSDG=SBDG*HARP-(YTS/YB)*SBDG*HARP
      FO=YT+YB-DCGH(NH)-CGH
      SBF=+(SBXML+SBDC+SBDG+SBDS)-SBPRE
      STTOT=-(SBDG+SBDS)*ZB/ZT-STXML-SBDC*ZBC/ZTC-STPRE
      PAR=(CSTR*USS-(ESTL/EGIRD)*(SSPRE-SSDG)-0.02*CSTR*USS)*XNTS*AS
      STH=STDGH-PAR/AG+PAR*E/ZT
      SBAH=-(PAR/AG+PAR*E/ZB-SBDGH)
C
C     COMPUTE INITIAL STRESSES AT D/2
C     ASSUME 20KSI LOSSES AT RELEASE TO COMPUTE ELASTIC SHORTENING
C
      PINIT = XNTS * AS * ( CSTR * USS - 20. )
      STEBI=STEBG-PINIT/AEB+(YBEB-CGTEB)*PINIT/ZTEB
      ZBEB=ZTEB*((YT+YB)/YBEB-1.)
      SBEBI=-STEBG*ZTEB/ZBEB-PINIT/AEB-(YBEB-CGTEB)*PINIT/ZBEB
      SSTRD=SBEBI-(HHARP/(YT+YB))*(SBEBI-STEBI)
      PART=(CSTR*USS+(ESTL/EGIRD)*SSTRD-0.02*CSTR*USS)*XNH*AS*
     1                                             (XHARP/HARPL)
      SSTRD=SBEBI-(CGS(NS)/(YT+YB))*(SBEBI-STEBI)
      PAR=(CSTR*USS+(ESTL/EGIRD)*SSTRD-0.02*CSTR*USS)*XNS*AS
      HF=(PART*HHARP+PAR*CGS(NS))/(PART+PAR)
      PAR=PART+PAR
      STEBI=STEBG-PAR/AEB+(YBEB-HF)*PAR/ZTEB
      SBEBI=-STEBG*ZTEB/ZBEB-PAR/AEB-(YBEB-HF)*PAR/ZBEB
C
C
C TESTS FOR D/2 STRESSES @ RELEASE
C
      KTDA=0
      IF(STEBI.GT.FTITOP.AND.CHECK.EQ.0.OR.
     1     SBEBI.LT.COMPI.AND.CHECK.EQ.0)THEN
          IF(K2.EQ.1)WRITE(BGUNIT,*)'  D/2 AT RELEASE'
          IF(K2.EQ.1)WRITE(BGUNIT,'(''   STRESS TOP = '',F9.4,''   ALLOW
     1.= '',F9.4,/,''   STRESS BOT = '',F9.4,''   ALLOW.= '',F9.4)')
     2    STEBI,FTITOP,SBEBI,COMPA
          CALL HPMR(NH,NS,NTS,BGUNIT,MAXNH,KTDA,K2)
          IF(KTDA.EQ.0)GO TO 1290
          GO TO 1500
      END IF
      KTDB=0
      IF(SBEBI.GT.FTI.AND.CHECK.EQ.0.OR.
     1     STEBI.LT.COMPI.AND.CHECK.EQ.0)THEN
             IF ( KKONT .EQ. 1 ) THEN
                 CALL RESET ( NS, NH, KKONT, K2, BGUNIT )               .A.
                 GO TO 1290
             END IF
          IF(K2.EQ.1)WRITE(BGUNIT,*)'  D/2 AT RELEASE'
          IF(K2.EQ.1)WRITE(BGUNIT,'(''   STRESS BOT = '',F9.4,''   ALLOW
     1.= '',F9.4,/,''   STRESS TOP = '',F9.4,''   ALLOW.= '',F9.4)')
     2    SBEBI,FTI,STEBI,COMPA
          CALL HPLS(NH,NS,BGUNIT,MAXNS,KTDB,K2)
          IF(KTDB.EQ.0)GO TO 1290
          GO TO 1500
      END IF
C
C
C     COMPUTE ULTIMATE MOMENTS AND CAPACITY
C     SEE AASHTO SECTION 9.17-9.18 (1985 VERSION)
C
C         GIRDER ONLY CONDITION
C
      SPECU=(WG+WS)*.1625*XL*XL+DMOM*1.3
      IF(GTS.LE..01.AND.WS.LE..01.AND.IBULBT.EQ.1)SPECU=SPECU+WC*.1625
     1  *XL*XL+2.167*XML
C
C CHECK AGAINST 1.2*MCRACK
C
      KUM=0
      MCRG=1.2*(.0075*SQRT(FCU*1000.)+SBPRE-SBDG)/12.*GI/YB
      IF(SPECU.LT.MCRG)SPECU=MCRG
      D=YB+YT-YTS
      PSTAR=XNTS*AS/BTF/D
      FSUG=USS*(1.-0.5*PSTAR*USS/FCU)
      ADIM=AS*XNTS*FSUG/.85/FCU/BTF
      IF (ADIM.LE.TFLNG) THEN
C
C  FIND CAPACITY OF RECT. SECT. WITH WIDTH OF TOP FLANGE
C
         CMUG=XNTS*AS*FSUG*D*(1.-0.6*PSTAR*FSUG/FCU)
         PMAX=PSTAR*FSUG/FCU
         IF(PMAX.GT..30)KUM=1
      ELSE
C
C  FIND CAPACITY OF FLANGED SECT.
C
         ASF=0.85*FCU*(BTF-A3)*TFLNG/FSUG
         ASR=XNTS*AS-ASF
         IF (ASR.LT.0.) ASR=0.
         CMUG=ASR*FSUG*D*(1.-0.6*ASR*FSUG/A3/D/FCU)+.85*FCU*
     1     (BTF-A3)*TFLNG*(D-0.5*TFLNG)
         PMAX=ASR*FSUG/A3/D/FCU
         IF(PMAX.GT..30)KUM=1
      END IF
C
C  IF OVER REINFORCED USE CONCRETE CONTROLLED ULT STR.
C
      IF(KUM.EQ.1)THEN
        CMUG=.25*A3*D*D*FCU+.85*FCU*(BTF-A3)*TFLNG*(D-0.5*TFLNG)
      END IF
      CMUG=CMUG/12.
C
C  FOR COMPOSITE CASE
C
      IF(GTS.GE..01.AND.WS.GE..01.AND.IBULBT.NE.1)THEN
        KUMC=0
        SPECUL=(WG+WS)*.1625*XL*XL+DMOM*1.3+WC*.1625*XL*XL+2.167*XML
C
C CHECK AGAINST 1.2*MCRACK
C
        MCRC=1.2*((.0075*SQRT(FCU*1000.)+SBPRE)*CSI/YBC/12.
     1       -(WT+WG)*XL*XL/8.*(CSI*YB/YBC/GI-1.))
        IF(SPECUL.LT.MCRC)SPECUL=MCRC
        D=YB+YT-YTS+TH
        PSTAR=XNTS*AS/B/D
        FSUC=USS*(1.-0.5*PSTAR*USS/4.0)
        ADIM1=AS*XNTS*FSUC/.85/4.0/B
        IF(ADIM1.LE.TH)THEN
C
C  FIND CAPACITY OF RECT. SECT. WITH WIDTH OF EFF. SLAB
C
          CMUC=XNTS*AS*FSUC*D*(1.-0.6*PSTAR*FSUC/4.0)
          PMAX=PSTAR*FSUC/4.0
          IF(PMAX.GT..30)KUMC=1
        ELSE
          ADIM=(AS*XNTS*FSUC/.85/4.-B*TH)/BTF
          IF(ADIM.LE.TFLNG)THEN
C
C  FIND CAPACITY OF FLANGED SECTION WITH NEUT. AXIS IN TOP GIR. FLNG.
C
             ASF=0.85*4.0*(B-BTF)*TH/FSUC
             ASR=XNTS*AS-ASF
             IF(ASR.LT.0.)ASR=0.
             CMUC=ASR*FSUC*D*(1.-0.6*ASR*FSUC/BTF/D/4.0)
     1        +0.85*4.0*(B-BTF)*TH*(D-0.5*TH)
             PMAX=ASR*FSUC/BTF/D/4.0
             IF(PMAX.GT..30)KUMC=1
          ELSE
C
C  FIND CAPACITY OF SECTION WITH NEUT. AXIS IN GIRDER WEB
C
             ASF=0.85*4.0*(B-BTF)*TH/FSUC+0.85*4.0
     1           *(BTF-A3)*(TH+TFLNG)/FSUC
             ASR=XNTS*AS-ASF
             IF(ASR.LT.0.)ASR=0.
             CMUC=ASR*FSUC*D*(1.-0.6*ASR*FSUC/A3/D/4.0)
     1        +0.85*4.0*(B-BTF)*TH*(D-0.5*TH)
     2        +0.85*4.0*(BTF-A3)*(TH+TFLNG)*(D-0.5*(TH+TFLNG))
             PMAX=ASR*FSUC/A3/D/4.0
             IF(PMAX.GT..30)KUMC=1
          END IF
        END IF
C  IF OVER REINFORCED USE CONC. CONTROLLED ULTIMATE STRENGTH
C
        IF(KUMC.EQ.1)THEN
           CMUC=.25*A3*D*D*4.0
     1             +.85*4.0*(B-BTF)*TH*(D-0.5*TH)
     2             +.85*4.0*(BTF-A3)*(TH+TFLNG)*(D-0.5*(TH+TFLNG))
        END IF
        CMUC=CMUC/12.
      END IF
C
C  CHECK ULTIMATE STRENGTHS
C
C       GIRDER ONLY
C
      KTULTG=0
      IF(SPECU.GT.CMUG)THEN
          IF(KUM.EQ.1.AND.CHECK.EQ.0)THEN
             KTULTG=1
             GO TO 1500
          END IF
          IF(KUM.EQ.0.AND.CHECK.EQ.0)THEN
             IF ( KKONT .EQ. 1 ) THEN
                 CALL RESET ( NS, NH, KKONT, K2, BGUNIT )               .A.
                 GO TO 1290
             END IF
             CALL ADSTR(NH,NS,NTS,MAXNS,MAXNH,BGUNIT,KTULTG,K2)
             IF(K2.EQ.1)WRITE(BGUNIT,'(''   GULT. CAP = '',F12.0,'' MULT
     1 = '',F12.0)')CMUG,SPECU
             U=1
             IF(KTULTG.EQ.0)GO TO 1290
          END IF
      END IF
C
C       COMPOSITE GIRDER
C
      KTULTC=0
      IF(SPECUL.GT.CMUC.AND.IBULBT.NE.1.AND.WS.GT..01.AND.GTS.GT..01)
     1  THEN
          IF(KUMC.EQ.1.AND.CHECK.EQ.0)THEN
             KTULTC=1
             GO TO 1500
          END IF
          IF(KUMC.EQ.0.AND.CHECK.EQ.0)THEN
             IF ( KKONT .EQ. 1 ) THEN
                 CALL RESET ( NS, NH, KKONT, K2, BGUNIT )               .A.
                 GO TO 1290
             END IF
             CALL ADSTR(NH,NS,NTS,MAXNS,MAXNH,BGUNIT,KTULTG,K2)
             IF(K2.EQ.1)WRITE(BGUNIT,'(''   CULT. CAP = '',F12.0,'' MULT
     1 = '',F12.0)')CMUC,SPECUL
             U=1
             IF(KTULTC.EQ.0)GO TO 1290
             GO TO 1500
          END IF
      END IF
C
C  CHECK FINAL STRESSES AT MID SPAN
C
      KTCL=0
      IF(SBF.GT.FT.AND.CHECK.EQ.0)THEN
             IF ( KKONT .EQ. 1 ) THEN
                 CALL RESET ( NS, NH, KKONT, K2, BGUNIT )               .A.
                 GO TO 1290
             END IF
          CALL ADSTR(NH,NS,NTS,MAXNS,MAXNH,BGUNIT,KTCL,K2)
          IF(K2.EQ.1)WRITE(BGUNIT,'(''   STRESS BOT = '',F9.4,''   ALLOW
     1.= '',F9.4)')
     2    SBF,FT
          U=0
          IF(KTCL.EQ.0) GO TO 1290
          GO TO 1500
      END IF
C
C  CHECK STRESSES AT HARPING PT. AT RELEASE
C
      IF( STH .GT. FTITOP .AND. DCGH(NH) .GE. 1. .AND. CHKCG .NE. 1.
     1    .AND. CHECK .EQ. 0 .OR. SBAH .LT. COMPI .AND. DCGH(NH)
     2    .GE. 1. .AND. CHKCG .NE. 1 .AND. CHECK .EQ. 0 ) THEN
             IF ( KKONT .EQ. 1 ) THEN
                 CALL RESET ( NS, NH, KKONT, K2, BGUNIT )               .A.
                 GO TO 1290
             END IF
           CGH=CGH+1.
           CRAISE=CRAISE+1.
           KCG=1
           GO TO 540
      END IF
C HARP MORE STRANDS TO REDUCE TOP OF GIRDER STRESSES IN NONCOMPOSITE
C GIRDERS AND BOTTOM OF GIRDER COMPRESSION FOR COMPOSITE GIRDERS
C DESIGNED CONTINUOUS FOR LIVE LOAD.
C
      IF ( KKONT .EQ. 2 .OR. CHECK .EQ. 1 ) GO TO 2050
      IF ( IBULBT .EQ. 0 .AND. L .EQ. 0 ) GO TO 2050
      IF ( IBULBT .EQ. 1 .AND. STEB .LE. 0.) GO TO 2050
      IF(K2.EQ.1)WRITE(BGUNIT,*)'  FINAL D/2 CONTINUOUS GIRDER '
      IF(K2.EQ.1)WRITE(BGUNIT,'(''   STRESS TOP = '',F9.4,''   ALLOW = '
     1',F9.4,/,''   STRESS BOT = '',F9.4,''   ALLOW.= '',F9.4)')
     2    STEB,FTTOP,SBEB,COMPA
          IF ( NS .GE. 6 .AND. NH .LE. MAXNH-2 .AND. NS .GE. NH+4 ) THEN.A.
              CALL HPMR(NH,NS,NTS,BGUNIT,MAXNH,KTD2A,K2)
              KKONT = 1
              GO TO 1290
          END IF
          GO TO 2050
C
C  WRITE FOR ERROR OUTPUTS
C
C
 1500 WRITE (OUNIT,2620) IDENT,NOPBM,NH,NS,RELAX
      IF (KCG.NE.0.AND.NOVER.LE.0.AND.CHECK.EQ.0)WRITE(OUNIT,2630)
      IF (KCG.EQ.0.AND.NOVER.GT.0.AND.CHECK.EQ.0)WRITE(OUNIT,2625)NOVER
      IF (KCG.NE.0.AND.NOVER.GT.0.AND.CHECK.EQ.0)WRITE(OUNIT,2635)NOVER
      IF (NOVER.GT.0.AND.CHECK.NE.0)WRITE(OUNIT,2645)
      WRITE (OUNIT,2640) XL,KSER,GIR,S,GTS,A3,ASP
      IF (CURB.NE.' NEW JERSEY') WRITE (OUNIT,2650) AS,USS,A,WC,DLL,AVFY
      IF (CURB.EQ.' NEW JERSEY') WRITE (OUNIT,2660) AS,USS,A,DLL,AVFY
      IF (ICOUNT.GT.100)WRITE(OUNIT,'(///,12X,''  PROBLEM DID NOT CONVER
     1GE'')')
      IF(KTD2A.EQ.1)WRITE(OUNIT,'(///,12X,''  FINAL STRESSES AT D/2 ARE
     1TOO HIGH'',/,12X,''  AND MORE STRANDS COULD NOT BE HARPED.'')')
      IF(KHDF.EQ.1)WRITE(OUNIT,'(///,12X,''  HARPED STRAND HOLD DOWN FOR.A.
     1CES ARE TOO HIGH'',/,12X,''  AND THE NUMBER OF HARPED STRANDS COUL.A.
     2D'',/,12X,''  NOT BE REDUCED.'')')                                .A.
      IF(KTD2B.EQ.1)WRITE(OUNIT,'(///,12X,''  FINAL STRESSES AT D/2 ARE
     1TOO HIGH'',/,12X,''  AND THE NUMBER OF HARPED STRANDS COULD'',/,12
     2X,''  NOT BE REDUCED.'')')
      IF (KTDA .EQ.1)WRITE(OUNIT,'(///,12X,''  STRESSES AT RELEASE AT D/
     12 ARE TOO'',/,12X,''  HIGH AND MORE STRANDS COULD'',/,12X,''  NOT
     2BE HARPED.'')')
      IF (KTDB .EQ.1)WRITE(OUNIT,'(///,12X,''  STRESSES AT RELEASE AT D/
     12 ARE '',/,12X,''  TOO HIGH AND THE NUMBER OF HARPED'',/,12X,''  S
     2TRANDS COULD NOT BE REDUCED.'')')
      IF (KTCL.EQ.1)WRITE(OUNIT,'(///,12X,''  STRESSES AT MID SPAN ARE T
     1OO HIGH'',/,12X,''  WITH THE MAXIMUM NUMBER OF STRANDS.'')')
      IF (KTULTG.EQ.1.AND.KUM.EQ.1)WRITE(OUNIT,'(///,12X,''  CONCRETE SE
     1CTION CANNOT HANDLE'',/,12X,''  THE ULTIMATE MOMENT.'')')
      IF (KTULTG.EQ.1.AND.KUM.EQ.0)WRITE(OUNIT,'(///,12X,''  MAXIMUM NUM
     1BER OF STRANDS'',/,12X,''  CANNOT HANDLE THE ULTIMATE MOMENT.'')')
      IF (KTULTC.EQ.1.AND.KUMC.EQ.1)WRITE(OUNIT,'(///,12X,''  COMPOSITE
     1SECTION CANNOT HANDLE'',//,12X,''  THE ULTIMATE MOMENT.'')')
      IF (KTULTC.EQ.1.AND.KUMC.EQ.0)WRITE(OUNIT,'(///,12X,''  MAXIMUM NU
     1MBER OF STRANDS'',//,12X,''  CANNOT HANDLE THE ULTIMATE MOMENT.'')
     2')
      WRITE(OUNIT,'(//,''        TERMINATING PROBLEM!!!'')')
C
C  START NEW PROBLEM
C
      GO TO 110
C
C
C  BEGIN FINAL OUTPUT IF ALL STRESSES BUT HARPING PT MET OR
C   IT IS A CHECK PROBLEM
C
 2050 WRITE (OUNIT,2620) IDENT,NOPBM,NH,NS,RELAX
      IF (KCG.NE.0.AND.NOVER.LE.0.AND.CHECK.EQ.0)WRITE(OUNIT,2630)
      IF (KCG.EQ.0.AND.NOVER.GT.0.AND.CHECK.EQ.0)WRITE(OUNIT,2625)NOVER
      IF (KCG.NE.0.AND.NOVER.GT.0.AND.CHECK.EQ.0)WRITE(OUNIT,2635)NOVER
      IF (NOVER.GT.0.AND.CHECK.NE.0)WRITE(OUNIT,2645)
      WRITE (OUNIT,2640) XL,KSER,GIR,S,GTS,A3,ASP
      IF (CURB.NE.' NEW JERSEY') WRITE (OUNIT,2650) AS,USS,A,WC,DLL,AVFY
      IF (CURB.EQ.' NEW JERSEY') WRITE (OUNIT,2660) AS,USS,A,DLL,AVFY
      WRITE (OUNIT,2680) SBF,FT
      IF(SBF.GT.FT)WRITE(OUNIT,'(''+'',48X,'' STRESSES OVER LIMITS !'')'
     1)
      WRITE (OUNIT,2700) STTOT,COMPA
      IF(STTOT.LT.COMPA)WRITE(OUNIT,'(''+'',48X,'' STRESSES OVER LIMITS
     1!'')')
      WRITE (OUNIT,2710) STEB,FTTOP
      IF(STEB.GT.FTTOP)WRITE(OUNIT,'(''+'',48X,'' STRESSES OVER LIMITS !
     1'')')
      WRITE (OUNIT,2720) STH,FTITOP
      IF(STH.GT.FTITOP)
     1     WRITE(OUNIT,'(''+'',48X,'' STRESSES OVER LIMITS !'')')
      WRITE (OUNIT,2730) SBAH,COMPI
      IF(SBAH.LT.COMPI)WRITE(OUNIT,'(''+'',48X,'' STRESSES OVER LIMITS !
     1'')')
      WRITE (OUNIT,2740) STEBI,FTITOP
      IF(STEBI.GT.FTITOP)
     1     WRITE(OUNIT,'(''+'',48X,'' STRESSES OVER LIMITS !'')')
      WRITE (OUNIT,2750) SBEBI,COMPI
      IF(SBEBI.LT.COMPI)WRITE(OUNIT,'(''+'',48X,'' STRESSES OVER LIMITS
     1!'')')
      WRITE (OUNIT,2760)
      IF ( FHOLD .GT. FHMX ) WRITE ( OUNIT, '(/,'' ===> WARNING !!!  HOL.C.
     1D DOWN FORCE = '',F5.1,'' KIPS.  SOME FABRICATORS CAN ONLY'',/,18X.C.
     2,'' HANDLE 45 KIPS OF HOLD DOWN FORCE !!!'')') FHOLD              .C.
      WRITE (OUNIT,2770) FICU,FCU
      IF (NH.EQ.0)THEN
         WRITE (OUNIT,2790) WT,WC,CGS(NS)
         WRITE (OUNIT,2800) B
      ELSE
         WRITE (OUNIT,2790) WT,WC,CGS(NS),CGH,DCGH(NH),FO,B
      END IF
      WRITE (OUNIT,2820) CSI,YBC,QSLAB
      WRITE (OUNIT,2825) SPECU,CMUG
      IF(SPECU.GT.CMUG)WRITE(OUNIT,'(''+'',38X,'' ULTIMATE STRENGTH EXCE
     1EDED !'')')
      IF(IBULBT.NE.1.AND.GTS.GT.0.)THEN
         WRITE (OUNIT,2830) SPECUL,CMUC
         IF(SPECUL.GT.CMUC)WRITE(OUNIT,'(''+'',38X,'' ULTIMATE STRENGTH
     1EXCEEDED !'')')
      END IF
      IF(U.EQ.1)WRITE(OUNIT,'(''+'',7X,''  ULTIMATE STRENGTH CONTROLS'')
     1')
      IF (DLL.EQ.0)THEN
         FCU=FCU*1000.
         GO TO 2415
      END IF
C
C    SHEAR CALCULATIONS
C
C    WEB DESIGNED TO RESIST ENTIRE SHEAR
C    DEAD LOAD SHEAR INCLUDES GIRDER, DIAPHRAGMS, & SLAB ONLY.
C    MMAX & VI ARE BASED ON LL+I AND ADDED DL (CURB & ASPHALT).
C    MCR IS THE CRACKING MOMENT @ THE BOTTOM EXCEPT FOR CONTINUOUS SPANS
C    THEN MCR @ D/2 & THE TAPER PT.=CRACKING MOMENT @ TOP OF GIRDER.
C                                    (SLAB STEEL NEGLECTED)
C    AR = XL*XNL IF GIVEN IN DATA
C    AR = XL*VI/MMAX = 5 EXCEPT FOR SPANS .LE. 65 FT. THEN  AR = 2.
C           IF NOT GIVEN IN DATA
C
      HT = YT + YB                                                      .C.
      FCU=1000.*FCU
      XI=50./(XL-XHALF+125.)+1.
      IF (XI .GT. 1.3) XI=1.3
      ULLDXI=0.5*2.17*XI*(S/DLL)
C
C     COMPUTE SHEAR AND STIRRUP SPACING AT D/2
C
      IF(K2.EQ.1)WRITE(BGUNIT,*)'  BEGINNING SHEAR CALCS AT D/2'
      FXL=XHALF/XL
      CALL LIVLOD(DN,DIAPH,XL,FXL,ULLDXI,WC,WG,WS,VI,TTT,
     1MMAX,VU,CLOAD,XXX)
      CLOC=' D/2:'
C INITIALIZE TEMPORARY VS VALUES
      VSTMP1=5000000.
      VSTMP2=5000000.
C END INITIALIZATION
C
C  FIND D FOR SIMPLE SPAN
C
      CGTT=HT-CGTEB
      XCGTT=0.8*HT
      IF (CGTT.LE.XCGTT) CGTT=XCGTT
      VD=(WG+WS)*(XL/2.0-XHALF)+0.50*DN*DIAPH
      IF (L.NE.0) THEN
C
C  FIND D AND VCI FOR CONT. ENDS
C
         FD=STEBG+STEBS
         FPE=P/AEB-P*(YBEB-CGTEB)/ZTEB
         MCR=EBI/(YB+YT-YBEB)*(0.006*SQRT(FCU)-FD+FPE)/12.
         CGTT=YB+YT+A-4.5
         IF ( IBULBT .EQ. 1 ) CGTT = .8 * ( YB + YT )
         AR=5.
         IF (XL.LT.65.) AR=2.
         IF (XNL.NE.0.) AR=XL*XNL
         VCI=0.0006*SQRT(FCU)*(EBT*12.)*CGTT+VD+AR*MCR/XL
      ELSE
C
C  FIND VCI FOR SIMPLE ENDS
C
         FD=SBEBG+SBEBS
         FPE=P/AEB+P*(YBEB-CGTEB)/ZBEB
         MCR=EBI/YBEB*(0.006*SQRT(FCU)-FD+FPE)/12.
         VCI=0.0006*SQRT(FCU)*(EBT*12.)*CGTT+VD+VI*MCR/MMAX
      END IF
C
C  CHECK FOR VCI MINIMUM
C
      VCIMIN=0.0017*SQRT(FCU)*(EBT*12.)*CGTT
      IF (K2.EQ.1)WRITE(BGUNIT,'(''  @ D/2 '',/
     1   ''  D ='',F10.4,''  VD ='',F10.4,/,
     2   ''  MCR ='',F10.1,''  VCI ='',F10.1)')CGTT,VD,MCR,VCI
      IF (K2.EQ.1)WRITE(BGUNIT,'(
     1   ''  FPE ='',F10.4,''  FD ='',F10.5,/,
     2   ''  VI ='',F10.4,''  MMAX ='',F10.5,/,
     3   ''  VCIMIN ='',F10.4)') FPE,FD,VI,MMAX,VCIMIN
      IF (VCI.LT.VCIMIN) VCI=VCIMIN
C
C  FIND VCW
C
      VSTH=XNH*AS*(USS*CSTR-FLOS)*(DCGH(NH)/(12*HARPL+0.792))
      FPC=P/AEB-P*(YBEB-CGTEB)*EDY/EBI-(STEBG+STEBS)*ZTEB*EDY/EBI
      VCW=(0.0035*SQRT(FCU)+0.3*FPC)*(EBT*12.)*CGTT+VSTH
      IF (K2.EQ.1)WRITE(BGUNIT,'(
     1   ''  VSTH ='',F10.4,''  FPC ='',F10.5,/,
     2   ''  VCW ='',F10.4)') VSTH,FPC,VCW
C
C  FIND VC AND VW
C
      VC=VCW
      IF (VCI.LT.VCW) VC=VCI
      VS= VU / 0.9 - VC
      VSL=0.008*SQRT(FCU)*12*EBT*CGTT
C
C  CHECK AGAINST MAXIMUM VS
C
      IF (VS.LE.VSL) THEN
C
C  FIND STIRRUP SPACING
C
         SSTIR=AVFY*CGTT/VS
         IF (VS.GT.0.5*VSL .AND.SSTIR.GT.12.) SSTIR=12.
         IF (SSTIR.GT.18.) SSTIR=18.
         IF (SSTIR.GT.0) THEN
            WRITE (OUNIT,2850) CLOC,VU,VC,VS,SSTIR,CLOAD
            VSSTD = 6.2 * CGTT                                          .C.
            IF( VS .GT. VSSTD ) WRITE ( OUNIT,2195 )                    .C.
            GO TO 2220
         END IF
         WRITE (OUNIT,2185) CLOC,VU,VC,CLOAD
 2185    FORMAT(7X,' SHEAR @',A11,' VU=',F5.1,' VC=',F5.1,
     1   ' USE MAX. SPACING OF 18"',A8,/,' ')                           .C.
         GO TO 2220
 2195    FORMAT('+',7X,'CHECK STANDARD STIRRUP SPACING AT GIRDER END ON
     1STANDARD SHEET.')
      ELSE
         WRITE (OUNIT,2840) CLOC,VU,VC,VS,VSL,CLOAD
         WRITE (OUNIT,3050)
      END IF
C
C     COMPUTE SHEAR AND STIRRUP SPACING AT END OF END BLOCK TAPER
C     OR D.
C
 2220 IF (KEB.EQ.0)THEN
          CLOC='   D:'
          FXL=HT/XL/12.
          XLEB=HT/12.
      ELSE
          CLOC=' EB TAPER:'
          FXL    = XLEB/XL
      END IF
      XI=50./(XL-XLEB+125.)+1.
      IF (XI .GT. 1.3) XI=1.3
      ULLDXI=0.5*2.17*XI*(S/DLL)
      CALL LIVLOD(DN,DIAPH,XL,FXL,ULLDXI,WC,WG,WS,VI,TTT,
     1MMAX,VU,CLOAD,XXX)
      VD     = (WG+WS)*(XL/2.0-XLEB)+0.50*DN*DIAPH
      TAPER  = 1.-(1.-XLEB/(XL/2.))**2.
      SBDGET = TAPER*(SBDG+SBDS)
C
C  FIND D FOR SIMPLE ENDS
C
      HHRTP  = CGH+DCGH(NH)-DCGH(NH)*(12.*XLEB)/(12.*XHARP)             .C.
      CGTT   = (HHRTP*(XHARP/HARPL)*XNH+CGS(NS)*XNS)/XNTS
      CGTT   = HT - CGTT
      CGTTS  = CGTT
      XCGTT  = 0.8*HT
      IF(CGTT.LE.XCGTT) CGTT = XCGTT
      IF (L .NE. 0) THEN
C
C  FIND D AND VCI FOR CONTINUOUS ENDS
C
         STDGET = -SBDGET*YT/YB
         FD=STDGET
         FPE=P/AG-P*((YB-(HT-CGTTS))/ZT)
         MCR    = GI/(YB+YT-YBEB)*(0.006*SQRT(FCU)-FD+FPE)/12.
         CGTT   = YB + YT + A - 4.5
         IF ( IBULBT .EQ. 1 ) CGTT = .8 * ( YB + YT )
         VCI    = 0.0006*SQRT(FCU)*A3*CGTT+VD+AR*MCR/XL
      ELSE
C
C  FIND VCI FOR SIMPLE ENDS
C
         FD = SBDGET
         FPE = P/AG+P*((YB-(HT-CGTTS))/ZB)
         MCR    = GI/YB*(0.006*SQRT(FCU)-FD+FPE)/12.
         VCI = 0.0006*SQRT(FCU)*A3*CGTT+VD+VI*MCR/MMAX
      END IF
C
C  CHECK AGAINST VCI MINIMUM
C
      VCIMIN = 0.0017*SQRT(FCU)*A3*CGTT
      IF (K2.EQ.1)WRITE(BGUNIT,'(''  @ '',A,/
     1   ''  D ='',F10.4,''  VD ='',F10.4,/,
     2   ''  MCR ='',F10.1,''  VCI ='',F10.1)')CLOC,CGTT,VD,MCR,VCI
      IF (K2.EQ.1)WRITE(BGUNIT,'(
     1   ''  FPE ='',F10.4,''  FD ='',F10.5,/,
     2   ''  VI ='',F10.4,''  MMAX ='',F10.5,/,
     3   ''  VCIMIN ='',F10.4)') FPE,FD,VI,MMAX,VCIMIN
      IF (VCI .LT. VCIMIN) VCI=VCIMIN
C
C  FIND VCW
C
      FPC = P/AG-P*(YB-(HT-CGTTS))*(YBC-YB)/GI+SBDGET*ZB*(YBC-YB)/GI
      VCW = (0.0035*SQRT(FCU)+0.3*FPC)*A3*CGTT+VSTH
      IF (K2.EQ.1)WRITE(BGUNIT,'(
     1   ''  VCW ='',F10.4,''  FPC ='',F10.5 )') VCW,FPC
C
C  FIND VC AND VS
C
      VC     = VCW
      IF (VCI .LT. VCW) VC = VCI
      VST    = VU / 0.9 - VC
C
C  CHECK AGAINST MAXIMUM VS
C
      VSLT   = 0.008*SQRT(FCU)*A3*CGTT
      IF (VST.LE.VSLT) THEN
         SSTIR=AVFY*CGTT/VST
C
C  LOAD VARIABLES FOR INTERPOLATION
C
         VSTMP1=VST
         DTMP1=CGTT
C
C  FIND STIRRUP SPACING
C
         IF (SSTIR .GT. 0.) THEN
            IF (VST .GT. 0.5*VSLT .AND. SSTIR .GT. 12.) SSTIR = 12.
            IF (SSTIR .GT. 18.) SSTIR = 18.
            S1=SSTIR
            WRITE (OUNIT,2860) CLOC,VU,VC,VST,SSTIR,CLOAD
            GO TO 2290
         END IF
         WRITE (OUNIT,2266) CLOC,VU,VC,CLOAD
 2266    FORMAT(7X,' SHEAR @',A11,' VU=',F5.1,' VC=',F5.1,
     1   ' USE MAX. SPA. (S) OF 18"',A8,/,' ')                          .C.
         S1=18.
      ELSE
         WRITE (OUNIT,2840) CLOC,VU,VC,VST,VSLT,CLOAD
         WRITE (OUNIT,3050)
      END IF
C
C     COMPUTE SHEAR AND STIRRUP SPACING AT HARPING POINT
C
 2290 CLOC=' HARP. PT.:'
      FXL=XHARP/XL
      XI=50./(XL-XHARP+125.)+1.
      IF (XI .GT. 1.3) XI=1.3
      ULLDXI=0.5*2.17*XI*(S/DLL)
      CALL LIVLOD(DN,DIAPH,XL,FXL,ULLDXI,WC,WG,WS,VI,TTT,
     1MMAX,VU,CLOAD,XXX)
C
C  FIND D
C
      CGTT=HT-YTS
      XCGTT=0.8*HT
      IF(CGTT.LE.XCGTT) CGTT=XCGTT
C
C  FIND VCI
C
      IF (XL.GE.100.0) DN = 0
      VD=(WG+WS)*(XL/2.0-XHARP)
      IF(DN.LT.2..AND.FXL.LT..35)VD=VD+0.5*DN*DIAPH
      SBDGSH=SBDGH+SBDSH
      FD=SBDGSH
      FPE=P/AG+P*((YB-YTS)/ZB)
      MCR=GI/YB*(0.006*SQRT(FCU)-FD+FPE)/12.
      VCI=0.0006*SQRT(FCU)*A3*CGTT+VD+VI*MCR/MMAX
C
C  CHECK AGAINST VCI MINIMUM
C
      VCIMIN=0.0017*SQRT(FCU)*A3*CGTT
      IF (K2.EQ.1)WRITE(BGUNIT,'(''  @ '',A,/
     1   ''  D ='',F10.4,''  VD ='',F10.4,/,
     2   ''  MCR ='',F10.1,''  VCI ='',F10.1)')CLOC,CGTT,VD,MCR,VCI
      IF (K2.EQ.1)WRITE(BGUNIT,'(
     1   ''  FPE ='',F10.4,''  FD ='',F10.5,/,
     2   ''  VI ='',F10.4,''  MMAX ='',F10.5,/,
     3   ''  VCIMIN ='',F10.4)') FPE,FD,VI,MMAX,VCIMIN
      IF (VCI.LT.VCIMIN) VCI=VCIMIN
C
C  FIND VCW
C
      FPC = P/AG-P*(YB-YTS)*(YBC-YB)/GI+SBDGSH*ZB*(YBC-YB)/GI
      VCW=(0.0035*SQRT(FCU)+0.3*FPC)*A3*CGTT
      IF (K2.EQ.1)WRITE(BGUNIT,'(
     1   ''  VCW ='',F10.4,''  FPC ='',F10.5 )') VCW,FPC
C
C  FIND VC AND VS
C
      VC=VCW
      IF (VCI.LT.VCW) VC=VCI
      VS= VU / 0.9 - VC
C
C  CHECK AGAINST VS MAXIMUM
C
      VSL=0.008*SQRT(FCU)*A3*CGTT
      IF (VS.GT.VSL) THEN
         WRITE (OUNIT,2840) CLOC,VU,VC,VS,VSL,CLOAD
         WRITE (OUNIT,3050)
         GO TO 2425
      END IF
C
C  FIND STIRRUP SPACING
C
      SSTIR=AVFY*CGTT/VS
C
C  LOAD VARIABLES FOR INTERPOLATION
C
      VSTMP2=VS
      DTMP2=CGTT
      IS=18
      IF (VS.GT.0.5*VSL) IS=12
      IF (SSTIR.GE.0.) THEN
         WRITE (OUNIT,2930) CLOC,VU,VC,VS,SSTIR,IS,CLOAD
      ELSE
         WRITE (OUNIT,2336) VU,VC,CLOAD
 2336    FORMAT(7X,' SHEAR @ HARP. PT: VU=',F5.1,' VC=',F5.1,
     1   ' USE MAX. SPACING OF 18"',A8,/,' ')                           .C.
      END IF
C
C  FIND POINT WHERE 18 INCH STIRRUP SPACING BEGINS
C
      IF(VSTMP1.NE.5000000..OR.VSTMP2.NE.5000000.) THEN
         IF(DTMP2.LT.DTMP1)DTMP1=DTMP2
         VS18=AVFY*DTMP1/18.
         XLS=(VSTMP1-VS18)/(VSTMP1-VSTMP2)*(XHARP-XLEB)+XLEB
         XLS1=XLS-2.
         IF(KEB.EQ.0.)XLS1=XLS-HT/12.
         XTMP=XL/2.
         IF(XLS.GT.XTMP)XLS=XTMP
         IF(S1.GE.17.9)WRITE(OUNIT,2411)
 2411    FORMAT(7X,' S = 18 INCHES OVER ENTIRE SPAN',/,' ')
 2412    FORMAT(7X,' BEGIN 18" STIRRUP SPA. ',F4.1,' FEET FROM CL BEARIN
     6G (LS =',F5.1,')',/,' ')
         IF(S1.LT.17.9)WRITE(OUNIT,2412)XLS,XLS1
         IF(S1.LT.14.)WRITE(OUNIT,2405)
 2405    FORMAT('+',7X,'INTERPOLATE FOR A MULTI-STEP INCREMENT IN STIRRU
     1P SPACING.',/,' ')
      ELSE
         WRITE (OUNIT,2414)
 2414    FORMAT(7X,' CANNOT COMPUTE LS OR S',/,' ')
      END IF
      GO TO 2425
 2415 WRITE(OUNIT,2990)
C
C  FINISH OUTPUT AND GO TO DFLECT OR PICKPT ROUTINES
C
 2425 WRITE (OUNIT,3010) GR1,GR2
      SBPRE=-SBPRE
      WRITE (OUNIT,3020)
      WRITE (OUNIT,3040) SBDG,SBDS,SBDC,SBXML,SBPRE
      FCU=FCU/1000.
      GO TO 4000
C
 2530 FORMAT(A1,//,'        WASHINGTON STATE DEPARTMENT OF TRANSPORTATIO
     1N ',A11,'  TIME ',A8)                                             .C.
 2600 FORMAT (//'        PROBLEM ',I3,' - TOO MANY STRANDS REQUIRED TO M
     1AKE PRESTRESS, OVER',I3//)
 2610 FORMAT(/'        IDENT: ',I4,'  PROBLEM',I4,I5,' HARPED STRANDS', .B.
     1  I5,' STRAIGHT STRANDS',/,'            MAX. NO. OF HARPED STRANDS.B.
     2 =',I5,/,                                                         .B.
     3 '            MAX. NO. OF HARPED STRANDS EXCEEDED.',/,            .B.
     4 '            EXECUTION TERMINATED')                              .B.
 2615 FORMAT(/'        IDENT: ',I4,'  PROBLEM',I4,I5,' HARPED STRANDS', .B.
     1  I5,' STRAIGHT STRANDS',/,                                       .B.
     2 '            MAX. NO. OF STRAIGHT STRANDS =',I5,/,               .B.
     3 '            MAX. NO. OF STRAIGHT STRANDS EXCEEDED.',/,          .B.
     4 '            EXECUTION TERMINATED')                              .B.
 2620 FORMAT(/'        IDENT: ',I4,'  PROBLEM',I4,I5,' HARPED STRANDS',
     1  I5,' STRAIGHT STRANDS',/,49X,A)
 2625 FORMAT('        HARPED STRANDS LOWERED @ D/2',I3,' INCHES',/,' ')
 2630 FORMAT('        HARPED STRANDS RAISED @ MID-SPAN',/,' ')
 2635 FORMAT('        HARPED STRANDS RAISED @ MID-SPAN AND LOWERED @ D/2
     1',I3,' INCHES',/,' ')
 2640 FORMAT('        SPAN',F7.2,7X,'TYPE',I3,F9.0,' GIRDERS AT ',      .C.
     1  F6.2,5X,F8.2,' SLAB'//'        WEB THICKNESS',F5.1,' IN.,'
     2  ,19X,F4.1,' IN. ASPHALT OVERLAY')
 2645 FORMAT(/'        RISE IN HARPED STRANDS IS LARGER THAN 7 TIMES THE.A.
     1 HARPING DISTANCE!')
 2650 FORMAT(/'        STRANDS',F6.3,' @',F5.0,' K   A =',F5.2,F7.1,
     1  ' K/FT CURB  S/',F4.1,'  AVFY =',F6.2)
 2660 FORMAT(/'        STRANDS',F6.3,' @',F5.0,' K   A= ',F5.2,3X,
     1  'N. J. BARRIER  S/',F4.1,'  AVFY =',F6.2)
 2680 FORMAT(/11X,' FINAL DESIGN STRESS, BOT. CENTERLINE',F7.3,' (ALLOWE
     1D',F7.3,')',/,' ')
 2700 FORMAT(11X,' FINAL DESIGN STRESS, TOP CENTERLINE ',F7.3,' (ALLOWED
     1',F7.3,')',/,' ')
 2710 FORMAT(11X,' FINAL DESIGN STRESS, TOP AT D/2',F12.3,' (ALLOWED',
     1F7.3,')',/,' ')
 2720 FORMAT(11X,' STRESS TOP AT HARP PT. AT RELEASE',F10.3,' (ALLOWED'
     1,F7.3,')',/,' ')
 2730 FORMAT(11X,' STRESS BOT. AT HARP POINT AT RELEASE',F7.3,' (ALLOWED
     1',F7.3,')',/,' ')
 2740 FORMAT(11X,' STRESS TOP AT D/2 AT RELEASE',F15.3,' (ALLOWED',F7.3
     1,')',/,' ')
 2750 FORMAT  (11X,' STRESS BOT AT D/2 AT RELEASE',F15.3,' (ALLOWED',
     1F7.3,')',/,' ')
 2760 FORMAT(19X,'(INCLUDES ELASTIC SHORTENING AND RELAXATION)')
 2770 FORMAT(/'        GIR. CONCRETE STRENGTH AT STRAND RELEASE=',F7.3,
     1  '  AT 28 DAYS=',F7.3)
 2780 FORMAT(//'        PROBLEM',I3,' - "A" DIMENSION IS LESS THAN SLAB
     1THICKNESS'//)
 2790 FORMAT(/'        WS(SLAB+DIAPH) =',F6.3,'  WC(CURB+ASPH) =',F6.3,
     1   '  E =',F6.3,'  FCL =',F6.3//
     2   '        RISE IN HARPED STRANDS =',F7.3,'     FO =',F7.3,
     3   '    FLG W = ',F6.2)
 2800 FORMAT(/,61X,'FLG W = ',F6.2)
 2820 FORMAT(/'        COMPOSITE SECTION I =',F9.0,'   Y BOT =',F6.2,
     1   '    Q SLAB =',F7.0)
 2825 FORMAT(/'        ULTIMATE MOMENT REQ''D.',F9.0,'  AVAILABLE',F9.0,
     1  ' ( GIRDER ONLY )')                                             .C.
 2830 FORMAT('        ULTIMATE MOMENT REQ''D.',F9.0,'  AVAILABLE',F9.0,
     1  ' ( COMPOSITE )',/,' ')
 2840 FORMAT(7X,' SHEAR @',A11,' VU=',F5.1,' VC=',F5.1,' VS=',F5.1,
     1' (MAX. VS =',F5.1,')',A8,/,' ')                                  .C.
 2850 FORMAT(7X,' SHEAR @',A11,' VU=',F5.1,' VC=',F5.1,' VS=',F5.1,
     1' SPA.=',F5.1,A8,/,' ')                                           .C.
 2860 FORMAT(7X,' SHEAR @',A11,' VU=',F5.1,' VC=',F5.1,' VS=',F5.1,
     1' SPA. (S)=',F5.1,A8,/,' ')                                       .C.
 2930 FORMAT(7X,' SHEAR @',A11,' VU=',F5.1,' VC=',F5.1,
     1   ' VS=',F5.1,' SPA.=',F5.1,'(',I2,'" MAX)',A8,/,' ')            .C.
 2990 FORMAT(7X,' CHECK ULTIMATE SHEAR AND STIRRUP SPACING MANUALLY',/,'
     1 ')
 3010 FORMAT(7X,' GIR. D.L. END REACTION=',F6.1,' WITHOUT END DIAPHRAGM'
     1,/,7X,' GIR. D.L. END REACTION=',F6.1,' WITHOUT CURB, ASPHALT, OR .C.
     2END DIAPHRAGM')
 3020 FORMAT(/20X,'BREAKDOWN OF STRESSES AT CENTERLINE SPAN')
 3030 FORMAT (A4,1X,I1,F4.3,F4.2,F5.1,F5.0,F7.0,F4.2,F5.1,F7.0,F4.2,A1,
     1        I2,I2,1X,2F4.2,2F2.0,A1,A1,I1)
 3040 FORMAT(/,7X,' SBDG=',F7.3,' SBDS=',F7.3,' SBDC=',F7.3,' SBXML=',
     1F7.3,' SBPRE=',F7.3)
 3050 FORMAT('+',7X,'WARNING,  VS REQUIRED EXCEEDS THE ALLOWABLE!!')
C
C
      END
C
C
C
C
C
C
C
C
      SUBROUTINE LIVLOD(DN,DIAPH,XL,FXL,ULLDXI,WC,WG,WS,VI,MLL,
     1MMAX,VU,CLOAD,XXX)
C
      REAL*4 MLL,MD,MMAX
      DIMENSION XMLL(4),XVLL(4)
      CHARACTER*32 CLOAD
      INTEGER*4 ERUNIT,OUNIT
      COMMON /PARMIN/ IRS,IDEF,IPICK,OUNIT,ERUNIT,INUNIT,TUNIT,K2
C
C    LIVE LOAD CALCULATIONS FOR MOMENT AND SHEAR
C
C    LANE LOADING  HS25
C
   10 XMLL(1)=((1.-(1.-2.*FXL)**2)*.10*XL**2+FXL*(1.-FXL)*22.5*XL)      .D.
     1       *ULLDXI
      XVLL(1)=((1.-FXL)*(0.40*XL+32.5))*ULLDXI                          .D.
C
C    HS25 TRUCK GOING FORWARD (TO THE RIGHT)
C
      XMLL(2)=(90.*(1.-FXL-14/(3.*XL))*FXL*XL-140)*ULLDXI               .D.
      XVLL(2)=(90.*(1.-FXL-14/(3*XL))-10.)*ULLDXI                       .D.
C
      IF (FXL*XL.GE.14.) GO TO 15
      XMLL(2)=0
      XVLL(2)=0
C
C    HS25 TRUCK GOING BACKWARDS (TO THE LEFT)
C
   15 XMLL(3)=90.*(1.-FXL-28/(3.*XL))*FXL*XL*ULLDXI                     .D.
      XVLL(3)=90.*(1.-FXL-28/(3.*XL))*ULLDXI                            .D.
C
C    MILITARY
C
      XMLL(4)=(24.*XL*FXL*((1.-FXL)+(1.-FXL-4./XL)))*ULLDXI
      XVLL(4)=(24.*(1.-FXL)+24.*(1.-FXL-4./XL))*ULLDXI
      IF(XXX.GT.0.)THEN
         XMLL(4)=0.
         XVLL(4)=0.
      END IF
C
C    FIND MAXIMUM MOMENT
C
      MLL=XMLL(1)
      ICTRL=1
      DO 50  I=2,4
         IF (MLL .GT. XMLL(I)) GO TO 50
         ICTRL=I
         MLL=XMLL(I)
         VLL=XVLL(I)
50    CONTINUE
C
C    FIND MAXIMUM SHEAR
C
      IF(CLOAD.NE.'FLEX')THEN
         VLM=XVLL(1)
         ICTRL=1
         DO 60  I=2,4
            IF (VLM .GT. XVLL(I)) GO TO 60
            ICTRL=I
            VLM=XVLL(I)
60       CONTINUE
      END IF
      CLOAD=' (HS)'
      IF (ICTRL.EQ.1) CLOAD=' (LANE)'
      IF (ICTRL.EQ.4) CLOAD=' (MILIT)'
C
C
C    COMPUTE ADDED ULTIMATE DEAD LOAD MOMENT AND SHEAR
C
      MD=1.3*(1.-(1-2.*FXL)**2)*.125*(WC)*XL**2
      VDL=1.3*(.5*XL*(1.-2.*FXL)*(WC))
      IF (XXX.GE..01)MLL=MLL*XXX/25.                                    .D.     
      MMAX=MLL+MD
      IF (XXX.GE..01)VLM=VLM*XXX/25.                                    .D.     
      IF (XXX.GE..01)VLL=VLL*XXX/25.                                    .D.
      VI=VLL+VDL
      VD=.5*XL*(1.-2.*FXL)*(WS+WC+WG)+.5*DN*DIAPH
      IF (FXL.GT.0.35.AND.DN.GE.2) VD=VD-0.5*DN*DIAPH
      VU=1.3*VD+VLM
C
   70 RETURN
      END
C
C
C
C
      SUBROUTINE ADSTR(NH,NS,NTS,MAXNS,MAXNH,BGUNIT,KTCL,K2)
      INTEGER BGUNIT
C
C      SUBROUTINE ADDS 1 STRAND TO THE GIRDER
C           (IN ONE CASE ADDS 2)
C
      IF(NH.EQ.0.AND.NS+2.LE.MAXNS)THEN
         NS=NS+2
         NTS=NTS+2
         IF(K2.EQ.1)WRITE(BGUNIT,*)' ADDING 2 STRAIGHT STRANDS'
      ELSE
         IF(NS+2.LE.MAXNS)THEN
            NS=NS+2
            NH=NH-1
            NTS=NTS+1
            IF(K2.EQ.1)WRITE(BGUNIT,*)' ADDING 2 STRAIGHT AND REMOVING 1
     1 HARPED STRAND'
         ELSE
            IF(NH+1.LE.MAXNH)THEN
               NH=NH+1
               NTS=NTS+1
               IF(K2.EQ.1)WRITE(BGUNIT,*)' ADDING 1 HARPED STRAND'
            ELSE
               KTCL=1
               IF(K2.EQ.1)WRITE(BGUNIT,*)' TRIED TO ADD A STRAND BUT COU
     1LD NOT'
            END IF
         END IF
      END IF
      RETURN
      END
C
C
C
C
C
C
C
      SUBROUTINE HPMR(NH,NS,NTS,BGUNIT,MAXNH,KTD2,K2)
      INTEGER BGUNIT
C
C       HARPS MORE STRANDS
C
      IF ( NS .LT. 6 .OR. NH + 2 .GT. MAXNH ) THEN
          IF ( NH + 1 .GT. MAXNH ) THEN
               KTD2=1
               IF(K2.EQ.1)WRITE(BGUNIT,*)' TRIED TO HARP MORE STRANDS BU
     1T COULD NOT'
          ELSE
               NH = NH + 1
               NTS = NTS + 1
               IF(K2.EQ.1)WRITE(BGUNIT,*)' ADDING 1 HARPED STRAND'
          END IF
      ELSE
          NH=NH+2
          NS=NS-2
          IF(K2.EQ.1)WRITE(BGUNIT,*)' HARPING 2 MORE STRANDS'
      END IF
      RETURN
      END
C
C
C
C
C
      SUBROUTINE HPLS(NH,NS,BGUNIT,MAXNS,KTD2,K2)
      INTEGER BGUNIT
C
C       HARPS 2 LESS STRANDS
C
      IF(NH.LT.2.OR.NS+2.GT.MAXNS)THEN
          KTD2=1
          IF(K2.EQ.1)WRITE(BGUNIT,*)' TRIED TO HARP 2 LESS STRANDS BUT C
     1OULD NOT'
      ELSE
          NH=NH-2
          NS=NS+2
          IF(K2.EQ.1)WRITE(BGUNIT,*)' HARPING 2 LESS STRANDS'
      END IF
      RETURN
      END
C
C
C
C
C
      SUBROUTINE RESET ( NS, NH, KKONT, K2, BGUNIT )
      INTEGER*4 BGUNIT
          KKONT = 2
          NS = NS + 2
          NH = NH - 2
          IF(K2.EQ.1)WRITE(BGUNIT,*)' OPTIONAL HARPED STRANDS CAUSED FUR
     1THER STRAND CHANGES, HARPING 2 LESS '
      RETURN
      END      
