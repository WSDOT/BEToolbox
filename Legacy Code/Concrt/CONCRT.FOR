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

C      CORRECTION BY C.Y. HOU , FEB. 13 1975                            CYH00010
C                           CONCRETE T-BEAMS                               00020
C                                                                          00030
C         THIS PROGRAM  COMPUTES STRESSES IN T-BEAMS                       00040
C                                                                          00050
C     A - GARY CHRISTOPHER - REV. 8/20/73                                 A00060
C     B - NEAL CAMPBELL -PC CONVERSION 4/16/86                            B
C     T - TERRY BEEBE   - VMS CONVERSION JANUARY 1990                     T  
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C                                                                         A00070
      REAL*4  J,J1,J2,J3,J4,J5                                             00080
      REAL*4  K,K1,K2,K4,K5                                                00100
      REAL*4  L,N                                                          00110
      CHARACTER*1 CARCNT                                                  B
      INTEGER*4 OUNIT, ERUNIT                                             B
      CHARACTER*8 REVDAT, TIME                                            T
      CHARACTER*11 DATE                                                   T
      CHARACTER*23 DATIME                                                 T                             
      NW=3                                                                 00140
      NR=5                                                                 00150
      NERR=3                                                              B
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC    B
      REVDAT = ' 4/16/86'                                                 B
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC    B
      CARCNT='1'                                                          B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                        B
C     OUNIT = (6 FOR PC)                                                  T
C             (3 FOR FILE OUTPUT)                                         T
C     ERUNIT = (3 FOR FILE OUTPUT)                                        T
C     INUNIT = (5 FOR FILE INPUT)                                         B
C     CARCNT = ('1' FOR BATCH)                                            B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                        B
C                                                                         B
C                                                                         B
      CALL PARM(OUNIT,ERUNIT,CARCNT,INUNIT)                               B
      NW=OUNIT                                                            B
      NR=INUNIT                                                           B
      NERR=ERUNIT                                                         B
      ICNT=-1                                                             B
      CALL LIB$DATE_TIME (DATIME)                                         T
      TIME = DATIME(13:20)                                                T  
      DATE = DATIME(1:11)                                                 T 
C                                                                         B
C                                                                         B
 1000 FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION    ',    T
     1  A11,'  TIME' ,A8)                                                 T
 1001 FORMAT('        *** SERVICE LOAD BEAM ***     REV ',A8,//)          B
       WRITE(NW,1000)CARCNT,DATE,TIME                                     B
       WRITE(NW,1001)REVDAT                                               B
      WRITE(NW,10)                                                         00170
10    FORMAT('        IDENT.    KD,IN.    JD,IN.    F(C),PSI    F(S),PSI  B00190
     1   F-(S),PSI'/)                                                     B00200
C                                                                          00210
20    ICNT=ICNT+1                                                         B
      READ(NR,END=150,ERR=490,FMT=*)IDENT,B,D,T,W,N,A,C,E,M,U,Q           B
      IF(U.NE.0.0) GO TO 30                                                00370
      Z=0.0                                                                00380
      GO TO 40                                                             00390
   30 V=(B*T*(Q+D-.5*T)+W*(Q+D-T)**2*.5)/(B*T+W*(Q+D-T))                   00400
      F=12*M/U+V-Q                                                         00410
   40 J=.87                                                                00420
C                                                                          00430
   45 IF(U.EQ.0.0) GO TO 60                                                00440
      Z=J*D/F                                                              00450
      IF(Z.LT.1.0)GO TO 60                                                 00460
C                                                                          00470
      WRITE(NW,50)IDENT,Z,IDENT                                            00480
   50 FORMAT(//'         IDENT= ',I3,5X,'JD/E= ',F8.4,//,'  USE ECCENTRI  B00490
     1CALLY LOADED COLM. FOR IDENT= ',I3,/)                               B00500
      GO TO 20                                                             00510
C                                                                          00520
   60 R=1/(1-Z)                                                            00530
      P=(A*R)/(W*D)                                                        00540
      P1=C/(W*D)                                                           00550
      P2=((B-W)*T)/(W*D)                                                   00560
      K1=N*P+(2*N-1)*P1+P2                                                 00570
      K2=2*(N*P+(2*N-1)*P1*(E/D)+P2*T/(2*D))                               00580
      K4=K1**2+K2                                                          00590
      K=K4**0.5-K1                                                         00600
C                                                                          00610
      IF(K .LT. 1.0) GO TO 80                                              00620
C                                                                          00630
      WRITE(NW,70) IDENT,K                                                 00640
   70 FORMAT(//'         IDENT= ',I3,5X,'K= ',F8.4,//)                    B00650
      GO TO 20                                                             00660
C                                                                          00670
   80 Y=(1-(2*T)/(3*K*D))/(2-T/(K*D))                                      00680
      J3=K**2/6+(2*N*C/(W*D)-C/(W*D))*(E/D-E**2/(K*D**2))                  00690
      J4=(B*T-W*T)/(W*D)*(Y*T/D)*(1-T/(K*D*2))                             00700
      J5=K/2+(2*N-1)*C/(W*D)*(1-E/(K*D))+(B-W)*T/(W*D)*(1-T/(K*D*2))       00710
      J1=1-(J3+J4)/J5                                                      00720
C                                                                          00730
      IF(J1 .LE. (J+.005).AND.J1 .GE. (J-.005))GO TO 90                    00740
      J=J1                                                                 00750
      GO TO 45                                                             00760
C                                                                         A00770
   85 CONTINUE                                                            A00780
      GO TO 20                                                            A00790
C                                                                          00800
   90 IF(U.EQ.0.0) GO TO 100                                               00810
C                                                                          00820
      G=(1000*U*F)/(J1*A*R*D)                                              00830
      H=(G/N)*(K/(1-K))                                                    00840
      IF(C.EQ.0.0)GO TO 120                                                00850
      GO TO 110                                                            00860
C                                                                          00870
  100 G=(12000*M)/(J1*D*A)                                                 00880
      H=(G/N)*(K/(1-K))                                                    00890
      IF(C.EQ.0.0) GO TO 120                                               00900
C                                                                          00910
  110 L=2*G*((K-E/D)/(1-K))                                                00920
  120 J2=J1*D                                                              00930
      K5=K*D                                                               00940
      K5=INT(100.0*K5+.5)/100.0                                            00950
      J2=INT(100.0*J2+.5)/100.0                                            00960
      IH=H+.5                                                              00970
      IG=G+.5                                                              00980
C                                                                          00990
      WRITE(NW,130) IDENT,K5,J2,IH,IG                                      01000
  130 FORMAT(' ',8X,I3,5X,F7.2,3X,F7.2,4X,I8,4X,I8)                        01010
      IF(C.EQ.0.0)GO TO 20                                                 01020
      IL=L+.5                                                              01030
      WRITE(NW,140)IL                                                      01040
  140 FORMAT(' ',62X,I8)                                                   01050
      L=0                                                                  01060
      GO TO 20                                                             01070
  150 IF(ICNT.EQ.0)GO TO 490                                              A01080
      REWIND NR                                                            01090
      WRITE(NW,300)                                                        01120
  300 FORMAT(//,'        ','INPUT',//,                                    B01130
     1            '        PBM    B',6X,'D',5X,'T',6X,'B1',4X,'N',4X,'AS   01140
     1',3X,'AS1',4X,'D1',5X,'M',4X,'P',5X,'Q',/)                           01150
      DO 350 IX=1,ICNT                                                    B
      READ(NR,END=490,ERR=490,FMT=*)IDENT,B,D,T,W,N,A,C,E,M,U,Q           B
  350 WRITE(NW,400)IDENT,B,D,T,W,N,A,C,E,M,U,Q                             01160
  400 FORMAT(' ',7X,I3,1X,F5.2,1X,F6.2,1X,F5.2,1X,F6.2,1X,F4.1,1X,F5.2,1  B01170
     1X,F5.2,1X,F5.2,1X,I6,1X,F5.1,1X,F3.1)                                B1180
      GO TO 500                                                           B
  490 WRITE(NERR,*)' INPUT ERROR!!! CHECK YOUR DATA FILE!!!'              B
  500 STOP                                                                 01200
      END                                                                  01210
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
      SUBROUTINE PARM(OUNIT,ERUNIT,CARCNT,INUNIT)                         B
      CHARACTER*16 FLNM1, FLNM2                                           B
      CHARACTER*1 CARCNT                                                  B
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
C     OPEN(IIUNIT,FILE='CON')                                             T
C     OPEN(IOUNIT,FILE='CON')                                             T
10    WRITE(*,FMT='(//,'' Enter input file name:'',/)')                   T
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
          CARCNT='1'                                                      B
35    WRITE(*,FMT='(//,'' Enter output file name:'',/)')                  T
           READ(*,ERR=35,FMT='(A)')FLNM2                                  T
           IF(FLNM2.EQ.'CON')GO TO 60                                     T
           IF(FLNM2.EQ.'con')GO TO 60                                     T
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
