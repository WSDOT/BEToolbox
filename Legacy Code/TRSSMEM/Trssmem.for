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


C**** TRUSS MEMBER SECTION PROPERTIES**'TRSSMEM(X)'***       *RFF720728*P6300010
C                                                                          00020
C     A - GARY CHRISTOPHER - TSO CONVERSION - 8/01/73                   GCA00030
C     B - NEAL CAMPBELL - PC CONVERSION - 4/16/86                         B
C     T - TERRY BEEBE   - VMS CONVERSION - JANUARY 1990                   T 
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C                                                                          00040
      IMPLICIT REAL*8(A-H,O-Z)                                          RWT00050
      CHARACTER*1 CARCNT                                                  B
      CHARACTER*4 IMODE                                                   B
      CHARACTER*8 REVDAT, TIME                                            T
      CHARACTER*11 DATE                                                   T
      CHARACTER*23 DATIME                                                 T
      INTEGER*4 OUNIT, ERUNIT, STP                                        B
      LOGICAL*1 CCHAR                                                   GCA00100
CC                                                                        B
CC                                                                        B
      INUNIT=5                                                            B
      STP=9                                                               B
      OUNIT=9                                                             B
      ERUNIT=9                                                            B
      IPAGE=0                                                             B
      CARCNT='1'
      IMODE='FREE'                                                        B
      CCHAR=.TRUE.                                                        B
      MAXLIN=66                                                           B
C     CALL LIB$DATE_TIME (DATIME)                                         T
	CALL DATE_AND_TIME (DATIME)
      TIME = DATIME(13:20)                                                T
      DATE = DATIME(1:11)                                                 T 
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC    B
      REVDAT = ' 1/31/90'                                                 T
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC    B
C                                                                         B
C                                                                         B
C        PARM INFO PASSED TO THE PROGRAM                                GCA00170
C   IMODE  = 'FREE' NOT USED ANYMORE                                      B00180
C   OUNIT  - IS THE UNIT UPON WHICH THE OUTPUT IS PRODUCED                B00190
C           (3 = MAIN FRAME , 6 = PC)                                     B00200
C   ERUNIT - IS THE UNIT UPON WHICH THE ERROR MESSAGES ARE PRINTED        B00210
C           (3 = MAIN FRAME , 9 = PC)                                     B00220
C   CCHAR  = .TRUE. NOT USED ANYMORE                                      B00230
C   MAXLIN - IS THE MAXIMUM NUMBER OF LINES PERMITTED ON ONE PAGE         B00270
C           (DEFAULT IS 66)                                               B00280
      CALL PARM(IMODE,OUNIT,ERUNIT,CCHAR,MAXLIN)                          B00290
      STP = OUNIT                                                       GCA00300
      ICOUNT = MAXLIN + 1                                                 B00310
      GO TO 11                                                            B
5     WRITE (STP,7)                                                     RFF00370
    7 FORMAT (/,'        END OF PROBLEM GROUP')                           B00380
      STOP                                                                B
   11 READ(INUNIT,END=5,ERR=94,FMT=*)PW,PT,OO,RIV,CW,CT,HH,NP,NC          B00430
      IF (NP)25,20,25                                                     B00460
   20 PN=2.                                                             TMS00470
      NP=PN                                                             TMS00480
      GO TO 30                                                          TMS00490
   25 PN=NP                                                             TMS00500
   30 IF (NC)40,35,40                                                   TMS00510
   35 CN=2.                                                             TMS00520
      NC=CN                                                             TMS00530
      GO TO 45                                                          TMS00540
   40 CN=NC                                                             TMS00550
   45 AG=PN*PW*PT+CN*(CW-HH)*CT                                         TMS00560
      AN=PN*(PW-RIV)*PT+CN*(CW-HH)*CT                                   TMS00570
      AC=AG+(HH/2.)*CT*CN                                               TMS00580
      WT=AC*3.4                                                         TMS00590
      ACOV=(CW-HH)*CT                                                   TMS00600
      BCOV=ACOV*CN                                                      TMS00610
      APL=PW*PT                                                         TMS00620
      BPL=APL*PN                                                        TMS00630
      PLN=PN*(PW-RIV)*PT                                                TMS00640
      COVN=CN*(CW-HH)*CT                                                TMS00650
      IF (NC-71)47,46,47                                                TMS00660
   46 W=1.                                                              TMS00670
      GO TO 49                                                          TMS00680
   47 W=2.                                                              TMS00690
   49 XI=PN*PT*(PW**3)/12.+W*ACOV*((PW/2.)+(CT/2.))**2.                 TMS00700
      RX=DSQRT(XI/AG)                                                   RFF00710
      YI=CN*CT*((CW**3)-(HH**3))/12.+(APL*((OO-PT)/2.)**2)*2.           TMS00720
      RY=DSQRT(YI/AG)                                                   RFF00730
      IF (ICOUNT.GT.MAXLIN)THEN                                           B
      IPAGE=IPAGE+1                                                       B
      WRITE(STP,1000)CARCNT,DATE,TIME                                     B
      WRITE(STP,1001)REVDAT,IPAGE                                         B
 1000 FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION    ',    T
     1   A11'  TIME ',A8)                                                 T
 1001 FORMAT('        *** TRUSS MEMBER SECTION PROPERTIES ***     REV ',  B
     1A8,'  PAGE',I3,//)                                                  B
      ICOUNT=25                                                           B
      END IF                                                              B
      WRITE (STP,48)                                                    RWT00750
48    FORMAT(/,' ','       SECTION PROPERTIES',/)                         B00760
      WRITE (STP,50)NP,PW,PT,RIV,NC,CW,CT,HH                            RFF00770
50    FORMAT(' ',7X,I2,' SIDE  PLATES @',F6.2,' X ',F6.3,' IN., W/'       B00780
     1,F3.0,' LIN RIVET HOLE'/' ',7X,I2,' COVER PLATES @',F6.2,' X ',     B00790
     2F6.3,' IN., W/',F6.3,' IN.WIDE H.H.')                               B00800
      WRITE (STP,55)OO                                                  RWT00810
55    FORMAT(' ','       TOTAL WIDTH O/O OF PLATES = ',F7.3,' INCHES')    B00820
      WRITE (STP,60)AG,AN,AC,XI,RX,YI,RY                                RWT00830
60    FORMAT(' ',7X,'AG=',F7.2,',    AN=',F7.2,',    AC=',F7.2/' ',7X,    B00840
     1'IX=', F9.1,',  RX=',F7.2,',    IY=',F9.1,',  RY=',F6.2)            B00850
      WRITE (STP,80) WT,BPL,PLN,BCOV,COVN                               RFF00860
80    FORMAT(' ','       WT=', F7.2 , ',    GROSS PL. A=' , F6.2,',    N  B00870
     1ET PL. A=',F6.2/' ',22X,'GROSS COV A=',F6.2,                        B00880
     2                                ',    NET COV A=',F6.2/' ',         B00880
     3/,' ',7X,'*************************************************')
      ICOUNT=ICOUNT+12                                                  RFF00890
      GO TO 11                                                            B00900
   94 WRITE(*,95)                                                       .T.00920
   95 FORMAT(/,' ERRONEOUS DATA - CORRECT AND RESUBMIT JOB')            GCA00930
  100 STOP                                                              RWT00940
      END                                                               TMS00950
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
      SUBROUTINE PARM(IMODE,OUNIT,ERUNIT,CCHAR,MAXLIN)                    B
      CHARACTER*16 FLNM1, FLNM2                                           B
      CHARACTER*4 IMODE                                                   B
      LOGICAL*1 CCHAR                                                     B
      INTEGER OUNIT, ERUNIT                                               B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                     B
C  STANDARD INTERACTIVE INPUT UNIT                                        B
      IIUNIT=1                                                            B
C  STANDARD INTERACTIVE OUTPUT UNIT                                       B
      IOUNIT=3                                                            B
C  STANDARD REPORT OUTPUT UNIT                                            B
      OUNIT=6                                                             B
C  STANDARD FILE INPUT UNIT                                               B
      INUNIT=5                                                            B
C  STANDARD ERROR OUTPUT UNIT                                             B
      ERUNIT=3                                                            B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                     B
      CCHAR=.TRUE.                                                        B
      IMODE='FREE'                                                        B
      MAXLIN=66
C     OPEN(IIUNIT,FILE='CON')                                             T
C     OPEN(IOUNIT,FILE='CON')                                             T
10    WRITE(*,210)                                                        T
 210  FORMAT(//,                                                          T
     1' Enter input file name:',/)                                        T
           READ(*,ERR=10,FMT='(A)')FLNM1                                  T
           IF(FLNM1.EQ.'CON')GO TO 25                                     T
           IF(FLNM1.EQ.'con')GO TO 25                                     T
              DO 15 I=1,16                                                B
               IF(FLNM1(I:I).EQ.'.')GO TO 25                              B
15             CONTINUE                                                   B
              DO 20 I=1,16                                                B
                 IF(FLNM1(I:I).EQ.' ')THEN                                B
                  IE = I+3                                                B
                  FLNM1(I:IE) = '.dat'                                    B
                  GO TO 25                                                B
                 END IF                                                   B
20             CONTINUE                                                   B
25         WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1        T
35    WRITE(*,235)                                                        T
 235  FORMAT(//,                                                          T
     1' Enter output file name:',/)                                       T
           READ(*,ERR=35,FMT='(A)')FLNM2                                  T
           IF(FLNM2.EQ.'CON')GO TO 60                                     T
           IF(FLNM2.EQ.'con')GO TO 60                                     B
              DO 40 I=1,16                                                B
               IF(FLNM2(I:I).EQ.'.')GO TO 60                              B
40             CONTINUE                                                   B
              DO 50 I=1,16                                                B
                 IF(FLNM2(I:I).EQ.' ')THEN                                B
                  IE = I+3                                                B
                  FLNM2(I:IE) = '.out'                                    B
                  GO TO 60                                                B
                 END IF                                                   B
50             CONTINUE                                                   B
60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2      T
          OPEN(OUNIT,FILE=FLNM2)                                          B
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')                            B
      IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5                        B
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6                        B
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6                        B
      RETURN                                                              B
      END                                                                 B
C                                                                         B
C                                                                         B
