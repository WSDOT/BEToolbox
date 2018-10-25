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


CC  THIS PROGRAM USES FREE FORMAT INPUT OF BDS CARD 200 WITH            00000100
CC  MODIFICATIONS TO OBTAIN THE TORSIONAL CONSTANT FOR                  00000200
CC  CONCRETE BOX GIRDER BRIDGES AND ALSO SETS UP INPUT FOR              00000300
CC  BDS FOR OTHER SECTION PROPERTIES. INCLUDES TRAFFIC BARRIER.         00000400
CC                                                                      00000500
CC  ALSO FINDS TORSIONAL CONSTANT FOR PRESTRESSED GIRDER                00000600
CC  BRIDGES INCLUDING STANDARD TRAFFIC BARRIERS.                        00000700
CC
CC  REVISION LOG:                                                       .T.
CC                                                                      .T.
CC  .T. - TERRY BEEBE - VMS CONVERSION - JANUARY 1990                   .T.
CC
CC  RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
CC  Robert S. Lotze - Win32 dll conversion  1/00
CC                                                                      .T. 

C Note from Robert: All lines saying "Goto 666" were added for Win32 dll conversion.
C  These lines were originally "STOP" lines, but since "STOP" does not do a clean
C  shut-down of files, I changed all these to go to lines that closed the files, then
C  exited gracefully from the program with an "End". 

C The following 10 lines added by Robert for Win32 dll conversion:
C Robert's setup for a DLL call:
C Fortran Code  establishing subroutine
	SUBROUTINE TORSION (MYINPUT, MYOUTPUT, BDSFILENAME)

C Specify that the routine name is to be made available to callers of the
C DLL and that the external name should not have any prefix or suffix
!DEC$ ATTRIBUTES STDCALL,DLLEXPORT :: TORSION
!DEC$ ATTRIBUTES ALIAS:'TORSION' :: TORSION
C
C
C The following 6 lines added by Robert for Win32 dll conversion:
CInput argument	
	CHARACTER*254,INTENT(IN) :: MYINPUT
CInput argument
	CHARACTER*254,INTENT(IN) :: MYOUTPUT
CPath and filename for BDS file:
	CHARACTER*254,INTENT(IN) :: BDSFILENAME
!DEC$ ATTRIBUTES REFERENCE :: MYINPUT
!DEC$ ATTRIBUTES REFERENCE :: MYOUTPUT
!DEC$ ATTRIBUTES REFERENCE :: BDSFILENAME
C                                     
C
C
      CHARACTER*8 ACCT                                                  00000800
      CHARACTER*1 BDS,SST,CARCNT                                        00000900
      REAL*8 XLT(20), XLB(20), ABOX(20), Q(20), VEC(20), QDIAG(20)      00001000
      REAL*8 WORK(20),WORK2(20)                                         00001100
C	"INUNIT" added by Robert to make sure that it is declared
      INTEGER*4 OUNIT, BDUNIT, ERUNIT, INUNIT
      CHARACTER*8 REVDAT, TIME                                          .T.
      CHARACTER*11 DATE                                                 .T.
      CHARACTER*23 DATIME                                               .T.
CC                                                                      00001400
CC                                                                      00001500
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                      00001600
C     OUNIT = (6 FOR FILE OUTPUT)                                       00001700
C     ERUNIT = (3 FOR PC)                                               00001800
C              (6 FOR MAINFRAME BATCH)                                  00001900
C     INUNIT = (5 FOR FILE INPUT)                                       00002000
C     CARCNT = ('1' FOR BATCH)                                          00002100
C              (' ' FOR INTERACTIVE)                                    00002200
C     BDUNIT = 12                                                       00002300
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                      00002400
C       
C	Last two parameters added by Robert Lotze for win32 dll conversion:
      CALL PARM(OUNIT,ERUNIT,CARCNT,INUNIT,MYINPUT,MYOUTPUT)                             
C     CALL LIB$DATE_TIME (DATIME)                                       .T.
	CALL DATE_AND_TIME (DATIME)
      TIME = DATIME(13:20)                                              .T.
      DATE = DATIME(1:11)                                               .T. 
C                                                                       00003000
C                                                                       00003100
C                                                                       00003200
C                                                                       00003300
C                                                                       00003400
 1000 FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION  ',    .T.
     1  A11,' TIME ',A8)                                                .T.
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC  00003700
      REVDAT = ' 1/31/90'                                               .T.03800
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC  00003900
 1001 FORMAT('        ***SUPERSTRUCTURE TORSIONAL MOMENTS OF INERTIA ***00004000
     1' ,/,8X,'REV ',A8,///)                                            00004100
      CTBAR=1.16                                                        00004200
      YIBAR=.291                                                        00004300
      ABAR=2.767                                                        00004400
10    READ(INUNIT,END=999,ERR=998,FMT='(A)')SST                         00004500
      WRITE(OUNIT,1000)CARCNT, DATE, TIME                               00004600
      WRITE (OUNIT,1001)REVDAT                                          00004700
      IF(SST.EQ.'b')SST = 'B'                                           00004800
      IF(SST.EQ.'B')THEN                                                00004900
      READ(INUNIT,ERR=998,END=998,FMT='(A)')BDS                         00005000
      IF(BDS.EQ.'y')BDS = 'Y'                                           00005100
      IF(BDS.EQ.'Y')THEN                                                00005200
C
C	Last parameter added by Robert Lotze for win32 conversion:
      CALL BDFILE(BDUNIT,BDSFILENAME)
      END IF                                                            00005400
C                                                                       00005500
C                                                                       00005600
C                                                                       00005700
C                                                                       00005800
C                                                                       00005900
      READ(INUNIT,ERR=998,END=998,FMT=*)WEDED,D,TT,TB,NINT,TWI,         00006000
     6         TWL,FL,TWR,FR,                                           00006100
     6         CLL,TCLE,TCLI,                                           00006200
     6         CLR,TCRE,TCRI                                            00006300
      WRITE(OUNIT,*)'       CONC. BOX GIRDER SUPERSTRUCTURE.'           00006400
      WRITE(OUNIT,300)WEDED,D,TT,TB,NINT,TWI,                           00006500
     6            TWL,FL,TWR,FR,                                        00006600
     6            CLL,TCLE,TCLI,                                        00006700
     6            CLR,TCRE,TCRI                                         00006800
      IF(BDS.NE.'Y')GO TO 3                                             00006900
      READ(INUNIT,ERR=998,END=998,FMT='(A)')ACCT                        00007000
      L0=2                                                              00007100
      SST='H'                                                           00007200
      L1=10                                                             00007300
      L2=100                                                            00007400
      K0=1                                                              00007500
      K1=10.*WEDED                                                      00007600
      K2=100.*D                                                         00007700
      K3=100.*TT                                                        00007800
      K4=100.*TB                                                        00007900
      K5=TWI                                                            00008000
      K6=1                                                              00008100
      IF(FL.EQ.0.)K6=0                                                  00008200
      K7=TWL                                                            00008300
      K8=100.*FL                                                        00008400
      K9=1                                                              00008500
      IF(FR.EQ.0.)K9=0                                                  00008600
      K10=TWR                                                           00008700
      K11=100.*FR                                                       00008800
      K12=10.*CLL                                                       00008900
      K13=TCLE                                                          00009000
      K14=TCLI                                                          00009100
      K15=10*CLR                                                        00009200
      K16=TCRE                                                          00009300
      K17=TCRI                                                          00009400
      K18=200                                                           00009500
      WRITE(BDUNIT,500)ACCT,K0,K0,L0,SST,L1,L2                          00009600
      WRITE(BDUNIT,501)ACCT,K0,K1,K2,                                   00009700
     6K3,K4,NINT,K5,                                                    00009800
     6K6,K7,K8,K9,K10,                                                  00009900
     6K11,K12,K13,K14,K15,                                              00010000
     6K16,K17,K18                                                       00010100
      WRITE(OUNIT,301)                                                  00010200
3     NC=NINT+1                                                         00010300
      TWI=TWI/12.                                                       00010400
      TT=TT/12.                                                         00010500
      TB=TB/12.                                                         00010600
      TCLE=TCLE/12.                                                     00010700
      TCLI=TCLI/12.                                                     00010800
      TCRE=TCRE/12.                                                     00010900
      TCRI=TCRI/12.                                                     00011000
      TWL=TWL/12.                                                       00011100
      TWR=TWR/12.                                                       00011200
CC  CHECK FOR SINGE OR MULTI-CELL                                       00011300
CC                                                                      00011400
      IF(NC.NE.1)GO TO 5                                                00011500
CC                                                                      00011600
CC  FIND BIX FOR SINGLE BOX (WITHOUT CANTS)                             00011700
CC                                                                      00011800
      DT=D-TT/2.-TB/2.                                                  00011900
      XWL=TWL/(2.*COS(ATAN(FL/DT)))                                     00012000
      XWR=TWR/(2.*COS(ATAN(FR/DT)))                                     00012100
      XLTP=WEDED-CLL-CLR-XWL-XWR                                        00012200
      XLBT=XLTP-FL-FR                                                   00012300
      XL=SQRT(DT*DT+FL*FL)                                              00012400
      XR=SQRT(DT*DT+FR*FR)                                              00012500
      A=DT*(XLTP+XLBT)/2.                                               00012600
      DSTS=XLTP/TT+XLBT/TB+XL/TWL+XR/TWR                                00012700
      BIX=4.*A*A/DSTS                                                   00012800
      TOTBOX=BIX+CTBAR                                                  00012900
      GO TO 200                                                         00013000
CC                                                                      00013100
CC  READ EXTRA INFORMATION FOR MULTI-CELL BOXES                         00013200
CC                                                                      00013300
5     READ(INUNIT,ERR=998,END=998,FMT=*)SE,(XLT(I),I=1,NC)              00013400
CC                                                                      00013500
CC  FIND BIX FOR MULTICELL BOX (WITHOUT CANTALEVERS)                    00013600
CC                                                                      00013700
CC  FIND L/T AND ABOX FOR 1ST (LEFT) CELL. NC=1                         00013800
CC                                                                      00013900
      DT=D-TT/2.-TB/2.                                                  00014000
      THETA=ATAN(SE)                                                    00014100
      DELSE=SE*DT                                                       00014200
      XWB=DT/COS(THETA)                                                 00014300
      XLB(1)=XLT(1)-FL+DELSE                                            00014400
      XL=SQRT(DT*DT+FL*FL)                                              00014500
      ABOX(1)=(XLT(1)+XLB(1))*DT/2.                                     00014600
      VEC(1)=ABOX(1)*2.                                                 00014700
      WLOT=-XWB/TWI                                                     00014800
      QDIAG(1)=XL/TWL+XLT(1)/TT+XLB(1)/TB+XWB/TWI                       00014900
CC                                                                      00015000
CC  FIND L/T AND ABOX FOR LAST (RIGHT) CELL. NC=NC                      00015100
CC                                                                      00015200
      XLB(NC)=XLT(NC)-FR-DELSE                                          00015300
      XR=SQRT(DT*DT+FR*FR)                                              00015400
      QDIAG(NC)=XR/TWR+XLT(NC)/TT+XLB(NC)/TB+XWB/TWI                    00015500
      ABOX(NC)=(XLT(NC)+XLB(NC))*DT/2.                                  00015600
      VEC(NC)=ABOX(NC)*2.                                               00015700
CC                                                                      00015800
CC  FIND L/T AND ABOX FOR INTERIOR CELLS                                00015900
CC                                                                      00016000
      IF(NC.LE.2)GO TO 80                                               00016100
      DO 30 I=2,NC-1                                                    00016200
      QDIAG(I)=XLT(I)/TT+XLT(I)/TB+2.*XWB/TWI                           00016300
      ABOX(I)=XLT(I)*DT                                                 00016400
30    VEC(I)=2.*ABOX(I)                                                 00016500
C                                                                       00016600
C     ******************************************************************00016700
C                                                                       00016800
C     ROUTINE THOMAS                                                    00016900
C                                                                       00017000
C     ROUTINE SOLVES TRIDIAGONAL MATRIX WITH                            00017100
C     ALL OFF DIAGONAL MEMBERS BEING THE SAME                           00017200
C                                                                       00017300
80    WORK(1)=WLOT/QDIAG(1)                                             00017400
      WORK2(1)=VEC(1)/QDIAG(1)                                          00017500
      DO 90 I=2,NC                                                      00017600
         WORK(I)=WLOT/(QDIAG(I)-WORK(I-1)*WLOT)                         00017700
         WORK2(I)=(WORK(I)*(VEC(I)-WLOT*WORK2(I-1)))/WLOT               00017800
90    CONTINUE                                                          00017900
      Q(NC)=WORK2(NC)                                                   00018000
      DO 95 I=NC-1,1,-1                                                 00018100
         Q(I)=WORK2(I)-WORK(I)*Q(I+1)                                   00018200
95    CONTINUE                                                          00018300
      BIX=0.                                                            00018400
      DO 100 I=1,NC                                                     00018500
      BIX=BIX+2.*Q(I)*ABOX(I)                                           00018600
100   CONTINUE                                                          00018700
CC                                                                      00018800
CC  FIND BCL AND BCE FOR CANTALEVERS                                    00018900
CC                                                                      00019000
CC  BCL                                                                 00019100
CC                                                                      00019200
      DELTH=(TCLI-TCLE)/CLL                                             00019300
      FINT=TCLE**3.*CLL+3.*TCLE*TCLE*DELTH*CLL*CLL/2.+                  00019400
     1     TCLE*DELTH*DELTH*CLL**3.+DELTH**3.*CLL**4./4.                00019500
      ACL=(TCLI+TCLE)/2.*CLL                                            00019600
      BCL=.3333*FINT/(1.+1.3333*FINT/ACL/CLL/CLL)                       00019700
      IF(BCL.LT.0.)BCL=-BCL                                             00019800
CC                                                                      00019900
CC  BCR                                                                 00020000
CC                                                                      00020100
      DELTH=(TCRI-TCRE)/CLR                                             00020200
      FINT=TCRE**3.*CLR+3.*TCRE*TCRE*DELTH*CLR*CLR/2.+                  00020300
     1     TCRE*DELTH*DELTH*CLR**3.+DELTH**3.*CLR**4./4.                00020400
      ACR=(TCRI+TCRE)/2.*CLR                                            00020500
      BCR=.3333*FINT/(1.+1.3333*FINT/ACR/CLR/CLR)                       00020600
      IF(BCR.LT.0.)BCR=-BCR                                             00020700
      TOTBOX=BIX+BCL+BCR+CTBAR                                          00020800
      ELSE                                                              00020900
CC                                                                      00021000
CC  COMPUTE VALUES FOR PSG BRIDGE                                       00021100
CC                                                                      00021200
CC  PRESTRESSED GIRDER PROGRAM BEGINS HERE.                             00021300
CC                                                                      00021400
      READ(INUNIT,ERR=998,END=998,FMT=*)KSER,NGIR,WEDED,TT,FCSB,FCGD,   00021500
     1                                                       GSPAC      00021600
      WRITE(OUNIT,*)'       PRESTRESSED GIRDER SUPERSTRUCTURE.'         00021700
      WRITE(OUNIT,303)KSER,NGIR,WEDED,TT,FCSB,FCGD,GSPAC                00021800
      IF(KSER.LT.4.OR.KSER.GT.14)GO TO 97                               00021900
CC          1  2  3  4   5  6   7  8   9  10  11 12  13 14              00022000
      GO TO(97,97,97,105,97,110,97,120,97,130,97,140,97,150),KSER       00022100
105   TCG=.0951                                                         00022200
      YIG=.133                                                          00022300
      YAG=1.759                                                         00022400
      GO TO 160                                                         00022500
110   TCG=.1432                                                         00022600
      YIG=.216                                                          00022700
      YAG=2.308                                                         00022800
      GO TO 160                                                         00022900
120   TCG=.2860                                                         00023000
      YIG=.559                                                          00023100
      YAG=3.309                                                         00023200
      GO TO 160                                                         00023300
130   TCG=.3198                                                         00023400
      YIG=.720                                                          00023500
      YAG=3.795                                                         00023600
      GO TO 160                                                         00023700
140   TCG=.3249                                                         00023800
      YIG=.818                                                          00023900
      YAG=4.349                                                         00024000
      GO TO 160                                                         00024100
150   TCG=.3316                                                         00024200
      YIG=1.522                                                         00024300
      YAG=4.682                                                         00024400
160   TT=TT/12.                                                         00024500
      TCSLAB=WEDED*TT**3./3.                                            00024600
      COMPN=SQRT(FCGD)/SQRT(FCSB)                                       00024700
      TOTBOX=NGIR*TCG*COMPN+TCSLAB+CTBAR                                00024800
CC  FIND LATERAL MOMENT OF INERTIA FOR A PSG SUPER                      00024900
      YISLAB=WEDED**3*TT/12.                                            00025000
      YIBAR=2*(YIBAR+ABAR*(WEDED/2.+.0763)**2)                          00025100
      IKEY=NGIR/2                                                       00025200
      RKEY1=IKEY                                                        00025300
      RKEY=NGIR/2.                                                      00025400
      SUM=0.0                                                           00025500
         DO 170 NXX=1,IKEY                                              00025600
         IF(RKEY1.EQ.RKEY)THEN                                          00025700
         SUM=SUM+((2*NXX-1)*GSPAC/2.)**2.*YAG                           00025800
         ELSE                                                           00025900
         SUM=SUM+(NXX*GSPAC)**2.*YAG                                    00026000
         END IF                                                         00026100
170      CONTINUE                                                       00026200
      YIGIR=(NGIR*YIG+2.*SUM)*COMPN                                     00026300
      YI=YIGIR+YISLAB+YIBAR                                             00026400
      WRITE(OUNIT,*)'        TORSIONAL CONSTANT BASED ON E OF SLAB.'    00026500
      WRITE(OUNIT,305)TOTBOX,YI                                         00026600
      GO TO 250                                                         00026700
97    WRITE(OUNIT,*)'        ERROR IN GIRDER TYPE'                      00026800
      GOTO 666
      END IF                                                            00027000
CC                                                                      00027100
CC  WRITE TORSION VALUES                                                00027200
CC                                                                      00027300
200   WRITE(OUNIT,302)TOTBOX                                            00027400
250   GO TO 10                                                          00027500
CC  OUTPUT FORMATS                                                      00027600
CC                                                                      00027700
300   FORMAT(/,8X,'INPUT DATA:',//,                                     00027800
     613X,'SLAB WIDTH =',F9.2,' FT.',5X,                                00027900
     6    'BOX DEPTH =',4X, F6.2,' FT.',/,                              00028000
     613X,'TOP SLAB THCK =',F6.2,' IN.',5X,                             00028100
     6    'BOT SLAB THCK =',F6.2,' IN.',/,                              00028200
     613X,'NO. INT. GIRD =',I3,12X,                                     00028300
     6    'INT GIRD THCK =',F6.2,' IN.',/,                              00028400
     613X,'LT. WEB THICK =',F6.2,' IN.',5X,                             00028500
     6    'LEFT FACTOR =',  F8.2,' FT.',/,                              00028600
     613X,'RT. WEB THICK =',F6.2,' IN.',5X,                             00028700
     6    'RT. FACTOR =',  F9.2,' FT.',/,                               00028800
     613X,'LT. CANT LEN. =',F6.2,' FT.',/,                              00028900
     613X,'CANT THCK INT =',F6.2,' IN.',5X,                             00029000
     6    'CANT THCK EXT =',F6.2,' IN.',/,                              00029100
     613X,'RT. CANT LEN. =',F6.2,' FT.',/,                              00029200
     613X,'CANT THCK INT =',F6.2,' IN.',5X,                             00029300
     6    'CANT THCK EXT =',F6.2,' IN.',///)                            00029400
301   FORMAT(8X,'INPUT DATA WRITTEN TO FIXED FORMAT',/,8X,              00029500
     6          'FILE FOR BDS SECTION PROPERTIES RUN.',//)              00029600
302   FORMAT(/,8X,'TORSIONAL CONSTANT =',F15.4,' FT.**4')               00029700
305   FORMAT(/,8X,'TORSIONAL CONSTANT =',F15.4,' FT.**4',//,            00029800
     6         8X,'LATERAL INERTIA    =',F15.4,' FT.**4')               00029900
303   FORMAT(/,8X,'INPUT DATA:',//,                                     00030000
     613X,'GIRDER SERIES =',I4,/,                                       00030100
     613X,'NO. OF GIRDERS =',I3,/,                                      00030200
     613X,'SLAB WIDTH =',4X,F6.2,' FT.',/,                              00030300
     613X,'SLAB THICK. =',3X,F6.2,' IN.',/,                             00030400
     613X,'SLAB STR. =',5X,F6.2,' KSI',/,                               00030500
     613X,'GIRD STR. =',5X,F6.2,' KSI',/,                               00030600
     613X,'GIRD SPA. =',5X,F6.2,' FT.',/,//)                            00030700
500   FORMAT(A8,3I2,2X,A1,I4,56X,I3)                                    00030800
501   FORMAT(A8,I2,12X,4I4,2I2,I1,I2,I3,I1,I2,2I3,2I2,I3,2I2,9X,I3)     00030900
C                                                                       00031000
C                                                                       00031100
C                                                                       00031200
C	Commented out by Robert Lotze:
C998   WRITE(*,*)'  READ ERROR !!! CHECK YOUR INPUT FILE.'               .T.31300
C	Added in by Robert Lotze to take up the "998" line number:
998	GOTO 666
999   GOTO 666
C	The following three lines added by Robert for Win32 dll conversion:
666	CLOSE(OUNIT)
	CLOSE(INUNIT)
      END                                                               00031500
C                                                                       00031600
C                                                                       00031700
C                                                                       00031800
C                                                                       00031900
C                                                                       00032000
C     Last two parameters added by Robert Lotze for win32 conversion:
      SUBROUTINE PARM(OUNIT,ERUNIT,CARCNT,INUNIT,
     1		INFILE,OUTFILE)

C	"*254" modified from "*16" by Robert Lotze:
      CHARACTER*254 FLNM1, FLNM2         
      CHARACTER*1 CARCNT                                                00032400
C	"INUNIT" added by Robert to make sure it's declared.
      INTEGER*4 OUNIT, ERUNIT, INUNIT
      COMMON IIUNIT,IOUNIT                                              00032600
C	Line added for Win32 conversion:
	CHARACTER*254 INFILE,OUTFILE
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   00032700
C  STANDARD INTERACTIVE INPUT UNIT                                      00032800
      IIUNIT=1                                                          00032900
C  STANDARD INTERACTIVE OUTPUT UNIT                                     00033000
      IOUNIT=3                                                          00033100
C  STANDARD REPORT OUTPUT UNIT                                          00033200
      OUNIT=6                                                           00033300
C  STANDARD FILE INPUT UNIT                                             00033400
      INUNIT=5                                                          00033500
C  STANDARD ERROR OUTPUT UNIT                                           00033600
      ERUNIT=3                                                          00033700
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   00033800
C     OPEN(IIUNIT,FILE='CON')                                           .T.33900
C     OPEN(IOUNIT,FILE='CON')                                           .T.34000

C	This block commented out by Robert S. Lotze for win32 conversion:
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C10    WRITE(*,210)                                                      .T.34100
C210   FORMAT(//,                                                        00034200
C     1' Enter input file name:',/)                                      00034300
C           READ(*,ERR=10,FMT='(A)')FLNM1                                .T.34400
C           IF(FLNM1.EQ.'CON')GO TO 25                                   .T.34500
C           IF(FLNM1.EQ.'con')GO TO 25                                   .T.34600
C              DO 15 I=1,16                                              00034700
C               IF(FLNM1(I:I).EQ.'.')GO TO 25                            00034800
C15             CONTINUE                                                 00034900
C              DO 20 I=1,16                                              00035000
C                 IF(FLNM1(I:I).EQ.' ')THEN                              00035100
C                  IE = I+3                                              00035200
C                  FLNM1(I:IE) = '.dat'                                  00035300
C                  GO TO 25                                              00035400
C                 END IF                                                 00035500
C20             CONTINUE                                                 00035600
C25         WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1      .T.35700
C          CARCNT='1'                                                    00035800
C35    WRITE(*,235)                                                      .T.35900
C235   FORMAT(//,                                                        00036000
C     1' Enter output file name:',/)                                      00036100
C           READ(*,ERR=35,FMT='(A)')FLNM2                                .T.36200
C           IF(FLNM2.EQ.'CON')GO TO 60                                   .T.36300
C           IF(FLNM2.EQ.'con')GO TO 60                                   .T.36400
C              DO 40 I=1,16                                              00036500
C               IF(FLNM2(I:I).EQ.'.')GO TO 60                            00036600
C40             CONTINUE                                                 00036700
C              DO 50 I=1,16                                              00036800
C                 IF(FLNM2(I:I).EQ.' ')THEN                              00036900
C                  IE = I+3                                              00037000
C                  FLNM2(I:IE) = '.out'                                  00037100
C                  GO TO 60                                              00037200
C                 END IF                                                 00037300
C50             CONTINUE                                                 00037400
C60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2    .T.37500
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C	The following two setting of varables added by Robert for Win32 dll conversion
	FLNM1 = INFILE
	FLNM2 = OUTFILE

          OPEN(OUNIT,FILE=FLNM2)                                        00037600
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')                          00037700
      IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5                      00037800
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6                      00037900
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6                      00038000
      RETURN                                                            00038100
      END                                                               00038200
C
C
C
C	Last parameter added by Robert Lotze for win32 conversion:
      SUBROUTINE BDFILE(BDUNIT,MYFILENAME)
C	"*254" changed from "*16" by Robert Lotze:
      CHARACTER*254 XTFILE
C	This line addded for win32 dll conversion:
	CHARACTER*254 MYFILENAME
      INTEGER BDUNIT                                                    00041900
      COMMON IIUNIT,IOUNIT                                              00042000
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   00042100
C  STANDARD INTERACTIVE INPUT UNIT                                      00042200
C     IIUNIT=1 FOR NORMAL OPERATION                                     00042300
C     IIUNIT=5 IF INUNIT ASSIGNED TO CON                                00042400
C  STANDARD INTERACTIVE OUTPUT UNIT                                     00042500
C     IOUNIT=3 FOR NORMAL OPERATION                                     00042600
C     IOUNIT=6 IF INUNIT ASSIGNED TO CON                                00042700
C  BDS OUTPUT UNIT                                                      00042800
      BDUNIT=12                                                         00042900
C
C	This block commented out by Robert Lotze for win32 conversion:
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                   00043000
C5     WRITE(*,210)                                                      .T.43100
C210   FORMAT(//,                                                        00043200
C     1' Enter BDS output file name:',//)                                00043300
C      READ(*,ERR=5,FMT='(A)')XTFILE                                     .T.43400
C      IF(XTFILE.EQ.'CON')GO TO 30                                       .T.43500
C      IF(XTFILE.EQ.'con')GO TO 30                                       .T.43600
C      DO 10 I=1,16                                                      00043700
C      IF(XTFILE(I:I).EQ.'.')GO TO 30                                    00043800
C10    CONTINUE                                                          00043900
C      DO 20 I=1,16                                                      00044000
C      IF(XTFILE(I:I).EQ.' ')THEN                                        00044100
C      IE = I+4                                                          00044200
C      XTFILE(I:IE) = '.dat'                                             00044300
C      GO TO 30                                                          00044400
C      END IF                                                            00044500
C20    CONTINUE                                                          00044600
C30    WRITE(*,FMT='(/,'' '',1X,A16,'' used for BDS file.'',//)')XTFILE  .T.44700
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC	
C
C	The following line was added by Robert Lotze for win32 conversion
C	  and the line following that was commented out.
	OPEN(BDUNIT,FILE=MYFILENAME)
C      OPEN(BDUNIT,FILE=XTFILE)                                          00044800
      RETURN                                                            00044900
      END                                                               00045000
C