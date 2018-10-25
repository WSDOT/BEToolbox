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


C     COMGIR                                                               00010
C  PROGRAM COMGIR CALCULATES BENDING STRESSES FOR THE TOP AND BOTTOM       00020
C  FLANGES OF A COMPOSITE GIRDER.                                          00030
C     CORRECTION BY C.Y. HOU, FEB. 18, 1975                               B
C     A - GARY CHRISTOPHER - REV. 8/20/73                                 A00050
C     B - NEAL CAMPBELL - PC CONVERSION - 4/15/86                         B
C     T - TERRY BEEBE   - VMS CONVERSION - JANUARY 1990                   T   
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C     Robert S. Lotze - Recompiled/Reconfigured for Win32 dll conversion 9-11/99

C Note from Robert: All lines saying "Goto 666" were added for Win32 dll conversion.
C  These lines were originally "STOP" lines, but since "STOP" does not do a clean
C  shut-down of files, I changed all these to go to lines that closed the files, then
C  exited gracefully from the program with an "End".  


C The following 9 lines added by Robert for Win32 dll conversion:
C Robert's setup for a DLL call:
C Fortran Code  establishing subroutine
	SUBROUTINE ComGir (MYINPUT, MYOUTPUT)

C Specify that the routine name is to be made available to callers of the
C DLL and that the external name should not have any prefix or suffix
!DEC$ ATTRIBUTES STDCALL,DLLEXPORT :: ComGir
C!DEC$ ATTRIBUTES DLLIMPORT :: ComGir
!DEC$ ATTRIBUTES ALIAS:'ComGir' :: ComGir

      IMPLICIT REAL*8 (A-H,P-Z)                                           B

C The following 6 lines added by Robert for Win32 dll conversion:
CInput argument	
	CHARACTER*254,INTENT(IN) :: MYINPUT
CInput argument
	CHARACTER*254,INTENT(IN) :: MYOUTPUT
!DEC$ ATTRIBUTES REFERENCE :: MYINPUT
!DEC$ ATTRIBUTES REFERENCE :: MYOUTPUT

      REAL*8  ZBF(8), ZT24(8),ZB24(8),ZT(8),ZB(8),ZT8(8),ZB8(8),ZTF(8)     00070
      INTEGER*4 ERLIST                                                    A00110
      CHARACTER*1 CARCNT                                                  B
      INTEGER*4 OUNIT, ERUNIT                                             B
      CHARACTER*8 REVDAT, TIME                                            T
      CHARACTER*11 DATE                                                   T
      CHARACTER*23 DATIME                                                 T

C	Following line added to make sure that this variable is declared
	REAL*8 INUNIT

CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC    B
      REVDAT = ' 3/27/86'                                                 B
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC    B
      ERUNIT = 9                                                          B
      INUNIT=5                                                            B
      CARCNT='1'                                                          B
      OUNIT = 3                                                           B
      ISPACE = 0                                                           00170
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                        B
C     OUNIT = (6 FOR PC)                                                  B
C             (3 FOR FILE OUTPUT)                                         T
C     ERUNIT = (3 FOR FILE OUTPUT)                                        T
C              (9 FOR MAINFRAME BATCH)                                    B
C     INUNIT = (5 FOR FILE INPUT)                                         T
C     CARCNT = ('1' FOR BATCH)                                            B
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                        B
C                                                                         B
C                                                                         B
C	The last two parameters added by Robert for Win32 dll conversion:
      CALL LOAD(OUNIT,ERUNIT,CARCNT,INUNIT,MYINPUT,MYOUTPUT)
C     CALL OSDATE(DATE)                                                   T
C     CALL GETIME(TIME)                                                   T
C      CALL LIB$DATE_TIME (DATIME)   
	CALL DATE_AND_TIME(DATIME)                                          
      TIME = DATIME(13:20)                                                T
      DATE = DATIME(1:11)                                                 T
      ERLIST=ERUNIT                                                       B
      LIST=OUNIT                                                          B
C                                                                         B
C                                                                         B
      READ(INUNIT,END=248,ERR=248,FMT=*)NOGIR,CDLC,RAD,SP,D,JK            B
      READ(INUNIT,END=248,ERR=248,FMT=*)(ZT(I),I=1,JK)                    B
      READ(INUNIT,END=248,ERR=248,FMT=*)(ZB(I),I=1,JK)                    B
      READ(INUNIT,END=248,ERR=248,FMT=*)(ZT8(I),I=1,JK)                   B
      READ(INUNIT,END=248,ERR=248,FMT=*)(ZB8(I),I=1,JK)                   B
      READ(INUNIT,END=248,ERR=248,FMT=*)(ZT24(I),I=1,JK)                  B
      READ(INUNIT,END=248,ERR=248,FMT=*)(ZB24(I),I=1,JK)                  B
      READ(INUNIT,END=248,ERR=248,FMT=*)(ZTF(I),I=1,JK)                   B
      READ(INUNIT,END=248,ERR=248,FMT=*)(ZBF(I),I=1,JK)                   B
   40 DO 245 J = 1,30                                                      CYH47
         READ(INUNIT,END=260,ERR=248,FMT=*)JJ,BMOM,BMOMAX,BMOMIN,II       B
         R = RAD                                                           00530
         BMOMC = BMOM * CDLC                                               00540
         BSTT = 12.0 * BMOM / ZT(II)                                       00550
         BSTB = 12.0 * BMOM / ZB(II)                                       00560
C                                                                          00570
C  CHECK WHETHER THE BENDING MOMENT IS POSITIVE OR NEGATIVE                00580
C                                                                          00590
         IF (BMOM .LT. 0.0) GO TO 50                                       00600
         BSTCT = 12.0 * BMOMC / ZT24(II)                                   00610
         BSTCB = 12.0 * BMOMC / ZB24(II)                                   00620
         BSTLT = 12.0 * BMOMAX / ZT8(II)                                   00630
         BSTLB = 12.0 * BMOMAX / ZB8(II)                                   00640
         BSTLLT = 12.0 * BMOMIN / ZT8(II)                                  00650
         BSTLLB = 12.0 * BMOMIN / ZB8(II)                                  00660
         GO TO 60                                                          00670
C                                                                          00680
   50    BSTCT = 12.0 * BMOMC / ZT(II)                                     00690
         BSTCB = 12.0 * BMOMC / ZB(II)                                     00700
         BSTLT = 12.0 * BMOMAX / ZT(II)                                    00710
         BSTLB = 12.0 * BMOMAX / ZB(II)                                    00720
         BSTLLT = 12.0 * BMOMIN / ZT(II)                                   00730
         BSTLLB = 12.0 * BMOMIN / ZB(II)                                   00740
C                                                                          00750
   60    BSTTF = BMOM * SP**2 / (10.0 * R * D)                             00760
         BSTBF = ((BMOM + BMOMC + BMOMAX) * SP**2) / (10.0 * R * D)        00770
         BSTB2 = ((BMOM + BMOMC + BMOMIN) * SP**2) / (10.0 * R * D)        00780
         BSTTF = 12.0 * BSTTF / ZTF(II)                                    00790
      ABSTBF=12.0*BSTBF/ZTF(II)                                           B
      ABSTB2=12.0*BSTB2/ZTF(II)                                           B
         BSTBF = 12.0 * BSTBF / ZBF(II)                                    00800
         BSTB2 = 12.0 * BSTB2 / ZBF(II)                                    00810
C                                                                          00820
         BSTT = -BSTT                                                      00830
         BSTCT = -BSTCT                                                    00840
         BSTLT = -BSTLT                                                    00850
         BSTLLT = -BSTLLT                                                  00860
         BST1 = BSTT + BSTCT                                               00870
         BST2 = BSTT + BSTCT + BSTLT                                       00880
         BST3 = BST1 + BSTLLT                                              00890
         BST = BSTB + BSTCB                                                00900
         BSTX = BST + BSTLB                                                00910
         BSTY = BST + BSTLLB                                               00920
C                                                                          00930
 1000 FORMAT(A1,//,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION    ',    T
     1  A11,'  TIME ',A8)                                                 T
 1001 FORMAT('        *** COMPOSITE CURVED GIRDER ***    REV ',A8,/)      B
 1002 FORMAT('             CURB DL/GIR. DL = ',F5.3,'  CURVE RAD. = ',    B
     1F6.1,/,'             DIAPHRAGM SPA.  = ',F5.2,'  GIR. DEPTH = ',    B
     2F6.2)                                                               B
       IF(ISPACE.EQ.0)THEN                                                B
       WRITE(OUNIT,1000)CARCNT,DATE,TIME                                  B
       WRITE(OUNIT,1001)REVDAT                                            B
       WRITE(OUNIT,1002)CDLC,RAD,SP,D                                     B
       END IF                                                             B
         WRITE (LIST,70) NOGIR,JJ                                          00940
   70    FORMAT( //,' ','       GIRDER NUMBER ',                          B00950
     1     I6,'  POINT ',I2)                                              A00960
         WRITE (LIST,80) II                                                00970
   80    FORMAT('        GIRDER SECTION NUMBER ',I2,/)                    B00980
       WRITE(OUNIT,1003)                                                  B
1003   FORMAT(  '                            GIRDER ONLY       N=24       B
     1     N=8        TRANS')                                             B
         WRITE (LIST,90) ZT(II),ZT24(II),ZT8(II),ZTF(II)                   00990
   90    FORMAT(' ','       SEC MODULUS - TOP  ',4(F10.2,3X))             B01000
         WRITE (LIST,100) ZB(II),ZB24(II),ZB8(II),ZBF(II)                  01010
  100    FORMAT('        SEC MODULUS - BOT  ',4(F10.2,3X),/)              B01020
         WRITE (LIST,110) BMOM,BMOMC,BMOMAX                                01030
  110    FORMAT(' ','       DLM = ',F9.2,5X,                              B01040
     1                       'DLC = ',F9.2,5X,'LL+I = ',F9.2)             B01045
         WRITE (LIST,120) BSTT,BSTCT,BSTLT,BST2                            01050
  120    FORMAT('        FT = ',F7.2,' + ',F7.2,' + ',F7.2,' = ',F7.2)    B01060
         WRITE (LIST,130) BSTB,BSTCB,BSTLB,BSTX                            01070
  130    FORMAT('        FB = ',2(F7.2,' + '),F7.2,' = ',F7.2)            B01080
         WRITE (LIST,140) BSTTF,BSTBF                                      CYH1
  140    FORMAT('            BENDING STRESS IN FLANGE - TOP = ',F9.4,' -  B01100
     * BOTTOM = ',F9.4,/)                                                 B01110
         WRITE (LIST,110) BMOM,BMOMC,BMOMIN                                01120
         WRITE (LIST,120) BSTT,BSTCT,BSTLLT,BST3                           01130
         WRITE (LIST,130) BSTB,BSTCB,BSTLLB,BSTY                           01140
         WRITE (LIST,140) BSTTF,BSTB2                                      CYH1
C                                                                          01160
         K = 1                                                             01170
         A = BSTT + BSTCT                                                  01180
         B = BST2                                                          01190
         C = BST3                                                          01200
  150    IF ((ABS(C) - ABS(B)) .GT. 0.0) GO TO 160                         01210
         RMAX = B                                                          01220
         RA = C                                                            01230
         GO TO 170                                                         01240
  160    RMAX = C                                                          01250
         RA = B                                                            01260
  170    D1 = ABS(RMAX - RA)                                               01270
         D2 = ABS(RMAX - A)                                                01280
         IF ((D1 - D2) .GT. 0.0) GO TO 180                                 01290
         R = A / RMAX                                                      01300
         GO TO 190                                                         01310
  180    R = RA / RMAX                                                     01320
  190    GO TO (200,220), K                                                01330
  200    WRITE (LIST,210) R                                                01340
  210    FORMAT('        R IN TOP FLANGE =    ',F11.4)                    B01350
         A = BSTB + BSTCB                                                  01360
         B = BSTX                                                          01370
         C = BSTY                                                          01380
         K = 2                                                             01390
         GO TO 150                                                         01400
  220    WRITE (LIST,230) R                                                01410
  230    FORMAT('        R IN BOTTOM FLANGE = ',F11.4,/)                  B01420
         ISPACE = ISPACE + 1                                               01430
         IF (ISPACE .EQ. 2) GO TO 242                                      01440
         WRITE (LIST,240)                                                  01450
  240    FORMAT(' ',9X,11('***   '),/)                                    B01460
         GO TO 245                                                         01470
  242    ISPACE = 0                                                       B01500
  245    CONTINUE                                                          01510
      GOTO 666
  248 WRITE(ERLIST,*)' INPUT ERROR!! CHECK YOUR DATA SET!!!'               01530
  260 GOTO 666

C	The following two lines added by Robert for Win32 dll conversion.
  666	CLOSE(OUNIT)
	CLOSE(INUNIT)
      END                                                                 01570
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C                                                                         B
C     The last two parameters added by Robert for Win32 dll conversion:
      SUBROUTINE LOAD(OUNIT,ERUNIT,CARCNT,INUNIT,MYINFILE,MYOUTFILE)
C	Following line added by Robert to make sure that this variable is declared
	REAL*8 INUNIT
C	The following variables were originally set to "*16"
      CHARACTER*254 FLNM1, FLNM2                                          B
      CHARACTER*1 CARCNT                                                  B
      INTEGER OUNIT, ERUNIT                                               B
C	The following two variables added by Robert for Win32 dll conversion:
	CHARACTER*254 MYINFILE,MYOUTFILE
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

C The following section commented out by Robert for Win32 dll conversion:
C==========================================================================
C     OPEN(IIUNIT,FILE='CON')                                             T
C     OPEN(IOUNIT,FILE='CON')                                             T
C10    WRITE(*,FMT='(//,'' Enter input file name:'',/)')                   T
C           READ(*,ERR=10,FMT='(A)')FLNM1                                  T
C           IF(FLNM1.EQ.'CON')GO TO 25                                     T
C           IF(FLNM1.EQ.'con'.OR.FLNM1.EQ.'lpt1')GO TO 25                  B
C              DO 15 I=1,16                                                B
C               IF(FLNM1(I:I).EQ.'.')GO TO 25                              B
C15             CONTINUE                                                   B
C              DO 20 I=1,16                                                B
C                 IF(FLNM1(I:I).EQ.' ')THEN                                B
C                  IE = I+3                                                B
C                  FLNM1(I:IE) = '.dat'                                    B
C                  GO TO 25                                                B
C                 END IF                                                   B
C20             CONTINUE                                                   B
C25         WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1        T
C          CARCNT='1'                                                      B
C35    WRITE(*,FMT='(//,'' Enter output file name:'',/)')                  T
C           READ(*,ERR=35,FMT='(A)')FLNM2                                  T
C           IF(FLNM2.EQ.'CON')GO TO 60                                     T
C           IF(FLNM2.EQ.'con')GO TO 60                                     T
C              DO 40 I=1,16                                                B
C               IF(FLNM2(I:I).EQ.'.')GO TO 60                              B
C40             CONTINUE                                                   B
C              DO 50 I=1,16                                                B
C                 IF(FLNM2(I:I).EQ.' ')THEN                                B
C                  IE = I+3                                                B
C                  FLNM2(I:IE) = '.out'                                    B
C                  GO TO 60                                                B
C                 END IF                                                   B
C50             CONTINUE                                                   B
C60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2      T
C===========================================================================

C	The following two variables "set" by Robert for Win32 dll conversion:
	FLNM1 = MYINFILE
	FLNM2 = MYOUTFILE
          OPEN(OUNIT,FILE=FLNM2)                                          B
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')                            B
      IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5                        B
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6                        B
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6                        B
      RETURN                                                              B
      END                                                                 B
C                                                                         B
C                                                                         B
