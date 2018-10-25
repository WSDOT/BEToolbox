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


C     PROGRAM GPILE -- DEVELOPED BY NEAL J CAMPBELL ON THE AT FROM THE
C     ORIGINAL PROGRAM :
C     J E BOWLES  --  3-DIMENSIONAL PILE GROUP ANALYSIS
C
C     REVISION LOG:
C
C     T - TERRY BEEBE - VMS CONVERSION - JANUARY 1990                     T
C     R - RICK BRICE - CHANGE PROGRAM TO QUAD PRECESION TO HANDLE         R
C                      VERY LARGE NUMBERS. 12/15/92                       R
C         AFTER THE CHANGE TO QUAD PRECESION, THE FOLLOWING LINES STARTED
C         PRODUCING RUNTIME ERRORS.
C               WRITE(OUNIT,1000)DATE,TIME,NPAGE
C               WRITE(OUNIT,1010)REVDAT
C         FOR SOME REASON, THIS PRODUCES AN ACCESS VIOLATOIN ERROR AT 
C         RUN TIME. THIS WILL NEED TO BE LOOKED INTO MORE CLOSELY AT
C         A LATER DATE.
C
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/26/93
C     RICHARD BRICE - RECOMPILED FOR FORTRAN LIBRARY UPDATE 1/18/95
C     Robert S. Lotze - Recompiled/Reconfigured for Win32 dll conversion 12/99

C
C     SIGNS ARE:  +FX = TO RT,  + FY = UP   + FZ = OUT OF PLANE OF PAPER
C                 +FW IS AT 90 DEG TO THE BATTER
C                 +FU IS DOWN ALONG THE PILE AXIS
C                 +FV DEPENDS ON BETA AND IS PARALLEL TO NEG Z AXIS
C                  WHEN BETA = 0
C    + MOMENT ACCORDING TO RT HAND RULE
C    + MOMENT ACCORDING TO RT HAND RULE USING FORCE DIRECTIONS FOR THUMB
C     BETA = ANGLE OF PILE PROJECTION WITH X-AXIS IN XZ PLANE
C
C     BATTER = PILE BATTER  H:12  WITH 12 = VERTICAL AND H = HORIZONTAL.
C     USE BATTER = 0. FOR VERTICAL PILES -- NOTE THAT BETA DEFINES PILE
C     BATTER DIRECTION.
C
C     BETA = 0. DEG FOR VERT. PILES OR PILES BATTERED PARALLEL TO X-AXIS
C
C     ** NOTE THAT AS BETA INCREASES FROM ZERO DEG TO 360 DEG THAT
C        POSITIVE DIRECTIONS OF FW AND FV ALSO ROTATE
C
C
C     OLVL = 1 OUTPUT ALL MATRIX INFORMATION
C     OLVL = 0 NORMAL OUTPUT
C

C Note from Robert: All lines saying "Goto 666" were added for Win32 dll conversion.
C  These lines were originally "STOP" lines, but since "F" does not do a clean
C  shut-down of files, I changed all these to go to lines that closed the files, then
C  exited gracefully from the program with an "End".  


C The following 8 lines added by Robert for Win32 dll conversion:
C Robert's setup for a DLL call:
C Fortran Code  establishing subroutine
	SUBROUTINE GPile (MYINPUT, MYOUTPUT)

C Specify that the routine name is to be made available to callers of the
C DLL and that the external name should not have any prefix or suffix
!DEC$ ATTRIBUTES STDCALL,DLLEXPORT :: GPile
!DEC$ ATTRIBUTES ALIAS:'GPile' :: GPile
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
      INTEGER*4 OUNIT, ERUNIT, OLVL, NTYP(100)
      CHARACTER*8 REVDAT, TIME                                            T
      CHARACTER*11 DATE                                                   T
      CHARACTER*23 DATIME                                                 T  
      CHARACTER*80 TITLE
      REAL*8    XF(6), BETA(100),SUM(6), COMP(100,6),C(100,10),X(100),    R
     1 Y(100),Z(100),BATTER(100),A(6,6), S(6,6), SAT(6,6),ASAT(6,6),      R
     2 P(6), ZZ(6), YY(6),XPILE(6),IASAT(6,6),SRSSV(100),SRSSM(100),      R
     3 F(100,6), TEMP(6) ,DET                                             R
C
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C     OUNIT = (6 FOR FILE OUTPUT)
C     ERUNIT = (3 TO SCREEN)
C     ERUNIT = (6 IF OUTPUT TO CON OR SCREEN)
C     INUNIT = (5 FOR FILE INPUT)
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C
      ERUNIT = 3
      INUNIT = 5
      OUNIT  = 6
      INDEX = 0
      ITEST = 0
      NPAGE = 1
      OLVL = 0
      CALL PARM(ERUNIT,MYINPUT,MYOUTPUT)
C      CALL LIB$DATE_TIME (DATIME)                                         T
	CALL DATE_AND_TIME (DATIME)
      TIME = DATIME(13:20)                                                T
      DATE = DATIME(1:11)                                                 T 
C
C
C      IF(ERUNIT.EQ.3)WRITE(OUNIT,'('' '')')                            T
C
 1000 FORMAT('1',/,7X,' WASHINGTON STATE DEPT. OF TRANSPORTATION  ',      T
     1  A11,'  TIME ',A8,' PAGE',I3)                                      T
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
      REVDAT = '01/30/90'                                                 T
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
 1010 FORMAT('        *** GROUP PILE ANALYSIS ***          REV '
     1,A8,/)
C      WRITE(OUNIT,1000)DATE,TIME,NPAGE
C      WRITE(OUNIT,1010)REVDAT
      READ(INUNIT,1020,END=998,ERR=998)TITLE
 1020 FORMAT(A)
      WRITE(OUNIT,1030)TITLE
 1030 FORMAT(8X,A80,/)
      READ(INUNIT,ERR=998,END=997,FMT=*)NPILES,NTYPE,OLVL
      WRITE(OUNIT,1040)
 1040 FORMAT(8X,'GENERAL INPUT DATA',//,T17,'PILE NO',5X, 'X',9X,'Y',9X
     1,'Z', 7X,'BETA', 6X,'BATTER    TYPE')
      DO 10 I = 1, NPILES
      READ(INUNIT,ERR=998,END=997,FMT=*)
     1           X(I),Y(I),Z(I),BETA(I),BATTER(I),NTYP(I)
      IF(I .EQ. 51)THEN
      NPAGE = NPAGE + 1
C      WRITE(OUNIT,1000)DATE,TIME,NPAGE
C      WRITE(OUNIT,1010)REVDAT
      WRITE(OUNIT,1030)TITLE
      WRITE(OUNIT,1040)
      END IF
   10 WRITE(OUNIT,1050)I,X(I),Y(I),Z(I),BETA(I),BATTER(I),NTYP(I)
 1050 FORMAT(19X,I2,5(3X,F7.2),I7)
      DO 20 J = 1, NTYPE
   20 READ(INUNIT,ERR=998,END=997,FMT=*)K,(C(K,I),I=1,10)
      ILINE = 11 + NPILES
      IF ( NPILES .GE. 51 ) ILINE = ILINE - 50
      IPAGE = 5 - ( 63 - ILINE ) / 10
      IF ( 5 - IPAGE .LT. NTYPE ) THEN
           IPAGE = 4
      ELSE
           WRITE(OUNIT,1060)
 1060      FORMAT(2X,//,T17,'THE PILE SPRING MATRICES ARE:')
      END IF
      DO 30 I = 1, NTYPE
           IPAGE = IPAGE + 1
      IF (IPAGE .EQ. 5 ) THEN
      NPAGE = NPAGE + 1
C           WRITE(OUNIT,1000)DATE,TIME,NPAGE
C           WRITE(OUNIT,1010)REVDAT
           WRITE(OUNIT,1030)TITLE
           WRITE(OUNIT,1070)
 1070 FORMAT(8X,'GENERAL INPUT DATA',//,T17,'THE PILE SPRING MATRICES AR
     1E:')
           IPAGE = 0
      END IF
      WRITE(OUNIT,1080)I
 1080 FORMAT(1X,/,T17,'PILE TYPE',I3,//,T26,'Fu',8X,'Fv',8X,'Fw',
     1          8X,'Mu',8X,'Mv',8X,'Mw')
  30  WRITE(OUNIT,1090)C(I,1),C(I,2),C(I,7),C(I,3),C(I,8),C(I,4),
     1                   C(I,9),C(I,5),C(I,10),C(I,6)
 1090 FORMAT(T17,' Îu  ',E10.3,5('    0.0   '),/,
     2 T17,' Îv      0.0   ',E10.3,3('    0.0   '),E10.3,/,
     3 T17,' Îw  ',2('    0.0   '),E10.3,'    0.0   ',E10.3,'    0.0   '
     4,/,T17,' Èu  ',3('    0.0   '),E10.3,2('    0.0   '),/,
     5 T17,' Èv  ',2('    0.0   '),E10.3,'    0.0   ',E10.3,'    0.0   '
     6,/,T17,' Èw      0.0   ',E10.3,3('    0.0   '),E10.3)
      DO  40  I = 1,6
      P(I) = 0.
      SUM(I) = 0.
      DO  40  J = 1,6
   40 ASAT(I,J) = 0.
      RAD = 180./3.1415927
C***********
C     START LOOP DEVELOPING REQ'D MATRICES
C***********
      I = 1
      IPAGE = 4
   50 BETAR = BETA(I)/RAD
      SINB = SIN(BETAR)
      COSB = COS(BETAR)
      SLOPE = SQRT(144.+BATTER(I)**2)
      COSA = BATTER(I)/SLOPE
      SINA = 12./SLOPE
      IF(BATTER(I).EQ.0.) SINA = 1.0
C     BUILD A-MATRIX
      DO 60  K = 1,6
      DO 60  J = 1,6
   60 A(K,J) = 0.
      A(1,1) = COSA*COSB
      A(1,2) = SINB
      A(1,3) = SINA*COSB
      A(2,1) = -SINA
      A(2,3) = COSA
      A(3,1) = COSA*SINB
      A(3,2) = -COSB
      A(3,3) = SINA*SINB
      A(4,1) = Z(I)*SINA + Y(I)*COSA*SINB
      A(4,2) = -Y(I)*COSB
      A(4,3) = -Z(I)*COSA + Y(I)*SINA*SINB
      A(4,4) = A(1,1)
      A(4,5) = A(1,2)
      A(4,6) = A(1,3)
      A(5,1) = Z(I)*A(1,1) - X(I)*A(3,1)
      A(5,2) = Z(I)*SINB + X(I)*COSB
      A(5,3) = Z(I)*A(1,3) - X(I)*A(3,3)
      A(5,4) = A(2,1)
      A(5,6) = A(2,3)
      A(6,1) = -Y(I)*A(1,1) - X(I)*SINA
      A(6,2) = -Y(I)*A(1,2)
      A(6,3) = -Y(I)*A(1,3) + X(I)*COSA
      A(6,4) = A(3,1)
      A(6,5) = A(3,2)
      A(6,6) = A(3,3)
C     BUILD S-MATRIX
      DO 70  K = 1,6
      DO 70  J = 1,6
   70 S(K,J) = 0.
      S(1,1) = C(NTYP(I),1)
      S(2,2) = C(NTYP(I),2)
      S(3,3) = C(NTYP(I),3)
      S(4,4) = C(NTYP(I),4)
      S(5,5) = C(NTYP(I),5)
      S(6,6) = C(NTYP(I),6)
      S(2,6) = C(NTYP(I),7)
      S(3,5) = C(NTYP(I),8)
      S(5,3) = C(NTYP(I),9)
      S(6,2) = C(NTYP(I),10)
      IF( INDEX .GT. 0 ) GO TO 160
      DO 80  K = 1,6
      DO 80  L = 1,6
      SAT(K,L) = 0.
      DO 80  M = 1,6
   80 SAT(K,L) = SAT(K,L) + S(K,M)*A(L,M)
      IF (OLVL .EQ. 0 ) GO TO 90
           IPAGE = IPAGE + 1
      IF (IPAGE .EQ. 5 ) THEN
           NPAGE = NPAGE + 1
C           WRITE(OUNIT,1000)DATE,TIME,NPAGE
C           WRITE(OUNIT,1010)REVDAT
           WRITE(OUNIT,1030)TITLE
           WRITE(OUNIT,1100)
 1100      FORMAT(8X,'INDIVIDUAL PILE MATRICES:')
           IPAGE = 0
      END IF
      WRITE(OUNIT,1110)I
 1110 FORMAT( //,T17,' THE A-MATRIX FOR PILE NO',I3,' ***', T69, 'THE SA
     1T MATRIX--100 FACTORED')
      WRITE(OUNIT,1120)((A(M,K), K = 1,6),(SAT(M,J), J = 1,6),M=1,6)
 1120 FORMAT(T17,0P6F10.4,1X,-2P6F9.1)
   90 DO 100 K = 1,6
      DO 100 L = 1,6
      DO 100 M = 1,6
  100 ASAT(K,L) = ASAT(K,L) + A(K,M)*SAT(M,L)
      I = I + 1
      DO 110 J = 1,6
      DO 110 K = 1,6
  110 IASAT(J,K)=ASAT(J,K)
      IF(I.LE.NPILES)GO TO 50
C     END OF ASAT FORMATION
C     CONVERT GLOBAL GROUP STIFFNESS MATRIX TO
C     LOCAL GROUP STIFFNESS MATRIX
C
      DO 120 I= 1 , 6
           SAT(I,1) =  ASAT(I,2)
           SAT(I,2) = -ASAT(I,1)
           SAT(I,3) =  ASAT(I,3)
           SAT(I,4) =  ASAT(I,5)
           SAT(I,5) = -ASAT(I,4)
  120      SAT(I,6) =  ASAT(I,6)
      DO 130 I= 1 , 6
           TEMP(I)  = -SAT(1,I)
           SAT(1,I) =  SAT(2,I)
           SAT(2,I) =  TEMP(I)
           TEMP(I)  = -SAT(4,I)
           SAT(4,I) =  SAT(5,I)
  130      SAT(5,I) =  TEMP(I)
C
C     [SAT] IS NOW THE LOCAL GROUP STIFFNESS ASSUMING LOCAL +X IS UP
C           (SAME AS GLOBAL +Y) AND LOCAL +Z IS SAME AS GLOBAL +Z.
C           (THEREFORE LOCAL +Y IS SAME AS GLOBAL -X)
C
C     [ASAT] IS NOW THE GLOBAL GROUP STIFFNESS
C
      IF( OLVL .GT. 0 ) THEN
           NPAGE = NPAGE + 1
C           WRITE(OUNIT,1000)DATE,TIME,NPAGE
C           WRITE(OUNIT,1010)REVDAT
           WRITE(OUNIT,1030)TITLE
           WRITE(OUNIT,1130)
 1130      FORMAT(8X,'GLOBAL GROUP STIFFNESS MATRIX:',//,T28,'FX',10X
     1        ,'FY',10X,'FZ',10X,'MX',10X,'MY',10X,'MZ')
           WRITE(OUNIT,1140)((ASAT(I,J),I=1,6),J=1,6)
 1140      FORMAT(T17,' ÎX ',6(2X,E10.3),/,T17,' ÎY ',6(2X,E10.3),/,
     1         T17,' ÎZ ',6(2X,E10.3)
     2         ,/,T17,' ÈX ',6(2X,E10.3),/,T17,' ÈY ',6(2X,E10.3),/,
     3         T17,' ÈZ ',6(2X,E10.3),//)
           WRITE(OUNIT,1150)
 1150      FORMAT(8X,'LOCAL GROUP STIFFNESS MATRIX:',//,T28,'FX',10X
     1        ,'FY',10X,'FZ',10X,'MX',10X,'MY',10X,'MZ')
           WRITE(OUNIT,1140)((SAT(I,J),I=1,6),J=1,6)
      END IF
C
C     BUILD P-MATRIX FOR NEW LOAD
C
  140 READ(INUNIT,END=999,ERR=998,FMT=*)LDNO,(P(J),J=1,6)
      NPAGE = NPAGE + 1
C      WRITE(OUNIT,1000)DATE,TIME,NPAGE
C      WRITE(OUNIT,1010)REVDAT
      WRITE(OUNIT,1030)TITLE
      WRITE(OUNIT,1160)LDNO,(P(I),I=1,6)
 1160 FORMAT(T17,' LOADING NO. ',I3,':',//,
     1       T17,   5X,'FX = ',F12.2,/,T17,    5X,'FY = ',
     2   F12.2,/, T17,    5X,'FZ = ',F12.2,
     3   /,T17,   5X,'MX = ',F12.2,/,T17,   5X
     4,'MY = ',F12.2,/, T17,   5X, 'MZ = ',F12.2)
C     **  INVERT ASAT MATRIX USING IBM STANDARD INVERSION SUBROUTINE  **
C            ON FIRST LOAD CASE ONLY
      IF ( ITEST .EQ. 0 ) THEN
      CALL MINV(IASAT,6,DET,ZZ,YY)
      ITEST = 1
      END IF
C     COMPUTE FOUNDATION DISPLACEMENTS XF
      DO 150  K = 1,6
      XF(K) = 0.
      DO 150  J = 1,6
  150 XF(K) = XF(K) + IASAT(K,J)*P(J)
      WRITE(OUNIT,1170)(XF(J),J=1,6)
 1170 FORMAT(//,T17,' THE FOUNDATION DISPLACEMENTS ARE:',/,T20,'ÎX =',
     1F9.6,2X,'ÎY=',F9.6,2X,'ÎZ=',F9.6,2X, 'ÈX=',F9.6,2X,
     1'ÈY=', F9.6, 2X, 'ÈZ=', F9.6)
      WRITE(OUNIT,1180)
 1180 FORMAT(//,T18,'THE PILE DISPLACEMENTS ARE:',
     1                    /,T18,'PILE',4X,'Îu',
     2 6X,'Îv',6X,'Îw',6X,'Èu',6X,'Èv',6X,'Èw')
      ILINE = 22
      INDEX = INDEX + 1
      I = 1
      GO TO 50
  160 DO 170  J = 1,6
      XPILE(J) = 0.
      DO 170  K = 1,6
      XPILE(J) = XPILE(J) + A(K,J)*XF(K)
  170 CONTINUE
      ILINE = ILINE + 1
      IF( ILINE .GT. 60 ) THEN
           NPAGE = NPAGE + 1
C           WRITE(OUNIT,1000)DATE,TIME,NPAGE
C           WRITE(OUNIT,1010)REVDAT
           WRITE(OUNIT,1030)TITLE
           WRITE(OUNIT,1190)LDNO
           ILINE = 13
 1190      FORMAT(T17,' LOADING NO. ',I3,':',//,
     1          T18,'PILE DISPLACEMENTS (CONT.):',
     2           /,T18,'PILE',4X,'Îu',
     3          6X,'Îv',6X,'Îw',6X,'Èu',6X,'Èv',6X,'Èw')
      END IF
      WRITE(OUNIT,1200)I,(XPILE(K),K=1,6)
 1200 FORMAT(T19,I2,2X,F7.4,5(1X,F7.4))
C
C    **  COMPUTE PILE FORCES USING F = S(I)*XPILE(I)  **
C
      DO 190  J = 1,6
      F(I,J) = 0.
      DO 180  K = 1,6
  180 F(I,J) = F(I,J) + S(J,K)*XPILE(K)
      SRSSV(I) = SQRT(F(I,2)*F(I,2)+F(I,3)*F(I,3))
      SRSSM(I) = SQRT(F(I,5)*F(I,5)+F(I,6)*F(I,6))
  190 CONTINUE
C  ** COMPUTE X,Y,Z FORCE COMPONENTS OF INDIVIDUAL PILES AND SUM AS
C       STATICS CHECK OF SOLUTION  *******
      DO 210  K = 1,6
      COMP(I,K) = 0.
      DO 200  J = 1,6
  200 COMP(I,K) = COMP(I,K) + A(K,J)*F(I,J)
      SUM(K) = SUM(K) + COMP(I,K)
  210 CONTINUE
      I = I+1
      IF(I.LE.NPILES)GO TO 50
      ILINE = ILINE + 5
      IF ( 60 - ILINE .LT. NPILES ) THEN
           NPAGE = NPAGE + 1
C           WRITE(OUNIT,1000)DATE,TIME,NPAGE
C           WRITE(OUNIT,1010)REVDAT
           WRITE(OUNIT,1030)TITLE
           WRITE(OUNIT,1210)LDNO
 1210      FORMAT(T17,' LOADING NO. ',I3,':')
           ILINE = 13
      END IF
      WRITE(OUNIT,1220)
 1220 FORMAT(//,T18,'THE PILE FORCES ARE:     ( NOTE: Fu, Fv, Fw, etc. a
     1re acting on the cap.)',/,T18,'PILE',6X,
     2    'Fu',10X,'Fv',10X,'Fw',10X,'Mu',10X,'Mv',10X,'Mw',8X,           R
     3'F (SRSS)   M (SRSS)')                                              R
      DO 220 I = 1,NPILES                                                 
      ILINE = ILINE + 1
      IF ( ILINE .GT. 60 ) THEN
           NPAGE = NPAGE + 1
C           WRITE(OUNIT,1000)DATE,TIME,NPAGE
C           WRITE(OUNIT,1010)REVDAT
           WRITE(OUNIT,1030)TITLE
           WRITE(OUNIT,1230)LDNO
           ILINE = 13
 1230      FORMAT(T17,' LOADING NO. ',I3,':',//,
     1          T18,'PILE FORCES (CONT.):',/,T18,'PILE',6X,               R
     2    'Fu',10X,'Fv',10X,'Fw',10X,'Mu',10X,'Mv',10X,'Mw',8X,           R
     3'F (SRSS)   M (SRSS)')                                              R
      END IF
  220 WRITE(OUNIT,1240)I,(F(I,L),L=1,6),SRSSV(I),SRSSM(I)
 1240 FORMAT(18X,I2,2X,F10.2,7(2X,F10.2))
      ILINE = ILINE + 5
      IF ( 60 - ILINE .LT. NPILES ) THEN
           NPAGE = NPAGE + 1
C           WRITE(OUNIT,1000)DATE,TIME,NPAGE
C           WRITE(OUNIT,1010)REVDAT
           WRITE(OUNIT,1030)TITLE
           WRITE(OUNIT,1210)LDNO
           ILINE = 13
      END IF
      WRITE(OUNIT,1250)
 1250 FORMAT(//,T17, ' INDIVIDUAL PILE FORCE COMPONENTS TO CHECK SUM OF
     1FORCES ALONG AXES' ,/,T18,'PILE NO',6X,'FX',10X,'FY',10X,'FZ',10X,
     2 'MX',10X,'MY',10X,'MZ')
      DO 230 I = 1,NPILES
      ILINE = ILINE + 1
      IF ( ILINE .GT. 60 ) THEN
           NPAGE = NPAGE + 1
C           WRITE(OUNIT,1000)DATE,TIME,NPAGE
C           WRITE(OUNIT,1010)REVDAT
           WRITE(OUNIT,1030)TITLE
           WRITE(OUNIT,1260)LDNO
           ILINE = 13
 1260      FORMAT(T17,' LOADING NO. ',I3,':',//,
     1          T17, ' INDIVIDUAL PILE FORCE COMPONENTS (CONT.):'
     2          ,/,T18,'PILE NO',6X,'FX',10X,'FY',10X,'FZ',10X,
     3          'MX',10X,'MY',10X,'MZ')
      END IF
  230 WRITE(OUNIT,1270)I,(COMP(I,K),K=1,6)
 1270 FORMAT(T18,I3,3X,6F12.4)
      WRITE(OUNIT,1280)(SUM(I),I=1,6),(P(N),N=1,6)
 1280 FORMAT(T16,'    ‰ =',1X,6F12.4,/,T24,'(',F10.3,')(',F10.3,')(',F10
     1.3,')(',F10.3,')(',F10.3,')(',F10.3,')')
      DO 240 I = 1,6
  240 SUM(I) = 0.
      GO TO 140
C	Below console write commented out and replaced without an output file write
C  997 WRITE(*,*)'   PREMATURE END OF DATA FILE ENCOUNTERED, CHECK Y       T
C     1OUR FILE.'
  997 WRITE(OUNIT,*)'   PREMATURE END OF DATA FILE ENCOUNTERED, CHECK Y       T
     1OUR FILE.'
      WRITE(OUNIT,*)'   PREMATURE END OF DATA FILE ENCOUNTERED, CHECK YO
     1UR FILE.'
      GO TO 999
C	Below console write commented out and replaced without an output file write
C  998 WRITE(*,*)'   READ ERROR, CHECK YOUR DATA FILE.'                    T
  998 WRITE(OUNIT,*)'   READ ERROR, CHECK YOUR DATA FILE.'                    T
      WRITE(OUNIT,*)'   READ ERROR, CHECK YOUR DATA FILE.'
  999 IF(ERUNIT.EQ.3)WRITE(OUNIT,'('' '')')
      WRITE(OUNIT,*)'.'
      GOTO 666
C	The following two lines added by Robert for Win32 dll conversion:
666	CLOSE(OUNIT)
	CLOSE(INUNIT)
      END
C
C
C
C
C
C     ..................................................................
C
C        SUBROUTINE MINV
C
C        PURPOSE
C        INVERT A MATRIX
C
C        USAGE
C           CALL MINV(A,N,D,L,M)
C
C        DESCRIPTION OF PARAMETERS
C           A - INPUT MATRIX, DESTROYED IN COMPUTATION AND REPLACED BY
C            RESULTANT INVERSE.
C           N - ORDER OF MATRIX A
C           D - RESULTANT DETERMINANT
C           L - WORK VECTOR OF LENGTH N
C           M - WORK VECTOR OF LENGTH N
C
C        REMARKS
C           MATRIX A MUST BE A GENERAL MATRIX
C
C        SUBROUTINES AND FUNCTION SUBPROGRAMS REQUIRED
C           NONE
C
C        METHOD
C           THE STANDARD GAUSS-JORDAN METHOD IS USED. THE DETERMINANT
C           IS ALSO CALCULATED. A DETERMINANT OF ZERO INDICATES THAT
C           THE MATRIX IS SINGULAR.
C
C     ..................................................................
C
      SUBROUTINE MINV(A,N,D,L,M)
      DIMENSION A(1),L(6),M(6)
C
C         ..............................................................
C
C        IF A DOUBLE PRECISION VERSION OF THIS ROUTINE IS DESIRED, THE
C        C IN COLUMN 1 SHOULD BE REMOVED FROM THE DOUBLE PRECISION
C        STATEMENT WHICH FOLLOWS.
C
C      DOUBLE PRECISION A,D,BIGA,HOLD
      REAL*8  A,D,BIGA,HOLD,L,M
C	"N" was added below to make sure that it was declared.
C	  The number passed in as "N" is "6", so this seems right.
	INTEGER N
C
C        THE C MUST ALSO BE REMOVED FROM DOUBLE PRECISION STATEMENTS
C        APPEARING IN OTHER ROUTINES USED IN CONJUNCTION WITH THIS
C        ROUTINE.
C
C        THE DOUBLE PRECISION VERSION OF THIS SUBROUTINE MUST ALSO
C        CONTAIN DOUBLE PRECISION FORTRAN FUNCTIONS.  ABS IN STATEMENT
C        10 MUST BE CHANGED TO DABS.
C
C        ...............................................................
C
C        SEARCH FOR LARGEST ELEMENT
C
      D=1.0
      NK=-N
      DO 80 K=1,N
      NK=NK+N
      L(K)=K
      M(K)=K
      KK=NK+K
      BIGA=A(KK)
      DO 20 J=K,N
      IZ=N*(J-1)
      DO 20 I=K,N
      IJ=IZ+I
   10 IF( DABS(BIGA)- DABS(A(IJ))) 15,20,20                                R
   15 BIGA=A(IJ)
      L(K)=I
      M(K)=J
   20 CONTINUE
C
C        INTERCHANGE ROWS
C
      J=L(K)
      IF(J-K) 35,35,25
   25 KI=K-N
      DO 30 I=1,N
      KI=KI+N
      HOLD=-A(KI)
      JI=KI-K+J
      A(KI)=A(JI)
   30 A(JI) =HOLD
C
C        INTERCHANGE COLUMNS
C
   35 I=M(K)
      IF(I-K) 45,45,38
   38 JP=N*(I-1)
      DO 40 J=1,N
      JK=NK+J
      JI=JP+J
      HOLD=-A(JK)
      A(JK)=A(JI)
   40 A(JI) =HOLD
C
C        DIVIDE COLUMN BY MINUS PIVOT (VALUE OF PIVOT ELEMENT IS
C        CONTAINED IN BIGA)
C
   45 IF(BIGA) 48,46,48
   46 D=0.0
      RETURN
   48 DO 55 I=1,N
      IF(I-K) 50,55,50
   50 IK=NK+I
      A(IK)=A(IK)/(-BIGA)
   55 CONTINUE
C
C        REDUCE MATRIX
C
      DO 65 I=1,N
      IK=NK+I
      HOLD=A(IK)
      IJ=I-N
      DO 65 J=1,N
      IJ=IJ+N
      IF(I-K) 60,65,60
   60 IF(J-K) 62,65,62
   62 KJ=IJ-I+K
      A(IJ)=HOLD*A(KJ)+A(IJ)
   65 CONTINUE
C
C        DIVIDE ROW BY PIVOT
C
      KJ=K-N
      DO 75 J=1,N
      KJ=KJ+N
      IF(J-K) 70,75,70
   70 A(KJ)=A(KJ)/BIGA
   75 CONTINUE
C
C        PRODUCT OF PIVOTS
C
      D=D*BIGA
C
C        REPLACE PIVOT BY RECIPROCAL
C
      A(KK)=1.0/BIGA
   80 CONTINUE
C
C        FINAL ROW AND COLUMN INTERCHANGE
C
      K=N
  100 K=(K-1)
      IF(K) 150,150,105
  105 I=L(K)
      IF(I-K) 120,120,108
  108 JQ=N*(K-1)
      JR=N*(I-1)
      DO 110 J=1,N
      JK=JQ+J
      HOLD=A(JK)
      JI=JR+J
      A(JK)=-A(JI)
  110 A(JI) =HOLD
  120 J=M(K)
      IF(J-K) 100,100,125
  125 KI=K-N
      DO 130 I=1,N
      KI=KI+N
      HOLD=A(KI)
      JI=KI-K+J
      A(KI)=-A(JI)
  130 A(JI) =HOLD
      GO TO 100
  150 RETURN
      END
C
C
C
C
C
C	Last two parameters added for win32 dll conversion:
      SUBROUTINE PARM(ERUNIT,INPUTFILE,OUTPUTFILE)
C	Next line added by Robert for win32 dll conversion:
      CHARACTER*254 INPUTFILE, OUTPUTFILE
C	Changed from "*16" to "*254" by Robert for win32 dll conversion:
	CHARACTER*254 FLNM1, FLNM2
      INTEGER OUNIT, ERUNIT
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  STANDARD REPORT OUTPUT UNIT
      OUNIT=6
C  STANDARD FILE INPUT UNIT
      INUNIT=5
C  STANDARD ERROR OUTPUT UNIT
      ERUNIT=3
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C	The following section commented out by Robert for Win32 dll conversion:
C========================================================================
C10    WRITE(*,210)
C210   FORMAT(//,
C     1' Enter input file name:',/)
C           READ(*,ERR=10,FMT='(A)')FLNM1
C           IF(FLNM1.EQ.'CON')GO TO 25                                     T
C           IF(FLNM1.EQ.'con')GO TO 25                                     T
C              DO 15 I=1,16
C               IF(FLNM1(I:I).EQ.'.')GO TO 25
C15             CONTINUE
C              DO 20 I=1,16
C                 IF(FLNM1(I:I).EQ.' ')THEN
C                  IE = I+3
C                  FLNM1(I:IE) = '.dat'
C                  GO TO 25
C                 END IF
C20             CONTINUE
C25         WRITE(*,FMT='(/,'' '',1X,A16,'' used for input '',//)')FLNM1   T
C35    WRITE(*,235)
C235   FORMAT(//,
C     1' Enter output file name:',/)
C           READ(*,ERR=35,FMT='(A)')FLNM2
C           IF(FLNM2.EQ.'CON')GO TO 60                                     T
C           IF(FLNM2.EQ.'con')GO TO 60                                     T  
C              DO 40 I=1,16
C               IF(FLNM2(I:I).EQ.'.')GO TO 60
C40             CONTINUE
C              DO 50 I=1,16
C                 IF(FLNM2(I:I).EQ.' ')THEN
C                  IE = I+3
C                  FLNM2(I:IE) = '.out'
C                  GO TO 60
C                 END IF
C50             CONTINUE
C60        WRITE(*,FMT='(/,'' '',1X,A16,'' used for output '',/// )')FLNM2 T
C	Next two lines added by Robert for win32 dll conversion:
CThese two lines added by Robert for Win32 dll conversion:
	FLNM1 = INPUTFILE
	FLNM2 = OUTPUTFILE
          OPEN(OUNIT,FILE=FLNM2,STATUS='NEW')
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6
      RETURN
      END
C
C
C
