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

C
C
C   REVISION LOG:
C
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/27/93
C
C
C
C
C
      SUBROUTINE PARM
      CHARACTER*16 FLNM1, FLNM2
      INTEGER OUNIT, ERUNIT
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C  STANDARD INTERACTIVE INPUT UNIT
      IIUNIT=1
C  STANDARD INTERACTIVE OUTPUT UNIT
      IOUNIT=3
C  STANDARD REPORT OUTPUT UNIT
      OUNIT=6
C  STANDARD FILE INPUT UNIT
      INUNIT=5
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
10    WRITE(*,210)
210   FORMAT(//,
     1' Enter input file name:',/)
           READ(*,ERR=10,FMT='(A)')FLNM1
C          IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'LPT1')GO TO 25
C          IF(FLNM1.EQ.'con'.OR.FLNM1.EQ.'lpt1')GO TO 25
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
25         WRITE(*,FMT='(/,1X,A16,'' used for input  '',//)')FLNM1
35    WRITE(*,235)
235   FORMAT(//,
     1' Enter output file name:',/)
           READ(*,ERR=35,FMT='(A)')FLNM2
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
60    WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2
      OPEN(OUNIT,FILE=FLNM2,STATUS='NEW')
      OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')
      RETURN
      END
C

