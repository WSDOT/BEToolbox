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


C	This line commented out by Robert S. Lotze for Win32 dll conversion:
C      PROGRAM PSGDES

C Note from Robert: All lines saying "Goto 666" were added for Win32 dll conversion.
C  These lines were originally "STOP" lines, but since "STOP" does not do a clean
C  shut-down of files, I changed all these to go to lines that closed the files, then
C  exited gracefully from the program with an "End". 

C The following 8 lines added by Robert for Win32 dll conversion:
C Robert's setup for a DLL call:
C Fortran Code  establishing subroutine
	SUBROUTINE PSGDES (MYINPUT, MYOUTPUT)

C Specify that the routine name is to be made available to callers of the
C DLL and that the external name should not have any prefix or suffix
!DEC$ ATTRIBUTES STDCALL,DLLEXPORT :: PSGDES
!DEC$ ATTRIBUTES ALIAS:'PSGDES' :: PSGDES
C
C
C The following 6 lines added by Robert for Win32 dll conversion:
CInput argument	
	CHARACTER*254,INTENT(IN) :: MYINPUT
CInput argument
	CHARACTER*254,INTENT(IN) :: MYOUTPUT
!DEC$ ATTRIBUTES REFERENCE :: MYINPUT
!DEC$ ATTRIBUTES REFERENCE :: MYOUTPUT
C                                     
C
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
C
C     12/99 - 01/00 - Robert S. Lotze - Modified for Win32 dll 
C					conversion.                                                                   
C                                                                   
C                                                                   
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
C	Last two parameters added by Robert S. Lotze for Win32 dll conversion:
      CALL PARM(IRS,IDEF,IPICK,OUNIT,ERUNIT,INUNIT,TUNIT,CARCNT,K2,
     1  MYINPUT,MYOUTPUT)
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
      GOTO 666
C
   40 IF(IRS.EQ.3)THEN
C	Following line commented out by Robert S. Lotze for Win32 dll
C	  conversion.  (It writes to the screen).
C      WRITE ( * , * ) 'PSGDEF HS-25 FORM DATED 8/08/91 WAS USED'        .C.
         CALL DFLECT
         GO TO 99
         END IF
      IF(IRS.EQ.4)THEN
C	Following line commented out by Robert S. Lotze for Win32 dll
C	  conversion.  (It writes to the screen).
C      WRITE ( * , * ) 'PICKPT HS-25 FORM DATED 8/08/91 WAS USED'        .C.
         CALL PICKPT                                            
         GO TO 99
         END IF
      IF (IRS.EQ.1) THEN
C	Following line commented out by Robert S. Lotze for Win32 dll
C	  conversion.  (It writes to the screen).
C      WRITE ( * , * ) 'PSGDES HS-25 VER(1) FORM DATE 8/08/91 WAS USED'  .C.
      ELSE
C	Following line commented out by Robert S. Lotze for Win32 dll
C	  conversion.  (It writes to the screen).
C      WRITE ( * , * ) 'PSGDES HS-25 VER(2) FORM DATE 8/08/91 WAS USED'  .C.
      END IF
      CALL PREDES
99    GOTO 666
C	The following two lines added by Robert for Win32 dll conversion:
666	CLOSE(OUNIT)
	CLOSE(INUNIT)
      END       
C
C
C
C
C	Last two parameters added by Robert S. Lotze for Win32 dll conversion:
      SUBROUTINE PARM(IRS,IDEF,IPICK,OUNIT,ERUNIT,
     1                INUNIT,TUNIT,CARCNT,K2,INFILE,OUTFILE)

C	The following line modified from "*16" to "*254" by Robert S. Lotze
C		for Win32 dll conversion:
      CHARACTER*254 FLNM1, FLNM2
      CHARACTER*1 CARCNT,QUES
      CHARACTER*8 TIME,REVDAT                                           .C.
      INTEGER*4 OUNIT, ERUNIT, TUNIT
      CHARACTER*11 DATE                                                 .C.

C	This line added by Robert for Win32 dll conversion:
	CHARACTER*254 INFILE,OUTFILE

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
C	Following line changed from "3" to "999" by Robert Lotze for win32 conversion.
C	  The purpose is because it creates an unwanted file and it seems that all the 
C	  code writes to it only if it is equal to "3".
      ERUNIT=999
      TUNIT=10
      CARCNT='1'
C	The following variable was changed from "1" to "0" by
C	  Robert S. Lotze for Win32 conversion (isn't needed for the 
C	  conversion...we just didn't want the additional BUG.OUT output)
	K2=0

C      OPEN(2,FILE='BUG.OUT',STATUS='NEW')
      IRS=2
C	The following was changed from "0" to "1" by Robert S. Lotze
C	  for Win32 dll conversion:  (or perhaps from 1 to 0?)
	IPICK=0
	IDEF=0

C	Following two lines added by Robert Lotze for win32 dll conversion:
	FLNM1=INFILE
	FLNM2=OUTFILE

C	This big-ol' comment block was placed here by Robert S. Lotze for
C	  Win32 dll conversion (needed to get rid of the interactive-ness):
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C10    WRITE(*,20)                                                       .C.   
C20    FORMAT(/////,
C     1' Prestressed Girder Design Program',//,                          .C.
C     2'    1- Prestressed Girder Design (short version) ',/,            .C.
C     3'    2- Prestressed Girder Design (long version)',/,              .C.
C     4'    3- Prestressed Girder Deflection Analysis',/,                .C.
C     5'    4- Pick Point Analysis',//,                                  .C.
C     6' Enter Choice: ',/)                                              .C.
C      READ(*,ERR=10,FMT=*)IRS                                           .C.
C      IF(IRS.LE.0.OR.IRS.GE.5)GO TO 10                                 
C      IF(IRS.EQ.1)THEN
C30    WRITE(*,40)                                                       .C.
C40    FORMAT(//,
C     1' Do you want to run a deflection analysis also? (y/n)')          .C.
C      READ (*,ERR=30,FMT='(A)')QUES                                     .C.
C      IF(QUES.EQ.'y')QUES='Y'
C      IF(QUES.EQ.'Y')IDEF=1
C50    WRITE(*,60)                                                       .C.
C60    FORMAT(//,
C     1' Do you want to run a pick point analysis also? (y/n)')          .C.
C      READ (*,ERR=50,FMT='(A)')QUES                                     .C.
C      IF(QUES.EQ.'y')QUES='Y'
C      IF(QUES.EQ.'Y')IPICK=1
C      END IF
C70    WRITE(*,FMT='(//,'' Enter input file name'',/)')                  .C.
C      READ(*,ERR=70,FMT='(A)')FLNM1                                     .C.
C      IF(FLNM1.EQ.'CON')GO TO 100                                       .C.
C      IF(FLNM1.EQ.'con')GO TO 100                                       .C.
C      DO 80 I=1,16
C      IF(FLNM1(I:I).EQ.'.')GO TO 100
C80    CONTINUE
C      DO 90 I=1,16
C      IF(FLNM1(I:I).EQ.' ')THEN
C      IE = I+3
C      FLNM1(I:IE) = '.dat'
C      GO TO 100
C      END IF
C90    CONTINUE
C100   WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1           .C.
C110   WRITE(*,FMT='(//,'' Enter output file name'',/)')                 .C.
C      READ(*,ERR=110,FMT='(A)')FLNM2                                    .C.
C      IF(FLNM2.EQ.'CON')GO TO 140                                       .C.
C      IF(FLNM2.EQ.'con')GO TO 140                                       .C.
C      DO 120 I=1,16
C      IF(FLNM2(I:I).EQ.'.')GO TO 140
C120   CONTINUE
C      DO 130 I=1,16
C      IF(FLNM2(I:I).EQ.' ')THEN
C      IE = I+3
C      FLNM2(I:IE) = '.out'
C      GO TO 140
C      END IF
C130   CONTINUE
C140   WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2        .C.
C      IF(IRS.EQ.3.OR.IRS.EQ.4)THEN
C      TUNIT=5
C      OPEN(TUNIT,FILE=FLNM1,STATUS='OLD')
C      END IF

C	 The following "IF" was commented out by Robert S. Lotze for 
C	   Win32 dll conversion:
C      IF(IRS.LE.2)THEN
      OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')

C	 The following If block was replaced by the singular line
C	   added by RSL below it.
C      IF(IDEF.EQ.1.OR.IPICK.EQ.1.OR.IRS.EQ.2)
C     1           OPEN(TUNIT,STATUS='SCRATCH')
C      END IF

C	 Added by Robert S. Lotze for Win32 Conversion:
       OPEN(TUNIT,STATUS='SCRATCH')

      OPEN (OUNIT,FILE=FLNM2,STATUS='UNKNOWN')
C      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      RETURN
      END
C
C
C
C
C
C