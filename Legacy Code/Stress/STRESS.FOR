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

CC  PROGRAM FINDS SECTION PROPERTIES FOR COMPOSITE STEEL BOX GIRDERS.   A0000100
CC  
CC  REVISION LOG:
CC 
CC     .T. - TERRY BEEBE - VMS CONVERSION - JANUARY 1990                .T. 
CC
CC     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
CC                                                                      00000200
C  SET VARIABLE TYPES************************************************** A
       INTEGER*4 OUNIT, ERUNIT, IFLG, JFLG                              00000300
       CHARACTER*1 CARCNT                                               00000400
       CHARACTER*8 REVDAT, TIME                                         .T.00500
       CHARACTER*11 DATE                                                .T.
       CHARACTER*23 DATIME                                              .T.
C  SET UP REAL VARIABLE NAMES BEGINING WITH I-O.                        00000600
       REAL M,N1,N2,IFX,IFY,MP,MN,LW,LB,LT,IXSF,IXS,IXSL,IXRG,LR        00000700
       REAL YS(10)                                                      00000800
       REAL IXW,IXBF,IXTF,IRG,IXC1,IXC2,IYRG,IXBP,IYSL,IYBP,IS          00000900
       REAL IYW,IYBF,IYTF,IYSG,MDG,MDS,MDTB,MLI,IYC1,IYC2,IXSG          00001000
C                                                                       A
C THE VARIABLE ACCOUNT IS AS FOLLOWS: ********************************* A
C      THE VARIABLES INPUT ARE:                                         A
C           NP - PROBLEM NUMBER                                         A
C           D - WEB DEPTH                                               A
C           T - WEB THICKNESS                                           A
C           WT - TOP FLANGE WIDTH                                       A
C           TT - TOP FLANGE THICKNESS                                   A
C           WB - BOTTOM FLANGE WIDTH                                    A
C           TB - BOTTOM FLANGE THICKNESS                                A
C           WS - STAB WIDTH                                             A
C           TS - SLAB THICKNESS                                         A
C           LT - BOX GIRDER WIDTH @ TOP                                 A
C           LB - BOX GIRDER WIDTH @ BOTTOM                              A
C           G - SPACE - WEB TOP TO SLAB BOTTOM                          A
C           C - SPACE - WEB TOP TO REBAR                                A
C           AS - REBAR AREA                                             A
C           LR - REINFORCED SLAB WIDTH                                  A
C           F - DIST. - GIRDER BOTTOM TO STIFFENER CENTROID             A
C           AF - BOTTOM FLANGE STIFFENER AREA                           A
C           IFX - MOMENT OF INERTIA OF BOTTOM STIFFENER - X AXIS        A
C           IFY - MOMENT OF INERTIA OF BOTTOM STIFFENER - Y AXIS        A
C           N1 - MODULAR RATIO OF CONCRETE - FOR STRESS                 A
C           N2 - MODULAR RATIO OF CONCRETE - FOR DEFLECTION             A
C           K - OUTPUT CODE                                             A
C                                                                       A
C           MDG - DEAD LOAD MOMENT OF GIRDER                            A
C           MDS - DEAD LOAD MOMENT OF SLAB                              A
C           MDTB - SUPERIMPOSED DEAD LOAD MOMENT                        A
C           MLI - LIVE LOAD + IMPACT MOMENT                             A
C                                                                       A
C           YS(KN) - LOCATION OF STRESS CHECKS FROM BOTTOM FLANGE       A
C           MP - MOST POSITIVE MOMENT                                   A
C           MN - MOST NEGATIVE MOMENT                                   A
C                                                                       A
C     THE VARIABLES OUTPUT ARE:                                         A
C           ASG - STEEL GIRDER AREA                                     A
C           IXSG - MOMENT OF INERTIA OF THE STEEL GIRDER                A
C           IYSG - MOMENT OF INERTIA OF THE GIRDER ABOUT THE Y AXIS     A
C           YB - DIST. FROM BOTTOM TO GIRDER NA                         A
C           YT - DIST. FROM TOP TO GIRDER NA                            A
C           QBF - Q OF GIRDER BOTTOM FLANGE                             A
C           QTF - Q OF GIRDER TOP FLANGE                                A
C           DLS - DEAD LOAD OF STEEL GIRDER, PLF (WT.=490 PCF)          A
C                                                                       A
C           IXC1 - COMPOSITE MOMENT OF INERTIA (N1)                     A
C           IXC2 - COMPOSITE MOMENT OF INERTIA (N2)                     A
C           IYC1 -     "       "     "    "    ABOUT Y AXIS (N1)        A
C           IYC2 -     "       "     "    "    ABOUT Y AXIS (N2)        A
C           YBC1 - DIST. BOT. TO NA - COMPOSITE (N1)                    A
C           YBC2 - DIST. BOT. TO NA - COMPOSITE (N2)                    A
C           YTC1 -   "   TOP  "  "        "     (N1)                    A
C           YTC2 -   "   TOP  "  "        "     (N2)                    A
C           QBF1 - COMPOSITE Q OF BOTTOM FLANGE (N1)                    A
C           QBF2 -     "     "  "   "       "   (N2)                    A
C           QS1 - Q OF SLAB (N1)                                        A
C           QS2 - Q OF SLAB (N2)                                        A
C           QTS1 - Q OF SLAB + TOP FLANGE (N1)                          A
C           QTS2 - Q OF SLAB + TOP FLANGE (N2)                          A
C           IXRG - MOMENT OF INERTIA OF GIRDER + REINFORCEMENT          A
C           IYRG -    "   "     "    "    "    "       "       - Y AXIS A
C           YRB - DIST. FROM BOTTOM TO NA                               A
C           YRT - DIST. FROM TOP OF STEEL FLANGE TO NA                  A
C           YRS - DIST. FROM REBAR CENTROID TO NA                       A
C           Q9 - Q OF BOTTOM FLANGE, GIRDER + REBAR                     A
C           Q - Q OF TOP FLANGE, GIRDER + REBAR                         A
C                                                                       A
C           S1 - STRESS @ GIRDER TOP - FROM GIRDER DL                   A
C           S2 -    "   "   "    BOT. -  "    "    "                    A
C           S3 - STRESS @ GIRDER TOP - FROM SLAB DL                     A
C           S4 -    "   "   "    BOT. -  "   "   "                      A
C           S5 - STRESS @ GIRDER TOP - FROM SUPERIMPOSED DL             A
C           S6 -    "   "   "    BOT. -  "        "      "              A
C           S7 - STRESS @ GIRDER TOP - FROM LL + I                      A
C           S8 -    "   "   "    BOT. -  "   " " "                      A
C           S - SUM OF TOP STRESSES                                     A
C           S9 - SUM OF BOTTOM STRESSES                                 A
C                                                                       A
C           Z - CONCRETE/REBAR STRESS FROM SUPERIMPOSED LOADS           A
C           Z1 - REBAR STRESS FOR LL + I                                A
C           Z2 - Z + Z1 FOR REBAR STRESSES                              A
C           Z3 - CONCRETE STRESS FOR LL + I                             A
C           Z4 - Z + Z3 FOR CONCRETE STRESSES                           A
C                                                                       A
C           F1 - STRESS @ YS(KN) FOR MOST POSITIVE STRESS               A
C           F2 -   "    "   "     "   "   NEGATIVE   "                  A
C           F3 - SUM OF STRESSES @ YS(KN) FOR STRESS RANGE              A
C                                                                       A
C     INTERMEDIATE VARIABLES ARE NOT DEFINED HERE                       A
CC                                                                      00001100
CC                                                                      00001200
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC 00001300
C     OUNIT = (6 FOR FILE OUTPUT)                                       00001400
C     ERUNIT = (3 FOR PC)                                               00001500
C              (6 FOR MAINFRAME BATCH)                                  00001600
C     INUNIT = (5 FOR FILE INPUT)                                       00001700
C     CARCNT = ('1' FOR BATCH)                                          00001800
C              (' ' FOR INTERACTIVE)                                    00001900
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC 00002000
C                                                                       00002100
C                                                                       00002200
      CALL PARM(OUNIT,ERUNIT,CARCNT,INUNIT)
      CALL LIB$DATE_TIME (DATIME)                                       .T.02300
      TIME = DATIME(13:20)                                              .T.02400
      DATE = DATIME(1:11)                                               .T.02500
C                                                                       00002600
C                                                                       00002700
C                                                                       00002800
C                                                                       00002900
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC 00003200
C      MODIFICATIONS TO THE PROGRAM BY M. ANDERSON ARE NOTED            A0001200
C      BY THE CODE "A".                                                 A0000100
C      25 NOV 1986                                                      A0000100
C                                                                       A0000100
       REVDAT = ' 1/31/90'                                              .T.03300
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC 00003400
C                                                                       A
C  READ IN SECTION GEOMETRIES ***************************************** 00003500
C                                                                       A
10     READ(INUNIT,END=99,ERR=998,FMT=*) NP,D,T,WT,TT,WB,TB,WS,TS       00003600
       READ(INUNIT,ERR=998,END=998,FMT=*)LT,LB,G,C,AS,LR,F,AF,IFX,IFY   00003700
       READ(INUNIT,ERR=998,END=998,FMT=*)N1,N2,K                        00003800
C                                                                       A
C  WRITE PAGE HEADING ************************************************* 00003900
C                                                                       A
 1000 FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION  ',    .T.03000
     1  A11,'  TIME ',A8)                                               .T.03100
       WRITE(OUNIT,1000)CARCNT, DATE, TIME                              00004000
       WRITE(OUNIT,11)REVDAT                                            00004100
11     FORMAT(' ',/,'                               COMPOSITE STEEL GIRD00004200
     1ER',/,'                            SECTION PROPERTIES & STRESSES',00004300
     2/,'                            -----------------------------'     00004400
     3,/,8X,'REV ',A8,///)                                              00004500
C                                                                       A
C  COMPUTE IX,IY,CENTROID,WEIGHT,AND SHEAR FLOW OF STEEL GIRDER. ****** 00004600
C                                                                       A
       XDV=(LT-LB)/2.                                                   00004700
       LW=SQRT(D*D+XDV*XDV)                                             00004800
       ASG=LW*T+WT*TT+WB*TB+AF                                          00004900
       YCW=D/2.+TB                                                      00005000
       YCBF=TB/2.                                                       00005100
       YCTF=D+TB+TT/2.                                                  00005200
       YCSF=F+TB                                                        00005300
       YB=(LW*T*YCW+WT*TT*YCTF+WB*TB*YCBF+AF*YCSF)/ASG                  00005400
       IXW=D*D*T*LW/12.                                                 00005500
       IXBF=TB**3.*WB/12.                                               00005600
       IXTF=TT**3.*WT/12.                                               00005700
       IXW=IXW+LW*T*(ABS(YB-YCW))**2.                                   00005800
       IXBF=IXBF+WB*TB*(ABS(YB-YCBF))**2.                               00005900
       IXTF=IXTF+WT*TT*(ABS(YB-YCTF))**2.                               00006000
       IXSF=IFX+AF*(ABS(YB-YCSF))**2.                                   00006100
       IXSG=IXW+IXBF+IXTF+IXSF                                          00006200
       DLS=ASG/144.*490.                                                00006300
       IYW=XDV*XDV*T*LW/12.                                             00006400
       IYBF=WB**3.*TB/12.                                               00006500
       IF(LT.EQ.0.)THEN                                                 00006600
            IYTF=WT**3.*TT/12.                                          00006700
       ELSE                                                             00006800
            IYTF=(WT/2.)**3.*TT/6.                                      00006900
            IYTF=IYTF+WT*TT*LT*LT/4.                                    00007000
       END IF                                                           00007100
       IYW=IYW+LW*T*((XDV+LB)/2.)**2.                                   00007200
       IYSG=IYW+IYBF+IYTF+IFY                                           00007300
       QBF=WB*TB*(YB-TB/2)                                              00007400
       YT=D+TT+TB-YB                                                    00007500
       QTF=WT*TT*(YT-TT/2)                                              00007600
C                                                                       A
C   OUTPUT 1 HERE - STEEL GIRDER PROPERTIES *************************** A0007700
C                                                                       A
1      FORMAT(/,'      PROBLEM NO. ',I4,21X,'* GIRDER PROPERTIES',/,    00007800
     6'      ************************',13X,'*******************',/,     00007900
     6'      WEB',F10.2,' BY',F7.3,14X,'* AREA',F11.2,/,                00008000
     6'      TOP FL',F7.2,' BY',F7.3,14X,'* IX',F13.2,/,                00008100
     6'      BOT FL',F7.2,' BY',F7.3,14X,'* IY',F13.2,/,                00008200
     6'      SLAB',F9.2,' BY',F7.3,14X,'* Y BOT',F10.2,/,               00008300
     6'      STIFF A=',F5.2,' F=',F5.2,16X,'* Y TOP',F10.2,/,           00008400
     6'      STIFF IX=',F8.3,' IY=',F8.3,8X,'* Q B FL',F9.2,/,          00008500
     6'      REBAR A=',F5.2,' C=',F7.3,' G=',F5.2,6X,'* Q T FL',F9.2,/, 00008600
     643X,'* LB/FT',F10.2,//)                                           00008700
       WRITE(OUNIT,1)NP,D,T,ASG,WT,TT,IXSG,WB,TB,IYSG,WS,TS,            00008800
     6YB,AF,F,YT,IFX,IFY,QBF,AS,C,G,QTF,DLS                             00008900
C                                                                       A
C  COMPUTE PROPERTIES OF 2 COMPOSITE SECTIONS. ************************ 00009000
C                                                                       A
       ACS1=WS*TS/N1                                                    00009100
       ACB1=WT*(G-TT)/N1                                                00009200
       AC1=ASG+ACS1+ACB1                                                00009300
       YBC1=(ACS1*(TS/2+G+D+TB)+ACB1*((G-TT)/2+TT+D+TB)+ASG*YB)/AC1     00009400
       YTC1=D+TT+TB-YBC1                                                00009500
       IXS=IXSG+ASG*(ABS(YBC1-YB))**2.                                  00009600
       IXSL=TS**3.*WS/12./N1                                            00009700
       YSL=D+TB+G+TS/2.                                                 00009800
       IXSL=IXSL+WS*TS/N1*(ABS(YSL-YBC1))**2.                           00009900
       YBP=D+TB+TT+(G-TT)/2.                                            00010000
       IXBP=(ABS(G-TT))**3.*WT/N1/12.                                   00010100
       IF(G.LT.TT)IXBP=-IXBP                                            00010200
       IXBP=IXBP+WT/N1*(G-TT)*(ABS(YBP-YBC1))**2.                       00010300
       IXC1=IXS+IXSL+IXBP                                               00010400
       IF(LT.EQ.0)THEN                                                  00010500
           IYSL=TS/N1*WS**3./12.                                        00010600
           IYBP=(ABS(G-TT))/N1*WT**3./12.                               00010700
           IF(G.LT.TT)GOTO 998                                          A0010800
       ELSE                                                             00010900
           IYSL=TS/N1*(WS/2.)**3./6.+TS/N1*WS*LT*LT/4.                  00011000
           IYBP=(ABS(G-TT))/N1*(WT/2.)**3./6.+(ABS(G-TT))/N1*WT*LT*LT/4.A
           IF(G.LT.TT)GOTO 998                                          A0011200
       END IF                                                           00011300
       IYC1=IYSG+IYSL+IYBP                                              00011400
       QS1=ACS1*(YTC1-TT+G+TS/2.)+ACB1*(YTC1+(G-TT)/2.)                 00011500
       QTS1=QS1+WT*TT*(YTC1-TT/2.)                                      00011600
       QBF1=WB*TB*(YBC1-TB/2.)                                          00011700
       ACS2=WS*TS/N2                                                    00011800
       ACB2=WT*(G-TT)/N2                                                00011900
       AC2=ASG+ACS2+ACB2                                                00012000
       YBC2=(ACS2*(TS/2+G+D+TB)+ACB2*((G-TT)/2+TT+D+TB)+ASG*YB)/AC2     00012100
       YTC2=D+TT+TB-YBC2                                                00012200
       IXS=IXSG+ASG*(ABS(YBC2-YB))**2.                                  00012300
       IXSL=TS**3.*WS/12./N2                                            00012400
       YSL=D+TB+G+TS/2.                                                 00012500
       IXSL=IXSL+WS*TS/N2*(ABS(YSL-YBC2))**2.                           00012600
       YBP=D+TB+TT+(G-TT)/2.                                            00012700
       IXBP=(ABS(G-TT))**3.*WT/N2/12.                                   00012800
       IF(G.LT.TT)IXBP=-IXBP                                            00012900
       IXBP=IXBP+WT/N2*(G-TT)*(ABS(YBP-YBC2))**2.                       00013000
       IXC2=IXS+IXSL+IXBP                                               00013100
       IF(LT.EQ.0)THEN                                                  00013200
           IYSL=TS/N2*WS**3./12.                                        00013300
           IYBP=(ABS(G-TT))/N2*WT**3./12.                               00013400
       ELSE                                                             00013600
           IYSL=TS/N2*(WS/2.)**3./6.+TS/N2*WS*LT*LT/4.                  00013700
           IYBP=(ABS(G-TT))/N2*(WT/2.)**3./6.+(ABS(G-TT))/N2*WT*LT*LT/4.00013800
       END IF                                                           00014000
       IYC2=IYSG+IYSL+IYBP                                              00014100
       QS2=ACS2*(YTC2-TT+G+TS/2.)+ACB2*(YTC2+(G-TT)/2.)                 00014200
       QTS2=QS2+WT*TT*(YTC2-TT/2.)                                      00014300
       QBF2=WB*TB*(YBC2-TB/2.)                                          00014400
C                                                                       A
C  OUTPUT 2 HERE, COMPOSITE GIRDER PROPERTIES. ************************ 00014500
C                                                                       A
2      FORMAT('      COMPOSITE GIRDER PROPERTIES   N=',F4.1,            00014600
     6' * COMPOSITE GIRDER PROPERTIES   N=',F4.1,/,                     00014700
     6'      ***********************************************************00014800
     6****************',/,                                              00014900
     6'      IX CMP=',F12.2,18X,'* IX CMP=',F12.2,/,                    00015000
     6'      IY CMP=',F12.2,18X,'* IY CMP=',F12.2,/,                    00015100
     6'      Y BOT =',F12.2,18X,'* Y BOT =',F12.2,/,                    00015200
     6'      Y TOP =',F12.2,18X,'* Y TOP =',F12.2,/,                    00015300
     6'      Q B FL=',F12.2,18X,'* Q B FL=',F12.2,/,                    00015400
     6'      Q SLAB=',F12.2,18X,'* Q SLAB=',F12.2,/,                    00015500
     6'      Q SL+T FL=',F9.2,18X,'* Q SL+T FL=',F9.2,//)               00015600
       WRITE(OUNIT,2)N1,N2,IXC1,IXC2,IYC1,IYC2,YBC1,YBC2,               00015700
     6YTC1,YTC2,QBF1,QBF2,QS1,QS2,QTS1,QTS2                             00015800
C                                                                       A
C  COMPUTE PROPERTIES OF REINFORCED COMPOSITE SECTION. **************** 00015900
C                                                                       A
       YRT=YT                                                           A
       YRB=YB                                                           A
       IXRG=IXSG                                                        A
       IF(AS.NE.0.0) THEN                                               00016000
            A=ASG+AS                                                    00016100
            YRB=(AS*(C+D+TB)+ASG*YB)/A                                  00016200
            YRT=D+TT+TB-YRB                                             00016300
            YRS=YRT-TT+C                                                00016400
            BR=AS/LR                                                    00016500
            IS=BR**3.*LR/12.                                            00016600
            IXRG=IXSG+ASG*(ABS(YB-YRB))**2.+IS+AS*YRS**2.               00016700
            IF(LT.EQ.0.)THEN                                            00016800
                 IYRG=IYSG+BR*LR**3./12.                                00016900
            ELSE                                                        00017000
                 IYRG=IYSG+BR*(LR/2.)**3./6.+AS*LT*LT/4.                00017100
            END IF                                                      00017200
            Q9=WB*TB*(YRB-TB/2)                                         00017300
            Q=AS*YRS+WT*TT*(YRT-TT/2)                                   00017400
C                                                                       A
C  OUTPUT 3 HERE ****************************************************** 00017600
C  IF NO REBAR, THEN SKIP THIS OUTPUT ********************************* 00017500
C                                                                       A
3      FORMAT('      COMPOSITE GIRDER PROPERTIES   REBAR',/,            00017700
     6'      ***********************************',/,                    00017800
     6'      IX=',F12.2,3X,'IY=',F12.2,/,                               00017900
     6'      Y BOT=',F9.2,3X,'Y TOP=',F9.2,3X,'Y REBAR=',F6.2,/,        00018000
     6'      Q B FL=',F8.2,3X,'Q T FL=',F8.2,//)                        00018100
            WRITE(OUNIT,3)IXRG,IYRG,YRB,YRT,YRS,Q9,Q                    00018200
       END IF                                                           00018300
C                                                                       A
C  IF NO MOMENTS TO ANALYZE, STOP ************************************* A0018400
C                                                                       A
       IF(K.GT.0)GO TO 10                                               A0018500
C                                                                       A
C  READ IN MOMENTS. *************************************************** 00018600
C                                                                       A
       READ(INUNIT,END=998,ERR=998,FMT=*) MDG,MDS,MDTB,MLI              00018700
C                                                                       A
C  COMPUTE STRESSES AT TOP AND BOTTOM. ******************************** 00018800
C                                                                       A
       S1=(MDG*12*YT)/IXSG                                              00018900
       S2=-(MDG*12*YB)/IXSG                                             00019000
       S3=(MDS*12*YT)/IXSG                                              00019100
       S4=-(MDS*12*YB)/IXSG                                             00019200
C                                                                       A
C  IF TOP IN TENSION USE GIRDER OR REBAR SECTION. ********************* 00019300
C                                                                       A
       IF(MDTB.GE.0.)THEN                                               00019400
           S5=(MDTB*12*YTC2)/IXC2                                       00019500
           S6=-(MDTB*12*YBC2)/IXC2                                      00019600
       ELSE                                                             00019700
           IF(AS.EQ.0.0)THEN                                            A0019800
           WRITE(OUNIT,*)' WARNING!!  TENSION IN UNREINF. CONCRETE SLAB'A
           END IF                                                       A
           S5=(MDTB*12*YRT)/IXRG                                        00019900
           S6=-(MDTB*12*YRB)/IXRG                                       00020000
       END IF                                                           00020100
C                                                                       A
C  IF TOP IN TENSION USE GIRDER OR REBAR SECTION. ********************* 00020200
C                                                                       A
       IF(MLI.GE.0.)THEN                                                00020300
           S7=(MLI*12*YTC1)/IXC1                                        00020400
           S8=-(MLI*12*YBC1)/IXC1                                       00020500
       ELSE                                                             00020600
           IF(AS.EQ.0.0) THEN                                           A0020700
           WRITE(OUNIT,*)' WARNING!!  TENSION IN UNREINF. CONCRETE SLAB'A
           END IF                                                       A
           S7=(MLI*12*YRT)/IXRG                                         00020800
           S8=-(MLI*12*YRB)/IXRG                                        00020900
       END IF                                                           00021000
       S=S1+S3+S5+S7                                                    00021100
       S9=S2+S4+S6+S8                                                   00021200
C                                                                       A
C  OUTPUT 4 HERE GIRDER STRESSES. ************************************* 00021300
C                                                                       A
4      FORMAT('      GIRDER STRESSES FOR FACTORED LOADS',/,             00021400
     6'      **********************************',/,22X,'DL',8X,'DL',8X,'00021500
     6DL',8X,'LL',6X,'TOTAL',/,                                         00021600
     620X,'GIRDER',5X,'SLAB',4X,'T.B.(N2)',3X,'+I(N1)',4X,'STRESS',//,  A0021700
     6'      MOMENTS',3X,4F10.0,/,                                      00021800
     6'      TOP FLANGE',5F10.3,/,                                      00021900
     6'      BOT FLANGE',5F10.3)                                        00022000
       WRITE(OUNIT,4)MDG,MDS,MDTB,MLI,S1,S3,S5,S7,S,S2,S4,S6,S8,S9      00022100
C                                                                       A
C  COMPUTE CONCRETE OR REBAR STRESSES. ******************************** A
C                                                                       A
       JFLG=O                                                           A0022200
       IF((AS.NE.0).AND.(MDTB.LT.0.0).AND.(MLI.LT.0.0)) THEN            A0022300
            Z=(MDTB*12*YRS)/IXRG                                        A0022400
            Z1=(MLI*12*YRS)/IXRG                                        A0022500
            Z2=Z+Z1                                                     A0022600
C                                                                       A
C  OUTPUT 5 HERE IF THERE IS REBAR AND -M IN SLAB. ******************** 00023300
C                                                                       A
5      FORMAT('      REBAR STRESS',18X,3F10.3,//)                       A0023400
                 WRITE(OUNIT,5)Z,Z1,Z2                                  A0023500
       ENDIF                                                            A
       IF(WS.GT.0.0) THEN                                               A0022700
           IF((AS.EQ.0.0).AND.(MDTB.LT.0)) THEN                         A0022800
           WRITE(OUNIT,*)' WARNING!!  TENSION IN UNREINF. CONCRETE SLAB'A
           Z=9999999.999                                                A0022900
           JFLG=1                                                       A0023000
           ELSE                                                         A0023100
                IF(MDTB.GE.0.0) THEN                                    A0023200
                     Z=(MDTB*12*(YTC2+G+TS-TT))/(N2*IXC2)               A
                ELSE                                                    A0023400
                     Z=0.                                               A
                     JFLG=1                                             A
                ENDIF                                                   A0023500
           ENDIF                                                        A0023600
           IF((AS.EQ.0.0).AND.(MLI.LT.0)) THEN                          A0022800
           WRITE(OUNIT,*)' WARNING!!  TENSION IN UNREINF. CONCRETE SLAB'A
                Z3=9999999.999                                          A0023800
                JFLG=1                                                  A0000100
           ELSE                                                         A0000100
                IF(MLI.GE.0.0) THEN                                     A0023200
                     Z3=(MLI*12*(YTC1+G+TS-TT))/(N1*IXC1)               A
                ELSE                                                    A0023400
                     Z3=0.                                              A
                     JFLG=1                                             A
                ENDIF                                                   A0023500
                IF(JFLG.EQ.0) THEN                                      A0023900
                     Z4=Z+Z3                                            A
                ELSE                                                    A
                     Z4=9999999.999                                     A
                ENDIF                                                   A
           ENDIF                                                        A
C                                                                       A
C  OUTPUT 6 HERE CONCRETE STRESSES IF NO NEGATIVE MOMENT IN SLAB. ***** A0024000
C                                                                       A
6      FORMAT('      CONCRETE STRESS',15X,3F10.3,//)                    00024100
            WRITE(OUNIT,6)Z,Z3,Z4                                       00024200
      ENDIF                                                             A
C                                                                       A
C  READ IN FATIGUE STRESS PTS. AND MAX. POS. AND NEG. MOMENTS. ******** 00024400
C                                                                       A
       READ(INUNIT,ERR=998,END=998,FMT=*)(YS(KN),KN=1,7),MP,MN          00024500
C                                                                       A
C  OUTPUT 7 HERE, TITLES. ********************************************* 00024600
C                                                                       A
       IF(MP.EQ.MN) GOTO 10                                             A0024700
7      FORMAT('      FATIGUE STRESS RANGE',/,'      ********************00024700
     6',/,'      Y LOCATION    STRESS FOR    STRESS FOR      STRESS',/, 00024800
     6'      FROM BOT FL   M=',F8.1,4X,'M=',F8.1,6X,'RANGE',/)          00024900
       WRITE(OUNIT,7)MP,MN                                              00025000
C                                                                       A
C  DO LOOP TO COMPUTE AND WRITE FATIGUE STRESSES. ********************* 00025100
C                                                                       A
       IFLG = 0                                                         A
       DO 20 KN=1,7                                                     00025200
            IF(YS(KN).EQ.0)IFLG=IFLG+1                                  A
            IF(MP.GE.0.)THEN                                            00025300
                 F1=(MP*12*(YS(KN)-YBC1))/IXC1                          00025400
            ELSE                                                        00025500
                 F1=(MP*12.*(YS(KN)-YRB))/IXRG                          00025600
            ENDIF                                                       00025700
            IF(MN.GE.0.)THEN                                            00025800
                 F2=(MN*12*(YS(KN)-YBC1))/IXC1                          00025900
            ELSE                                                        00026000
                 F2=(MN*12*(YS(KN)-YRB))/IXRG                           00026100
            ENDIF                                                       00026200
            F3=ABS(F1-F2)                                               00026300
C                                                                       A
C  OUTPUT 8 HERE ****************************************************** 00026400
C                                                                       A
8           FORMAT(6X,F8.3,4X,F8.3,5X,F9.3,5X,F10.3)                    00026500
            IF((YS(KN).EQ.0).AND.(IFLG.GE.2))GOTO 20                    A
            WRITE(OUNIT,8)YS(KN),F1,F2,F3                               A0026600
20     CONTINUE                                                         A
       GOTO 10                                                          A
998    WRITE(*,*)' INPUT ERROR!!  CHECK YOUR INPUT FILE!!'              .T.27100
       WRITE(OUNIT,*)' INPUT ERROR!!  CHECK YOUR INPUT FILE!!'          A0027100
99     STOP                                                             00027200
       END                                                              00027300
C                                                                       00027400
C                                                                       00027500
C                                                                       00027600
C                                                                       00027700
C                                                                       00027800
C                                                                       00027900
C                                                                       00028000
C                                                                       00028100
      SUBROUTINE PARM(OUNIT,ERUNIT,CARCNT,INUNIT)                       00028200
      CHARACTER*16 FLNM1, FLNM2                                         00028300
      CHARACTER*1 CARCNT                                                00028400
      INTEGER OUNIT, ERUNIT                                             00028500
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   00028600
C  STANDARD INTERACTIVE INPUT UNIT                                      00028700
      IIUNIT=1                                                          00028800
C  STANDARD INTERACTIVE OUTPUT UNIT                                     00028900
      IOUNIT=3                                                          00029000
C  STANDARD REPORT OUTPUT UNIT                                          00029100
      OUNIT=6                                                           00029200
C  STANDARD FILE INPUT UNIT                                             00029300
      INUNIT=5                                                          00029400
C  STANDARD ERROR OUTPUT UNIT                                           00029500
      ERUNIT=3                                                          00029600
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   00029700
C     OPEN(IIUNIT,FILE='CON')                                           .T.29800
C     OPEN(IOUNIT,FILE='CON')                                           .T.29900
10    WRITE(*,210)                                                      .T.30000
210   FORMAT(//,                                                        00030100
     1' Enter input file name:',/)                                      .T.30200
           READ(*,ERR=10,FMT='(A)')FLNM1                                .T.30300
           IF(FLNM1.EQ.'CON')GO TO 25                                   .T.30400
           IF(FLNM1.EQ.'con')GO TO 25                                   .T.30500
              DO 15 I=1,16                                              00030600
               IF(FLNM1(I:I).EQ.'.')GO TO 25                            00030700
15             CONTINUE                                                 00030800
              DO 20 I=1,16                                              00030900
                 IF(FLNM1(I:I).EQ.' ')THEN                              00031000
                  IE = I+3                                              00031100
                  FLNM1(I:IE) = '.dat'                                  00031200
                  GO TO 25                                              00031300
                 END IF                                                 00031400
20             CONTINUE                                                 00031500
25         WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1      .T.31600
          CARCNT='1'                                                    00031700
35    WRITE(*,235)                                                      .T.31800
235   FORMAT(//,                                                        00031900
     1' Enter output file name:',/)                                     .T.32000
           READ(*,ERR=35,FMT='(A)')FLNM2                                .T.32100
           IF(FLNM2.EQ.'CON')GO TO 60                                   .T.32200
           IF(FLNM2.EQ.'con')GO TO 60                                   .T.32300
              DO 40 I=1,16                                              00032400
               IF(FLNM2(I:I).EQ.'.')GO TO 60                            00032500
40             CONTINUE                                                 00032600
              DO 50 I=1,16                                              00032700
                 IF(FLNM2(I:I).EQ.' ')THEN                              00032800
                  IE = I+3                                              00032900
                  FLNM2(I:IE) = '.out'                                  00033000
                  GO TO 60                                              00033100
                 END IF                                                 00033200
50             CONTINUE                                                 00033300
60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2    .T.33400
          OPEN(OUNIT,FILE=FLNM2)                                        00033500
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')                          00033600
      IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5                      00033700
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6                      00033800
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6                      00033900
      RETURN                                                            00034000
      END                                                               00034100
C                                                                       00034200
C                                                                       00034300
C                                                                       00034400
