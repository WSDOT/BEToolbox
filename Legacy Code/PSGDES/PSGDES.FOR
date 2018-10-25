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


      PROGRAM PSGDES
C
C     ***  PRESTRESSED GIRDER DESIGN PACKAGE *** 'PSGDES' **************
C
C
      CHARACTER*1 CARCNT
      CHARACTER*8 TIME,REVDAT                                           .C.
      CHARACTER*80 LINE
      INTEGER*4 OUNIT,ERUNIT,TUNIT
      CHARACTER*11 DATE                                                 .C.
      CHARACTER*23 DATIME                                               .C. 
      COMMON /PARMIN/ IRS,IDEF,IPICK,OUNIT,ERUNIT,INUNIT,TUNIT,K2
      COMMON /REV/ TIME,REVDAT,CARCNT,DATE                              .C.
C
C
C
C     REVISION LOG:
C
C
C          12/16/87 - NEAL J. CAMPBELL - ADD ROUTINES TO COMPUTE BULB
C                     TEE PROPERTIES AND CORRECTIONS FROM CHECKS BY
C                     HLC.
C
C           9/06/88 - NEAL J. CAMPBELL - LIMIT HOLD DOWN FORCES. CHANGE .A.
C                     MAXIMUM STRAND SLOPE TO 7 TO 1. LIMIT RATIO OF    .A.
C                     HARPED TO STRAIGHT STRANDS TO 1.                  .A.
C                                                                       .A.
C           1/10/89 - NEAL J. CAMPBELL - CORRECT HARP RISE FOR SERIES   .B.
C                     10. ADD ERROR TRAP WHEN NO. OF STRAIGHT STRANDS   .B.
C                     AND HARPED STRANDS EXCEED THE MAXIMUM GIVEN.      .B.
C                                                                       .B.
C           1/10/90 - NEAL J. CAMPBELL - CHANGE STANDARD GIRDER SHAPES. .C.
C                     CHANGE HARPED STRAND RISE TO GIVE MAX 7 TO 1 SLOPE.C.
C                     CHANGE SLAB STRENGTH TO 5000 PSI IN PREDES (6000  .C.
C                     FOR DFLECT). CHANGE DEFAULT CONC. STR. TO 6.      .C.
C                     AND 7. FOR GIRDERS (8.4 FOR DFLECT).              .C.
C                     CONVERT TO VMS.                                   .C.
C                                                                       .C.
C          12/04/91 - S.G. ROCHELLE - CHANGED NOTES TO TELL PEOPLE THEY .C.
C                     ARE USING HS-25 VERSION.                          .C.
C                                                                       .C.
C                                                                       .C.
C                                                                       .C.
C          04/27/93 - RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5
C                                                                       .C.
C                                                                       .C.
C                                                                       .C.
C                                                                       .C.
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      REVDAT = ' 8/08/91'                                               .C.
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     IRS = (1 FOR SHORT PREDES VERSION)
C           (2 FOR LONG PREDES VERSION)
C           (3 FOR DFLECT ONLY)
C           (4 FOR PICKPT ONLY)
C     IDEF = (0 FOR NO DFLECT EXECUTION FROM PREDES)
C            (1 FOR DFLECT EXECUTION FROM PREDES)
C     IPICK = (0 FOR NO PICKPT EXECUTION FROM PREDES)
C             (1 FOR PICKPT EXECUTION FROM PREDES)
C     OUNIT = (6 FOR FILE OUTPUT)
C     ERUNIT = (3 FOR FILE OUTPUT)                                      .C.
C              (6 FOR SCREEN OUTPUT)                                    .C.   
C     INUNIT = (5 FOR FILE INPUT)
C     TUNIT = (10 FOR DFLECT EXECUTION FROM PREDES SUBROUTINE)
C             (5 FOR DFLECT AND PICKPT EXECUTION FROM MAIN)
C     CARCNT = ('1' FOR BATCH)
C              (' ' FOR INTERACTIVE)
C     K2 = (1 FOR MONITORING STRAND MANIPULATION (ACTIVATE BUG UNIT))
C          (0 TRACE NOT IN EFFECT (DEACTIVATE BUG UNIT))
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C
      CALL PARM(IRS,IDEF,IPICK,OUNIT,ERUNIT,INUNIT,TUNIT,CARCNT,K2)
C      CALL LIB$DATE_TIME (DATIME)                                      .C.
	CALL DATE_AND_TIME (DATIME)
      TIME = DATIME(13:20)                                              .C.
      DATE = DATIME(1:11)                                               .C. 
C
C  READ AND WRITE ENTIRE FILE
C
 1000 FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION   ',   .C.
     1  A11,'  TIME ',A8)                                               .C.
 1001 FORMAT('        *** PRESTRESSED GIRDER DESIGN PROGRAM ***
     1' ,/,8X,'REV ',A8,//////)
      WRITE(OUNIT,1000)CARCNT,DATE,TIME
      WRITE(OUNIT,1001)REVDAT
      WRITE ( OUNIT, * ) '           Echo of Data File Used:'
      WRITE ( OUNIT,'(//,'' '')')
      DO 10  I = 1, 100
      READ  ( INUNIT, END =  20 , ERR =  30, FMT = '(A80)') LINE
   10 WRITE ( OUNIT, '(8X,A80)')LINE
   20 REWIND INUNIT
      GO TO 40
   30 WRITE ( OUNIT, * ) '      ERROR READING DATA FILE DURING INPUT FIL
     1E ECHO !'
      IF ( ERUNIT .EQ. 3 )                                              .C.
     1WRITE (ERUNIT, * ) '      ERROR READING DATA FILE DURING INPUT FIL.C.
     2E ECHO !'                                                         .C.
      STOP
C
   40 IF(IRS.EQ.3)THEN
      WRITE ( * , * ) 'PSGDEF HS-25 FORM DATED 8/08/91 WAS USED'        .C.
         CALL DFLECT
         GO TO 99
         END IF
      IF(IRS.EQ.4)THEN
      WRITE ( * , * ) 'PICKPT HS-25 FORM DATED 8/08/91 WAS USED'        .C.
         CALL PICKPT                                            
         GO TO 99
         END IF
      IF (IRS.EQ.1) THEN
      WRITE ( * , * ) 'PSGDES HS-25 VER(1) FORM DATE 8/08/91 WAS USED'  .C.
      ELSE
      WRITE ( * , * ) 'PSGDES HS-25 VER(2) FORM DATE 8/08/91 WAS USED'  .C.
      END IF
      CALL PREDES
99    STOP
      END
C
C
C
C
C
      SUBROUTINE PARM(IRS,IDEF,IPICK,OUNIT,ERUNIT,
     1                INUNIT,TUNIT,CARCNT,K2)
      CHARACTER*16 FLNM1, FLNM2
      CHARACTER*1 CARCNT,QUES
      CHARACTER*8 TIME,REVDAT                                           .C.
      INTEGER*4 OUNIT, ERUNIT, TUNIT
      CHARACTER*11 DATE                                                 .C.
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     IRS = (1 FOR SHORT PREDES VERSION)
C           (2 FOR LONG PREDES VERSION)
C           (3 FOR DFLECT ONLY)
C           (4 FOR PICKPT ONLY)
C     IDEF = (0 FOR NO DFLECT EXECUTION FROM PREDES)
C            (1 FOR DFLECT EXECUTION FROM PREDES)
C     IPICK = (0 FOR NO PICKPT EXECUTION FROM PREDES)
C             (1 FOR PICKPT EXECUTION FROM PREDES)
C     OUNIT = (6 FOR FILE OUTPUT)
C     ERUNIT = (3 FOR FILE OUTPUT)                                      .C.
C              (6 FOR SCREEN OUTPUT)                                    .C.    
C     INUNIT = (5 FOR FILE INPUT)
C     TUNIT = (10 FOR DFLECT EXECUTION FROM PREDES SUBROUTINE)
C             (5 FOR DFLECT AND PICKPT EXECUTION FROM MAIN)
C     CARCNT = ('1' FOR BATCH)
C     K2 = (1 FOR MONITORING STRAND MANIPULATION (ACTIVATE BUG UNIT))
C          (0 TRACE NOT IN EFFECT (DEACTIVATE BUG UNIT))
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      OUNIT=6
      INUNIT=5
      ERUNIT=3
      TUNIT=10
      CARCNT='1'
      K2=1
      OPEN(2,FILE='BUG.OUT',STATUS='NEW')
      IRS=0
      IPICK=0
      IDEF=0
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
10    WRITE(*,20)                                                       .C.   
20    FORMAT(/////,
     1' Prestressed Girder Design Program',//,                          .C.
     2'    1- Prestressed Girder Design (short version) ',/,            .C.
     3'    2- Prestressed Girder Design (long version)',/,              .C.
     4'    3- Prestressed Girder Deflection Analysis',/,                .C.
     5'    4- Pick Point Analysis',//,                                  .C.
     6' Enter Choice: ',/)                                              .C.
      READ(*,ERR=10,FMT=*)IRS                                           .C.
      IF(IRS.LE.0.OR.IRS.GE.5)GO TO 10                                 
      IF(IRS.EQ.1)THEN
30    WRITE(*,40)                                                       .C.
40    FORMAT(//,
     1' Do you want to run a deflection analysis also? (y/n)')          .C.
      READ (*,ERR=30,FMT='(A)')QUES                                     .C.
      IF(QUES.EQ.'y')QUES='Y'
      IF(QUES.EQ.'Y')IDEF=1
50    WRITE(*,60)                                                       .C.
60    FORMAT(//,
     1' Do you want to run a pick point analysis also? (y/n)')          .C.
      READ (*,ERR=50,FMT='(A)')QUES                                     .C.
      IF(QUES.EQ.'y')QUES='Y'
      IF(QUES.EQ.'Y')IPICK=1
      END IF
70    WRITE(*,FMT='(//,'' Enter input file name'',/)')                  .C.
      READ(*,ERR=70,FMT='(A)')FLNM1                                     .C.
      IF(FLNM1.EQ.'CON')GO TO 100                                       .C.
      IF(FLNM1.EQ.'con')GO TO 100                                       .C.
      DO 80 I=1,16
      IF(FLNM1(I:I).EQ.'.')GO TO 100
80    CONTINUE
      DO 90 I=1,16
      IF(FLNM1(I:I).EQ.' ')THEN
      IE = I+3
      FLNM1(I:IE) = '.dat'
      GO TO 100
      END IF
90    CONTINUE
100   WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1           .C.
110   WRITE(*,FMT='(//,'' Enter output file name'',/)')                 .C.
      READ(*,ERR=110,FMT='(A)')FLNM2                                    .C.
      IF(FLNM2.EQ.'CON')GO TO 140                                       .C.
      IF(FLNM2.EQ.'con')GO TO 140                                       .C.
      DO 120 I=1,16
      IF(FLNM2(I:I).EQ.'.')GO TO 140
120   CONTINUE
      DO 130 I=1,16
      IF(FLNM2(I:I).EQ.' ')THEN
      IE = I+3
      FLNM2(I:IE) = '.out'
      GO TO 140
      END IF
130   CONTINUE
140   WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2        .C.
      IF(IRS.EQ.3.OR.IRS.EQ.4)THEN
      TUNIT=5
      OPEN(TUNIT,FILE=FLNM1,STATUS='OLD')
      END IF
      IF(IRS.LE.2)THEN
      OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')
      IF(IDEF.EQ.1.OR.IPICK.EQ.1.OR.IRS.EQ.2)
     1           OPEN(TUNIT,STATUS='SCRATCH')
      END IF
      OPEN (OUNIT,FILE=FLNM2,STATUS='UNKNOWN')
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6
      RETURN
      END
C
C
C
C
C
C
