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

      PROGRAM DISTBM
C
C
C                   PROGRAM CALCULATES LOAD DISTRIBUTION FOR
C                   MULTI-BEAM CONCRETE GIRDER BRIDGES
C
C     REVISION LOG:
C
C     T - TERRY BEEBE - VMS CONVERSION JANUARY 1990                      T  
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/24/93
C
      CHARACTER*8 REVDAT, TIME                                           T
      CHARACTER*11 DATE                                                  T
      CHARACTER*23 DATIME                                                T
      CHARACTER*1 QUES
      CHARACTER*1 CARCNT
      INTEGER*4 OUNIT, ERUNIT
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      REVDAT = ' 2/20/88'
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     IMODE  = (0 FOR BATCH)
C              (1 FOR INTERACTIVE)
C     OUNIT = (6 FOR FILE OUTPUT)
C             (3 FOR SCREEN OUTPUT (INTERACTIVE)
C     CARCNT = ('1' FOR BATCH)
C              (' ' FOR INTERACTIVE)
C     INUNIT = (5 FOR FILE INPUT)
C              (1 FOR INTERACTIVE)
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C
      IMODE = 0
      OUNIT = 6
      ERUNIT = 3
      CARCNT = '1'
      INUNIT = 5
      CALL PARM(IMODE,OUNIT,CARCNT,INUNIT)
      CALL LIB$DATE_TIME (DATIME)
      TIME = DATIME(13:20)
      DATE = DATIME(1:11)
C
C   BEGIN INTERACTIVE INPUT ROUTINE
C
    5 IF (IMODE.EQ.1) THEN
   10 WRITE(*,15)
   15 FORMAT(' ',/,' ENTER THE MULTI-BEAM SERIES ( 27, 35 OR 53 ) AND'
     1        ,/,'THE GIRDER SPACING IN FEET',/)
      READ(*,FMT=*,ERR=10)KSER,SPA
   20 WRITE(*,25)
   25 FORMAT(' ',/,' ENTER SPAN LENGTH & SUPERSTRUCTURE WIDTH IN FT.',/) T
      READ(*,FMT=*,ERR=20)SPANL,WIDTH
   30 WRITE(*,35)
   35 FORMAT(' ',/,' ENTER THE NUMBER OF LIVE LOAD LANES',/)
      READ(*,FMT=*,ERR=30)NL
      ELSE
C
C   BEGIN BATCH INPUT ROUTINE
C
      READ(INUNIT,END=2000,ERR=998,FMT=*)KSER,SPA,SPANL,WIDTH,NL
      END IF
C
C   BEGIN CALCULATION OF GIRDER PROPERTIES
C
      IF ( SPA .GT. 6.417 ) SPA = 6.417
      TORJ = 162.*SPA * (5.333 - 1.68/SPA * (1. - 1./192. / SPA ** 4. ))
C
C   SERIES 27 TRI-DECK
C
      IF ( KSER .EQ. 27 ) THEN
      TORJ = TORJ + 4166.
      BTF = SPA * 12.
      ADSLAB = ( BTF - 48. ) * 6.
      SLABI = ( BTF - 48. ) * 36.
      YB = ( 681.75 * 16.53 + ADSLAB * 24. ) / ( ADSLAB + 681.75 )
      ADY2G = 681.75 * ( ABS ( YB - 16.53 ) ) ** 2.
      ADY2S = ADSLAB * ( ABS ( YB - 24. ) ) ** 2.
      GIRI =  43046.95 + SLABI + ADY2G + ADY2S
      AREA = 681.75 + ADSLAB
      END IF
C
C   SERIES 35 BULB-TEE
C
      IF ( KSER .EQ. 35 ) THEN
      TORJ = TORJ + 3522.
      BTF = SPA * 12.
      ADSLAB = ( BTF - 48. ) * 6.
      SLABI = ( BTF - 48. ) * 36.
      YB = ( 650. * 21.43 + ADSLAB * 32. ) / ( ADSLAB + 650. )
      ADY2G = 650. * ( ABS ( YB - 21.43 ) ) ** 2.
      ADY2S = ADSLAB * ( ABS ( YB - 32. ) ) ** 2.
      GIRI =  98246.3 + SLABI + ADY2G + ADY2S
      AREA = 650. + ADSLAB
      END IF
C
C                       SERIES 53 BULB-TEE
C
      IF ( KSER .EQ. 53 ) THEN
      TORJ = TORJ + 4272.
      BTF = SPA * 12.
      ADSLAB = ( BTF - 48. ) * 6.
      SLABI = ( BTF - 48. ) * 36.
      YB=( 740. * 32.263+ ADSLAB * 50. ) / ( ADSLAB + 740.)
      ADY2G = 740. * ( ABS ( YB - 32.263 ) ) ** 2.
      ADY2S = ADSLAB * ( ABS ( YB - 50. ) ) ** 2.
      GIRI =   282612.0 + SLABI + ADY2G + ADY2S
      AREA = 740. + ADSLAB
      END IF
C
C  BEGIN CALCULATIONS OF LOAD DISTRIBUTION
C
      GIRK = SQRT ( 1.2 * GIRI / TORJ )
      GIRC = GIRK * WIDTH / SPANL
      RNL = NL
      IF ( GIRC .GT. 5. ) THEN
           BDMD =  5.75 - 0.5 * NL
      ELSE
           BDMD = ( 5.75 - 0.5*RNL ) + 0.7*NL * ( 1. - 0.2*GIRC ) ** 2.
      END IF
      BDMSN = SPA / BDMD
C
C                       BEGIN OUTPUT
C
      WRITE (OUNIT,45)  CARCNT,DATE,TIME
  45  FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION   ',    T
     1  A11,'  TIME ',A8)
      WRITE (OUNIT,55)REVDAT
  55  FORMAT('           *** DISTRIBUTION OF LIVE LOAD *** ',/,
     1       '        *** FOR STANDARD MULTI-BEAM GIRDERS ***
     2' ,/,8X,'REV ',A8,///)
      WRITE (OUNIT,65)KSER,SPA,SPANL,WIDTH,NL
  65  FORMAT('        INPUT DATA ECHO:',//,'            GIRDER SERIES ='
     1,I6,/,'            GIR SPACING   =',F6.2,'  FT',/,
     2'            SPAN LENGTH   =',F6.2,'  FT',/,
     3'            SUPER WIDTH   =',F6.2,'  FT',/,
     4'            NO. OF LANES  =',I6,///)
      WRITE (OUNIT,75)TORJ,GIRI,AREA,YB,GIRK,GIRC
  75  FORMAT('        PROGRAM RESULTS:',//,'            TORSION CONST ='
     1,F9.0,'  IN**4',/,'            MOM. OF INER. =',F9.0,'  IN**4',/,
     2'            AREA          =',F9.2,'  IN**2',/,
     3'            Y BOTTOM      =',F9.2,'  IN',/,
     4'            GIRDER K      =',F9.3,'  [(1+æ)I/J]**.5',/,
     5'            GIRDER C      =',F9.3,'  K(W/L)',///)
      WRITE (OUNIT,95)SPA,BDMD,BDMSN,BDMD
  95  FORMAT('        AS PER OUR BDM:',//,
     1'            S             =',F6.3,'  FT',/,
     2'            D             =',F6.3,'  FT',/,
     3'            S/D (DIST)    =',F6.3,'  WHEELS PER GIRDER',/,
     4'            N FOR PSGDES  =',F6.3,'  FT',///)
      IF ( IMODE .EQ. 1 ) THEN
100        WRITE(*,105) 
105        FORMAT(//,
     1     'IS THERE ANOTHER PROBLEM ? (YES/NO)',/)
           READ(*,ERR=100,FMT='(A)')QUES
           IF(QUES.EQ.'y') QUES='Y'
           IF(QUES.NE.'Y') GO TO 2000
      END IF
      GO TO 5
C
C                       END OUTPUT
C
 998  WRITE(*,*)'   INPUT ERROR!!!  CHECK YOUR INPUT FILE!!'
      WRITE(OUNIT,*)'   INPUT ERROR!!!  CHECK YOUR INPUT FILE!!'
 2000 STOP
      END
C
C
C
C
C
      SUBROUTINE PARM(IMODE,OUNIT,CARCNT,INUNIT)
      CHARACTER*16 FLNM1, FLNM2
      CHARACTER*1 CARCNT, QUES
      INTEGER OUNIT, ERUNIT
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     IMODE  = (0 FOR BATCH)
C              (1 FOR INTERACTIVE)
C     OUNIT = (6 FOR FILE OUTPUT)
C             (3 FOR SCREEN OUTPUT (INTERACTIVE)
C     CARCNT = ('1' FOR BATCH)
C              (' ' FOR INTERACTIVE)
C     INUNIT = (5 FOR FILE INPUT)
C              (1 FOR INTERACTIVE)
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      ERUNIT =  3
C ASK IF ITERACTIVE FOR INPUT OR NOT
      IMODE=1
      CARCNT=' '
      INUNIT=1
      OUNIT=6
5     WRITE(*,200)
200   FORMAT(//,
     1' Do you want to run interactive? (y/n)',/)
      READ (*,ERR=5,FMT='(A)')QUES
      IF(QUES.EQ.'n') QUES='N'
          IF(QUES.EQ.'N') THEN
           IMODE = 0
           INUNIT=5
10    WRITE(*,210)
210   FORMAT(//,
     1' Enter input file name:',/)
           READ(*,ERR=10,FMT='(A)')FLNM1
           IF(FLNM1.EQ.'con'.OR.FLNM1.EQ.'CON')THEN
               INUNIT=1
               IMODE=0
               GO TO 30
           END IF
              DO 15 I=1,16
               IF(FLNM1(I:I).EQ.'.')GO TO 25
15             CONTINUE
              DO 20 I=1,16
                 IF(FLNM1(I:I).EQ.' ')THEN
                  IE = I+3
                  FLNM1(I:IE) = '.dat'
                  GO TO 25
                 END IF
20             CONTINUE
25    WRITE(*,FMT='(/,'' '',1X,A16,'' used for input  '',//)')FLNM1
           END IF
C  ASK IF INTERACTIVE FOR OUTPUT OR NOT
30    WRITE(*,230)
230   FORMAT(//,
     1' Do you want a print out? (yes/no)',/)
      READ(*,ERR=30,FMT='(A)')QUES
      IF(QUES.EQ.'y') QUES='Y'
         IF(QUES.EQ.'Y') THEN
          OUNIT=6
          CARCNT='1'
35    WRITE(*,235)
235   FORMAT(//,
     1' Enter output file name:',/)
           READ(*,ERR=35,FMT='(A)')FLNM2
C           IF(FLNM2.EQ.'con'.OR.FLNM2.EQ.'CON')THEN
C                OUNIT = 3
C                CARCNT = ' '
C                GO TO 70
C           END IF
              DO 40 I=1,16
               IF(FLNM2(I:I).EQ.'.')GO TO 60
40             CONTINUE
              DO 50 I=1,16
                 IF(FLNM2(I:I).EQ.' ')THEN
                  IE = I+3
                  FLNM2(I:IE) = '.out'
                  GO TO 60
                 END IF
50             CONTINUE
60        WRITE(*,FMT='(/,'' '',1X,A16,'' used for output'',/// )')FLNM2
          OPEN (OUNIT,FILE=FLNM2,STATUS='NEW')
          END IF
70        IF(INUNIT.EQ.5)OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')
          IF(INUNIT.EQ.1)OPEN(INUNIT,FILE='CON')
C          OPEN (ERUNIT,FILE='CON')
      RETURN
      END
C
C
C
