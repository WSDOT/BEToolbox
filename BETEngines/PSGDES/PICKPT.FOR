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


      SUBROUTINE PICKPT
C
C
C     THIS PROGRAM IN A MODIFICATION AND
C     FORTRAN TRANSLATION OF A SPEAKEZ PROGRAM
C     WRITTEN BY KARL KIRKER AND REWRITTEN IN SEP. OF 1986 BY
C     NEAL J. CAMPBELL IN BRIDGE DESIGN.
C
C     THE PROGRAM FINDS THE MAXIMUM
C     OFFSETS FROM CENTER LINE BEARING FOR PICK POINTS ON
C     PRESTRESSED GIRDERS. THE PROGRAM IS DESIGNED TO BE LINKED
C     TO THE PRESTRESSED GIRDER DESIGN PROGRAM.
C
C     PROGRAM ASSUMES 1' OF GIRDER PAST CL BEARING.
C
C     IF TENSION CONTROLS, THE ALLOWABLE STRESSES MAY BE INCREASED
C     IF ADDITIONAL MILD STEEL IS ADDED TO THE BOTTOM OF THE GIRDER.
C
C     CURRENT CRITERIA:
C           LIFTING IN FABRICATION FACILITIES.
C                      IMPACT= 0.0
C                      ALLOW. TENS. STRESS=  3.*SQRT(F'CI*1000.)/1000.
C                                             BUT NOT MORE THAN 0.2 KSI
C                      ALLOW. COMP. STRESS=-.6*F'CI
C                      LOSSES=20 KSI.
C
C                  W/MILD STEEL REINFORCING
C
C                      ALLOW. TENS. STRESS= 7.5*SQRT(F'CI*1000.)/1000.
C
C           SHIPPING
C                      IMPACT=.20
C                      ALLOW. TENS. STRESS= 3.*SQRT(F'C*1000.)/1000.
C                      ALLOW. COMP. STRESS=-.6*F'C
C                      LOSSES=30 KSI.
C
C                  W/MILD STEEL REINFORCING
C
C                      ALLOW. TENS. STRESS= 7.5*SQRT(F'C*1000.)/1000.
C
C     REVISION LOG:
C
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 04/27/93
C
C	Robert S. Lotze - Modified for Win32 dll conversion 9/99 - 2/00
C
C
C
C     SYNOPSIS OF VARIABLE NAMES
C
C        AG       AREA OF GIRDER CROSS SECTION IN SQUARE INCHES.
C        ALSB     ALLOWABLE STRESS AT BOT. OF GIRDER (COMP.) IN KSI.
C        ALSM     ALLOWABLE STRESS AT TOP OF GD. W/EXTRA MILD STEEL.
C        ALST     ALLOWABLE STRESS AT TOP OF GIRDER (TENSION) IN KSI.
C        AS       STRAND AREA (USED IN CALCULATIONS).
C        BGUNIT   DEBUG FILE FOR TRACING OPERATIONS (2)
C        CGH      HEIGHT OF C.G. HARPED STRANDS AT CENTER LINE
C                 OF SPAN IN INCHES.
C        CGS      HEIGHT OF C.G. STRAIGHT STRANDS FROM BOTTOM IN INCHES.
C        CONTRL  CHARACTER ARRAY OF CASES
C        CSTR     .7 FOR STRESS RELIEVED AND .75 FOR LOW RELAX. STRANDS
C        DMOM     MOMENT IN K-IN THAT CAUSES STRESS LIMIT
C        FCU      28 DAY CONCRETE COMPRESSIVE STRENGTH IN KSI.
C        FHPS     COMBINED FORCE FROM HARPED STRANDS IN KIPS.
C        FICU     CONCRETE COMPRESSIVE STRENGTH AT RELEASE IN KSI.
C        FLOSS    INPUT LOSSES IN KSI EXPECTED IN PRESTRESSING.
C        FSS      COMBINED FORCE FROM STRAIGHT STRANDS IN KIPS.
C        HLN      DISTANCE FROM BUNKING PT. TO HARPING PT. IN FT.
C        INAS     INPUT STRAND AREA.
C        INUSS    INPUT ULTIMATE STRESS.
C        IMP      INPUT IMPACT FACTOR.
C        K2       1 IF BGUNIT IS ACTIVATED,  0 IF NOT
C        KSTY     1 FOR STRESS RELIEVED , 0 FOR LOW RELAXATION STRANDS
C        KTRL     INTEGER ARRAY OF CASES
C        LFTIMP   LIFTING IMPACT FACTOR (USED IN CALCULATIONS).
C        LFTLOS   LIFTING LOSSES (USED IN CALCULATIONS).
C        NHS      NUMBER OF HARPED PRESTRESSING STRANDS.
C        NSS      NUMBER OF STRAIGHT PRESTRESSING STRANDS.
C        PBM      PROBLEM NUMBER
C        PKPT     OFFSET TO PICK POINT FROM CENTER LINE OF BEARING
C                 OF THE GIRDER IN FT.
C        SHPIMP   SHIPPING IMPACT FACTOR (USED IN CALCULATIONS).
C        SHPLOS   SHIPPING LOSSES (USED IN CALCULATIONS).
C        SBOT     STRESS AT HARPING PT. AT BOT. OF GIRDER IN KSI.
C        STP      STRESS AT HARPING PT. AT TOP OF GIRDER IN KSI.
C        TITL     CHARACTER TITLE VARIABLE
C        USS      ULTIMATE STRESS (USED IN CALCULATIONS).
C        WG       WEIGHT OF GIRDER PER FT. OF LENGTH IN K/FT.
C        XHARP    DISTANCE CENTER LINE OF BEARING TO HARPING PT. IN FT.
C        XL       DISTANCE CENTER LINE OF BEARING TO CENTER LINE
C                 OF BEARING IN FT.
C        YB       DISTANCE FROM BOTTOM OF GIRDER TO SECTION N/A.
C        ZB       BOT. SECTION MODULUS OF GIRDER.
C        ZT       TOP SECTION MODULUS OF GIRDER.
C
C     INTERMEDIATE VARIABLES:
C            PTEMP,TEMB,TEMP,C1,C2
C
C     ARRAY ASSIGNMENTS:
C
C
C         PKPT(3): CHOOSEN PICK POINTS TO PRINT (WILL PRINT THE LESSER)
C         KTRL(3): CASE THAT CONTROLS (INTEGER)
C         +----------+----------+----------+
C         ] TENSILE  ]  W/STEEL ] COMPRESS ]
C         +----------+----------+----------+
C
C         ALST(5): ALLOWABLE TENSION STRESS
C         ALSM(5): ALLOWABLE TENSION STRESS WITH MILD REINF.
C         ALSB(5): ALLOWABLE COMPRESSION STRESS
C         FLOSS(5): PRESTRESSING LOSSES
C         IMP(5): IMPACT
C         CONTRL(5): CASE NAME (CHARACTER VARIABLE)
C         TEMP(4):STRESS TOP DUE TO PRESTRESSING
C         TEMB(4):STRESS BOTTOM DUE TO PRESTRESSING
C         +----------+----------+----------+----------+----------+
C         ] LIFT+IMP ] LIFT-IMP ] SHIP+IMP ] SHIP-IMP ] HARP.PT. ]
C         +----------+----------+----------+----------+----------+
C
C         STP(6) & SBOT(6): COMPUTED STRESSES
C         +---------+---------+---------+---------+---------+---------+
C         ] LIFTING ] SHIPPING] LIFTING ] SHIPPING] LIFTING ] SHIPPING]
C         ] TENSILE ] TENSILE ] W/STEEL ] W/STEEL ] COMPRESS] COMPRESS]
C         +---------+---------+---------+---------+---------+---------+
C
      REAL LFTIMP,LFTLOS,IMP(5),FLOSS(5),KTRL(3)
      REAL LOSLFT,LOSSHP,IMPLFT,IMPSHP,STP(6),SBOT(6),PKPT(3)
      REAL ALST(5),ALSB(5),ALSM(5),INAS,INUSS
      REAL TEMP(4),TEMB(4)
      CHARACTER*8 REVDAT,ACOMP,AMILD,ATEN,TIME,CONTRL(5)                .C.
      CHARACTER*11 DATE                                                 .C.
      CHARACTER*16 TITL
      CHARACTER*1 CARCNT
      LOGICAL NG
      INTEGER*4 OUNIT,ERUNIT,TUNIT,PBM,BGUNIT
      COMMON /PARMIN/ IRS,IDEF,IPICK,OUNIT,ERUNIT,INUNIT,TUNIT,K2
      COMMON /REV/ TIME,REVDAT,CARCNT,DATE                              .C.
      BGUNIT=2
C
C
1     READ(TUNIT,END=3,ERR=2200,FMT=*)LTYPE,PBM,NHS,NSS,XL,XHARP,
     1             FICU,FCU,AG,WG,ZT,ZB,YB,CGH,CGS
      IF(LTYPE.NE.1)GO TO 2200
      READ(TUNIT,END=2000,ERR=2200,FMT=*)LTYPE,PCKPT,IMPLFT,IMPSHP,
     1            LOSLFT,LOSSHP,INAS,INUSS,KSTY
      IF(LTYPE.NE.2)GO TO 2200
      GO TO 4
3     IF(IRS.NE.4)GO TO 2100
      RETURN
C
C
C
C
C     THE FOLLOWING "IF'S" SET VARIABLES TO THEIR DEFAULT
C     VALUES IF THEIR INPUT VALUES ARE "0"
C
4     IF (IMPLFT .EQ. 0) THEN
         LFTIMP=0.0
      ELSE
         LFTIMP=IMPLFT
      END IF
      IF (IMPSHP .EQ. 0) THEN
         SHPIMP=.20
      ELSE
         SHPIMP=IMPSHP
      END IF
      IF (LOSLFT .EQ. 0) THEN
         LFTLOS = 20
      ELSE
         LFTLOS=LOSLFT
      END IF
      IF (LOSSHP .EQ. 0) THEN
         SHPLOS = 35
      ELSE
         SHPLOS=LOSSHP
      END IF
      IF (INAS .EQ. 0) THEN
         AS = 0.153
      ELSE
         AS=INAS
      END IF
      IF (INUSS .EQ. 0) THEN
         USS = 270.0
      ELSE
         USS=INUSS
      END IF
      CSTR=.7
      IF(KSTY.EQ.0)CSTR=.75
      IF (FICU .EQ. 0) FICU=6.0                                         .C.
      IF (FCU .EQ. 0) FCU=7.0                                           .C.
      FLOSS(1)=LFTLOS
      FLOSS(2)=LFTLOS
      FLOSS(3)=SHPLOS
      FLOSS(4)=SHPLOS
      FLOSS(5)=SHPLOS
      CONTRL(1)='LIFT+IMP'
      CONTRL(2)='LIFT-IMP'
      CONTRL(3)='SHIP+IMP'
      CONTRL(4)='SHIP-IMP'
      CONTRL(5)='HARP.PT.'
C
      ALST(1)  =  3.*SQRT(FICU*1000.)/1000.
      IF ( ALST(1) .GT. .2 ) ALST(1) = 0.200
      ALST(2)  =  3.*SQRT(FICU*1000.)/1000.
      IF ( ALST(2) .GT. .2 ) ALST(2) = 0.200
      ALST(3)  =  3.*SQRT(FCU*1000.)/1000.
      ALST(4)  =  3.*SQRT(FCU*1000.)/1000.
      ALST(5)  =  3.*SQRT(FCU*1000.)/1000.
      ALSB(1)  = -.6*FICU
      ALSB(2)  = -.6*FICU
      ALSB(3)  = -.6*FCU
      ALSB(4)  = -.6*FCU
      ALSB(5)  = -.6*FCU
      ALSM(1)  =  7.5*SQRT(FICU*1000.)/1000.
      ALSM(2)  =  7.5*SQRT(FICU*1000.)/1000.
      ALSM(3)  =  7.5*SQRT(FCU*1000.)/1000.
      ALSM(4)  =  7.5*SQRT(FCU*1000.)/1000.
      ALSM(5)  =  7.5*SQRT(FCU*1000.)/1000.
      IMP(1)   = 1.+LFTIMP
      IMP(2)   = 1.-LFTIMP
      IMP(3)   = 1.+SHPIMP
      IMP(4)   = 1.-SHPIMP
      IMP(5)   = 1.-SHPIMP
      C1=-XL-2.
      C2=XHARP*XL-XHARP*XHARP-1.
      DO 10 I=1,3
      KTRL(I)=5
10    PKPT(I)=XHARP
      ATEN=CONTRL(5)
      AMILD=CONTRL(5)
      ACOMP=CONTRL(5)
C
C   FIND PICKPTS FOR ALL 12 CASES AND FIND THE 3 CONTROLLING PICKPTS
C
      DO 20 J=1,4
         FHPS = AS*NHS*(USS*CSTR-FLOSS(J))
         FSS  = AS*NSS*(USS*CSTR-FLOSS(J))
         TEMP(J) =-(FSS+FHPS)/AG+(FHPS*(YB-CGH))/ZT+(FSS*(YB-CGS))/ZT
C   FIND FOR CONTROLLING TENSION
         DMOM=(-ALST(J)+TEMP(J))*ZT
         PTEMP=(DMOM/6./WG/IMP(J)-C2)/C1
         IF(K2.EQ.1)
     1   WRITE(BGUNIT,'(7X,''TENSION PICKPT    '',F10.3)')PTEMP
         IF(PTEMP.LT.PKPT(1))THEN
            PKPT(1)=PTEMP
            ATEN=CONTRL(J)
            KTRL(1)=J
         END IF
C   FIND FOR CONTROLLING TENSION WITH MILD REINFORC.
         DMOM=(-ALSM(J)+TEMP(J))*ZT
         PTEMP=(DMOM/6./WG/IMP(J)-C2)/C1
         IF(K2.EQ.1)
     1   WRITE(BGUNIT,'(7X,''MILD TENS. PICK   '',F10.3)')PTEMP
         IF(PTEMP.LT.PKPT(2))THEN
            PKPT(2)=PTEMP
            AMILD=CONTRL(J)
            KTRL(2)=J
         END IF
C   FIND FOR CONTROLLING COMPRESSION
         TEMB(J) =-(FSS+FHPS)/AG-(FHPS*(YB-CGH))/ZB-(FSS*(YB-CGS))/ZB
         IF(K2.EQ.1)
     1   WRITE(BGUNIT,'(7X,''ALLOWABLE    '',F10.3)')ALSB(J)
         IF(K2.EQ.1)
     1   WRITE(BGUNIT,'(7X,''PRES. STRESS  '',F10.3)')TEMB(J)
         DMOM=(ALSB(J)-TEMB(J))*ZB
         PTEMP=(DMOM/6./WG/IMP(J)-C2)/C1
         IF(K2.EQ.1)
     1   WRITE(BGUNIT,'(7X,''COMPRESS. PICK    '',F10.3)')PTEMP
         IF(PTEMP.LT.PKPT(3))THEN
            PKPT(3)=PTEMP
            ACOMP=CONTRL(J)
            KTRL(3)=J
         END IF
20    CONTINUE
C
C  PRINT THE 3 PICK POINTS
C
       WRITE (OUNIT,5) DATE
5      FORMAT('1',//,4X,'WASH. STATE DEPT. OF TRANSPORTATION  ',
     1      '* PICK POINT *',2X,'DATE ',
     2       A11)                                                       .C.
       WRITE (OUNIT,7) REVDAT,TIME
7      FORMAT(1X,'   REV ',A8,35X,'TIME ',A8)
      IF ( NHS.EQ. 0 ) THEN
          WRITE ( OUNIT, 2300 ) PBM
          GO TO 65
      END IF
      IF ( XHARP .LE. 0 .OR. XHARP .GE. XL / 2. ) THEN
          WRITE ( OUNIT, 2310 ) PBM
          GO TO 65
      END IF
      WRITE(OUNIT,200)PBM
      WRITE(OUNIT,210)
      WRITE(OUNIT,230)LFTIMP*100,SHPIMP*100
      WRITE(OUNIT,235)LFTLOS,SHPLOS
      WRITE(OUNIT,270)ALST(1),ALST(3)
      WRITE(OUNIT,280)ALSB(1),ALSB(3)
      WRITE(OUNIT,290)ALSM(1),ALSM(3)
      WRITE(OUNIT,300)
      WRITE(OUNIT,250)AS
      WRITE(OUNIT,260)USS
      WRITE(OUNIT,310)NHS,NSS
      WRITE(OUNIT,320)XL,XHARP
      WRITE(OUNIT,330)FICU,FCU
      WRITE(OUNIT,340)AG,WG
      WRITE(OUNIT,350)ZT,ZB
      WRITE(OUNIT,360)YB,CGH
      WRITE(OUNIT,370)CGS
      IF(KSTY.EQ.1)WRITE(OUNIT,*)'       STRANDS ARE STRESS RELIEVED'
      IF(KSTY.EQ.0)WRITE(OUNIT,*)'       STRANDS ARE LOW RELAXATION'
      WRITE(OUNIT,380)
      IF (PKPT(1) .LT. 0) THEN
          WRITE(OUNIT,8)ATEN
      ELSE
          WRITE(OUNIT,9)ATEN,PKPT(1)
      ENDIF
      IF (PKPT(3) .LT. 0) THEN
          WRITE(OUNIT,11)ATEN
      ELSE
          WRITE(OUNIT,12)ACOMP,PKPT(3)
      ENDIF
      IF (PKPT(2) .LT. 0) THEN
          WRITE(OUNIT,13)ATEN
      ELSE
          WRITE(OUNIT,14)AMILD,PKPT(2)
      ENDIF
8     FORMAT(/,7X,'ALLOWABLE TENSION (',A8,' CONTROLS)',18X,'NG')
9     FORMAT(/,7X,'ALLOWABLE TENSION (',A8,
     1 ' CONTROLS)',15X,F6.2,' FT.')
11    FORMAT(7X,'ALLOWABLE COMPRESSION (',A8,' CONTROLS)',14X,'NG')
12    FORMAT(7X,'ALLOWABLE COMPRESSION (',
     1    A8,' CONTROLS)',11X,F6.2,' FT.')
13    FORMAT(7X,'ALLOWABLE TENSION W/MILD STEEL (',
     1    A8,' CONTROLS)',5X,'NG')
14    FORMAT(7X,'ALLOWABLE TENSION W/MILD STEEL (',
     1    A8,' CONTROLS)',2X,F6.2,' FT.')
C
C   CALCULATE STRESSES FOR 3 PICKPTS
C
C
      DO 23 I=1,3
         J=I-1
         M = 1
         IF ( KTRL(I) .EQ. 2  .OR.  KTRL(I) .EQ. 4 ) M = 2
         N = M + 2
         J=J*2+1
         STP(J) = TEMP(M)-WG*6.*IMP(M)*(C1*PKPT(I)+C2)/ZT
         SBOT(J) = TEMB(M)+WG*6.*IMP(M)*(C1*PKPT(I)+C2)/ZB
         J=J+1
         STP(J) = TEMP(N)-WG*6.*IMP(N)*(C1*PKPT(I)+C2)/ZT
         SBOT(J) = TEMB(N)+WG*6.*IMP(N)*(C1*PKPT(I)+C2)/ZB
23    CONTINUE
C
C  PRINT CONTROLLING PICKPTS WITH STRESSES
C
      WRITE(OUNIT,415)
      WRITE(OUNIT,420)
C
      NG=.FALSE.
      IF (PKPT(1) .LT. PKPT(3)) THEN
          IF (PKPT(1) .LT. 0) THEN
              NG=.TRUE.
              WRITE(OUNIT,432)
          ELSE
              WRITE(OUNIT,430)PKPT(1),STP(1),SBOT(1),STP(2),SBOT(2)
          ENDIF
          IF (PKPT(2) .LE. PKPT(3)) THEN
          IF (PKPT(2) .LT. 0.)GO TO 25
              WRITE(OUNIT,437)PKPT(2),STP(3),SBOT(3),STP(4),SBOT(4)
          ELSE
          IF (PKPT(3) .LT. 0.)GO TO 25
              WRITE(OUNIT,435)PKPT(3),STP(5),SBOT(5),STP(6),SBOT(6)
          ENDIF
      END IF
C
25    IF (PKPT(3) .LT. PKPT(1)) THEN
          IF (PKPT(3) .LT. 0) THEN
              NG=.TRUE.
              WRITE(OUNIT,436)
          ELSE
              WRITE(OUNIT,430)PKPT(3),STP(5),SBOT(5),STP(6),SBOT(6)
          ENDIF
      ENDIF
C
C   COMPUTE AND PRINT STRESSES FOR USER SPECIFIED PICK-PT
C
      IF (PCKPT .EQ. 0) GO TO 60
      IF (PCKPT .GE. XHARP) THEN
           WRITE(OUNIT,2320)PBM
           GO TO 65
      END IF
      WRITE(OUNIT,445)
      TITL=' IMPACT DOWN'
      DO 57 I=1,2
         K=I-2
      DO 55 J=1,3,2
         K=K+2
         STP(J) = TEMP(J)-WG*6.*IMP(K)*(C1*PCKPT+C2)/ZT
         SBOT(J) = TEMB(J)+WG*6.*IMP(K)*(C1*PCKPT+C2)/ZB
55    CONTINUE
      WRITE(OUNIT,450)TITL,PCKPT,STP(1),SBOT(1),STP(3),SBOT(3)
57    TITL=' IMPACT UP'
C
60    WRITE(OUNIT,440)
      IF (NG) WRITE(OUNIT,460)
65    IF(IRS-4)999,1,999
C
200   FORMAT(/,4X,'PROBLEM NUMBER ',I4)
210   FORMAT(/,38X,'LIFTING',8X,'SHIPPING')
230   FORMAT(/,4X,'IMPACT FACTOR',22X,F3.0,'%',11X,F3.0,'%')
235   FORMAT(4X,'LOSSES',29X,F3.0,'KSI',9X,F3.0,'KSI')
250   FORMAT(/,8X,'STRAND AREA',12X,F5.3,' IN.**2')                     .C.
260   FORMAT(8X,'ULTIMATE STRENGTH',4X,F5.1,'   KSI')                   .C.
270   FORMAT(/,4X,'ALLOWABLE TENSILE STRESS',11X,F8.5,' KSI',3X,        
     1F8.5,' KSI')
280   FORMAT(4X,'ALLOWABLE COMPRESSIVE STRESS',7X,F8.5,' KSI',3X,
     1F8.5,' KSI')
290   FORMAT(4X,'ALLOWABLE TENS W/MILD STEEL',8X,F8.5,' KSI',3X,
     1F8.5,' KSI')
300   FORMAT(/,34X,'====================================')
310   FORMAT(/,8X,'NO. HARPED STRANDS',4X,I2,13X,                       .C.
     1 'NO. STRAIGHT STRANDS',3X,I2)
320   FORMAT(8X,'SPAN LENGTH',10X,F6.2,'  FT.',5X,'X HARP POINT',       .C.
     1 11X,F5.2,'  FT.')
330   FORMAT(8X,'FCI',16X,F6.1,'    KSI',5X,'FC',18X,F6.1,'    KSI')    .C.
340   FORMAT(8X,'GIRDER AREA', 9X,F6.1,'   IN.**2',2X,                  .C.
     1'GIRDER WT/FT',11X,F6.3,' K')
350   FORMAT(8X,'S TOP',14X,F6.0,'    IN.**3',2X,                       .C.
     1'S BOTTOM',12X,F6.0,'    IN.**3')
360   FORMAT(8X,'Y BOTTOM',12X,F6.1,'   IN.',5X,                        .C.
     1'FCL',20X,F5.2,'  IN.')
370   FORMAT(8X,'E',22X,F4.2,'  IN.')                                   .C.
380   FORMAT(/,4X,'PICK POINT DISTANCES (FROM CL BRG) FOR:')
415   FORMAT(//4X,'STRESSES @ HARPING POINT (KSI):',3X,
     1 '[---- LIFTING ---]',5X,                                         .C.
     2 '[--- SHIPPING ---]')
420   FORMAT(/39X,'TOP',7X,'BOTTOM',7X,'TOP',7X,'BOTTOM')               .C.
430   FORMAT(/,4X,'MAX PICK POINT (',F6.2,' FT.)',5X,F8.5,3X,
     1 F8.5,4X,F8.5,3X,F8.5)                                            .C.
432   FORMAT(/,4X,'MAX PICK POINT (NG)')
435   FORMAT(4X,'MAX W/MILD STEEL (',F6.2,' FT.)',3X,F8.5,3X,
     1 F8.5,4X,F8.5,3X,F8.5)                                            .C. 
436   FORMAT(4X,'FAILS IN COMPRESSION')
437   FORMAT(4X,'MAX W/MILD STEEL (',F6.2,' FT.)',3X,F8.5,3X,
     1 F8.5,4X,F8.5,3X,F8.5)                                            .C.
440   FORMAT(//,4X,'WARNING:  STANDARD SPECS SPECIFY THAT THE MINUMUM PI
     1CK POINTS ARE:',/,
     2 13X,' LIFTING OR STORAGE -',/,
     3 17X,' 1.5 FT. OR AT THE PAD RECESS.',/,
     4 13X,' SHIPPING -',/,
     5 17X,' 3.0 FT. FOR W42G, W50G AND BULB TEE GIRDERS',/,            .C.
     6 17X,' 4.0 FT. FOR W58G GIRDERS',/,                               .C.
     7 17X,' 5.0 FT. FOR W73G GIRDERS.',/)                              .C.
445   FORMAT(4X,'SPECIFIED PICK PT.')
450   FORMAT(9X,A13,' (',F6.2,' FT.)',1X,F8.5,3X,
     1 F8.5,4X,F8.5,3X,F8.5)                                            .C.
460   FORMAT(//,4X,'(NG):  NO GOOD, ALLOWABLE STRESS EXCEEDED')
C
999   RETURN
C
2000  WRITE(ERUNIT,2010)
2010  FORMAT(2X,'INSUFFICIENT DATA ... TERMINATING PICKPT.')
      RETURN
2100  WRITE(ERUNIT,2110)
2110  FORMAT(2X,'FILE EMPTY ... TERMINATING PICKPT.')
      RETURN
2200  WRITE(ERUNIT,2210)
2210  FORMAT(2X,'ERROR IN DATA SET ... TERMINATING PICKPT.')
      RETURN
2300  FORMAT(12X,' PROBLEM NO. ',I4,/,12X,' PROGRAM PICKPT IS NOT DESIGN
     1ED TO ANALYZE GIRDERS WITHOUT',/,12X,' HARPED STRANDS.  TERMINATIN
     2G PICKPT.')
2310  FORMAT(12X,' PROBLEM NO. ',I4,/,12X,' EQUATIONS IN PICKPT CANNOT H
     1ANDLE HARPING POINTS EQUAL TO',/,12X,' ZERO OR HALF THE SPAN.  TER
     2MINATING PICKPT.')
2320  FORMAT(12X,' PROBLEM NO. ',I4,/,12X,' EQUATIONS IN PICKPT CANNOT H
     1ANDLE PICKPTS PAST THE HARP PT')
      END

