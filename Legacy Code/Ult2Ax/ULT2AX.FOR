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

C***  USD 2 AXES COLUMN DESIGN *** ULT2AX(X) ***                RFF     
C  PROGRAM ULT2AX COMPUTES THE ULTIMATE MOMENT CAPACITY ABOUT TWO       
C  PERPENDICULAR AXES FOR A GIVEN CONCRETE SECTION SUBJECT TO A GIVEN   
C  AXIAL LOAD.                                                          
C     REV.3-15-75 BY HOU                                                
C     A - GARY CHRISTOPHER - REV. 9/13/73                               
C     B - NEAL CAMPBELL - PC CONVERSION AND ADDITION OF CIRCLE GENERATOR
C     T - TERRY BEEBE   - VMS CONVERSION - FEBRUARY 1990                
C     RICHARD BRICE - RECOMPILED FOR VMS VERSION 5.5 - 04/28/93
C       
C                                                                       
      IMPLICIT REAL*8 (A-H,O-Z)                                         
      CHARACTER*1 CARCNT
      INTEGER*4 OUNIT, ERUNIT
      CHARACTER*8 REVDAT, TIME
      CHARACTER*11 DATE
      CHARACTER*23 DATIME
      CHARACTER*16 FLNM3
      DIMENSION RAREA(300),TAREA(50),CFC(15),D(50),PRST(50),F(300),G(50)
      DIMENSION FC(15),ST(300),PRSTR(50),FCB(50),NVERT(15),FOR(300)     
      DIMENSION P(50,15),Q(50,15),RC(300),SC(50),X1(50,15),Y1(50,15)    
      DIMENSION               X3(300),Y3(300),X4(50),Y4(50),X5(50),O(50)
      DIMENSION X2(300),Y2(300)                                         
      DIMENSION Y5(50),AREA(50),NBAR(50),X6(300),Y6(300)                
C                                                                       
C  ALL INPUT UNITS IN KIPS AND INCHES, COMP = + , TENSION = -           
C                                                                       
      ERUNIT=9                                                          
      INUNIT=5                                                          
      OUNIT=6                                                           
      NOERR = 0                                                         
      IREAD = 10                                                        
      CARCNT = '1'                                                      
C                                                                       
C                                                                       
C                                                                       
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                      
C     OUNIT = (6 FOR VMS FILE OUTPUT)                                   
C     ERUNIT = (* FOR VMS SCREEN OUTPUT)                                
C              (9 FOR MAINFRAME)                                        
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                      
C                                                                       
C                                                                       
      CALL PARM(OUNIT,ERUNIT)                                           
      ICARD=OUNIT                                                       
C      CALL LIB$DATE_TIME (DATIME)   
	CALL DATE_AND_TIME (DATIME)	                                    
      TIME = DATIME(13:20)                                              
      DATE = DATIME(1:11)                                               
      INUNIT = 5
      OUNIT  = 6
C                                                                       
C                                                                       
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC  
      REVDAT = ' 2/05/90'                                               
CCCCCCCCCCCCCCCCCCCCCCCCCCC REVDAT CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC  
C                                                                       
   10 CONTINUE                                                          
      IOUT=0                                                            
      ITAPE=7                                                           
C     REWIND 7                                                          
      READ(INUNIT,END=660,ERR=620,FMT=*)N2                              
      BACKSPACE INUNIT                                                  
      IREAD = IREAD + 10                                                
      WRITE(ITAPE,11)N2                                                 
   11 FORMAT(I4)                                                        
C                                                                       
      READ(INUNIT,END=640,ERR=620,FMT=*)N2,IPLOT                        
      IF(IPLOT .EQ. 0)GO TO 19
12    WRITE(*,13)
   13 FORMAT(//,
     1' Enter plot file name:',/)
      READ(*,ERR=12,FMT='(A)')FLNM3
         DO 14 I=1,16
         IF(FLNM3(I:I).EQ.'.')GO TO 16
14    CONTINUE
         DO 18 I=1,16
         IF(FLNM3(I:I).EQ.' ')THEN
            IE = I+3
            FLNM3(I:IE) = '.plo'
            GO TO 16
         END IF
18    CONTINUE
16    WRITE(*,FMT='(/,'' '',1X,A16,'' used for plot file'',///)')FLNM3
      OPEN(8,FILE=FLNM3,STATUS='NEW')         
19    IREAD = IREAD + 10                                                        
      DO 25 I = 1,N2                                                            
         NCIR=0                                                                 
         READ(INUNIT,END=640,ERR=620,FMT=*)NUM1,FC(I)                           
         IF(NUM1.LT.0)THEN                                                      
         NCIR=1                                                                 
         NUM1=-NUM1                                                             
         END IF                                                                 
         IREAD = IREAD + 10                                                     
         NVERT(I) = NUM1                                                        
C                                                                               
         NVRT = NVERT(I)                                                        
      WRITE(ITAPE,11)NVERT(I)                                                   
      IF(NCIR.EQ.1)THEN                                                         
      READ(INUNIT,END=640,ERR=620,FMT=*)DIAM,KK                                 
      DELST=0                                                                   
      CALL CIRGA   (DIAM,X1,Y1,NUM1,KK,I,DELST)                                 
      IREAD=IREAD+(NUM1*10)                                                     
      ELSE                                                                      
         DO 20 J = 1,NVRT                                                       
            READ(INUNIT,END=640,ERR=620,FMT=*)X1(J,I),Y1(J,I)                   
 20         IREAD = IREAD + 10                                                  
      END IF                                                                    
      NV=NVERT(I)                                                               
   25 WRITE(ITAPE,22)(X1(J,I),Y1(J,I),J=1,NV)                                   
   22 FORMAT(8F7.2/10F7.2/10F7.2/10F7.2/10F7.2/10F7.2/10F7.2)                   
C                                                                               
      NCIR=0                                                                    
      READ(INUNIT,END=640,ERR=620,FMT=*)N9                                      
      IREAD=IREAD+10                                                            
      WRITE(ITAPE,11)N9                                                         
      IF (N9 .EQ. 0) GO TO 40                                                   
      IF(N9.LT.0)THEN                                                           
      N9=-N9                                                                    
      NCIR=1                                                                    
      END IF                                                                    
      IF(NCIR.EQ.1)THEN                                                         
      READ(INUNIT,END=640,ERR=620,FMT=*)DIAM,DELST,TARTMP,PRSTMP                
      DO 30 K=1,N9                                                              
      TAREA(K)=TARTMP                                                           
30    PRSTR(K)=PRSTMP                                                           
      CALL CIRGB   (DIAM,X2,Y2,N9,1,DELST)                                      
      ELSE                                                                      
C                                                                               
      READ(INUNIT,END=640,ERR=620,FMT=*)(X2(K),Y2(K),TAREA(K),PRSTR(K),K        
     1=1,N9)                                                                    
      END IF                                                                    
      IREAD=IREAD+(N9*10)                                                       
      WRITE(ITAPE,33)(X2(K),Y2(K),K=1,N9)                                       
   33 FORMAT(8F7.2/8F7.2/8F7.2/8F7.2/8F7.2/8F7.2/8F7.2/8F7.2)                   
C                                                                               
  40  NCIR=0                                                                    
      READ(INUNIT,END=640,ERR=620,FMT=*)NT2                                     
      WRITE(ITAPE,11)NT2                                                        
      IREAD = IREAD + 10                                                        
      IF (NT2 .EQ. 0) GO TO 60                                                  
      IF(NT2.LT.0)THEN                                                          
      NCIR=1                                                                    
      NT2=-NT2                                                                  
      END IF                                                                    
      IF(NCIR.EQ.1)THEN                                                         
      READ(INUNIT,END=640,ERR=620,FMT=*)DIAM,DELST,RARTMP                       
      DO 50 L = 1 , NT2                                                         
50    RAREA(L)=RARTMP                                                           
      CALL CIRGB   (DIAM,X3,Y3,NT2,1,DELST)                                     
      ELSE                                                                      
      READ(INUNIT,END=640,ERR=620,FMT=*)(X3(L),Y3(L),RAREA(L),L=1,NT2)          
      END IF                                                                    
      IREAD=IREAD+(NT2*10)                                                      
      WRITE(ITAPE,33)(X3(L),Y3(L),L=1,NT2)                                      
C                                                                               
   60 NCIR=0                                                                    
      READ(INUNIT,END=640,ERR=620,FMT=*)NW1                                     
      IREAD = IREAD + 10                                                        
      WRITE(ITAPE,11)NW1                                                        
      IF (NW1 .EQ. 0) GO TO 80                                                  
C                                                                               
      READ(INUNIT,END=640,ERR=620,FMT=*)(AREA(I),X4(I),Y4(I),NBAR(I),           
     1    X5(I),Y5(I),I=1,NW1)                                                  
      IREAD=IREAD+(NW1*10)                                                      
      WRITE(ITAPE,33)(X4(I),Y4(I),X5(I),Y5(I),I=1,NW1)                          
80    READ(INUNIT,END=640,ERR=620,FMT=*)YLDST,RFACTR                            
      IREAD = IREAD + 10                                                        
C                                                                               
      READ(INUNIT,END=640,ERR=620,FMT=*)NOR,DEGRE,INDT                          
      IREAD = IREAD + 10                                                        
C                                                                               
      READ(INUNIT,END=640,ERR=620,FMT=*)NOAXL,XCENT,YCENT                       
      IREAD = IREAD + 10                                                        
      WRITE(ITAPE,84)NOR,DEGRE,NOAXL,XCENT,YCENT                                
   84 FORMAT(I4,F7.2,I4,2F7.2)                                                  
C                                                                               
      IF (INDT .EQ. 0) GO TO 280                                                
C                                                                               
C  LIST INPUT DATA EXCEPT FOR AXIAL LOADS                                       
C                                                                               
   90 FORMAT(A1,//,' WASHINGTON STATE DEPT. OF TRANSPORTATION   ',              
     1  A11,'  TIME ',A8)                                                       
   91 FORMAT(' *** ULT2AX COLUMN ANALYSIS ***   REV ',A8,/,22X,                 
     1 'LISTING OF INPUT DATA',/)                                               
       WRITE(ICARD,90)CARCNT,DATE,TIME                                          
       WRITE(ICARD,91)REVDAT                                                    
      WRITE (ICARD,100) N2                                                      
  100 FORMAT(22X,'NUMBER OF CONCRETE AREAS = ',I2,/)                            
C                                                                               
      DO 140 J = 1,N2                                                           
         WRITE (ICARD,110) J,NVERT(J),FC(J)                                     
  110    FORMAT(' CONCRETE AREA ',I2,', NO. OF POINTS = ',I2,', CONC STR        
     *ENGTH = ',F6.3,' KSI')                                                    
         WRITE (ICARD,120)                                                      
  120    FORMAT(' POINT       X           Y')                                   
C                                                                               
         NVRT = NVERT(J)                                                        
         DO 140 I = 1,NVRT                                                      
            WRITE(ICARD,130) I,X1(I,J),Y1(I,J)                                  
  130       FORMAT(2X,I2,5X,F9.2,3X,F9.2)                                       
  140    CONTINUE                                                               
C                                                                               
      WRITE (ICARD,150) N9                                                      
  150 FORMAT(/,' NUMBER OF TYPE 270K STRAND AREAS = ',I2)                       
      IF (N9 .EQ. 0) GO TO 185                                                  
      WRITE (ICARD,160)                                                         
  160 FORMAT('   AREA       X          Y     PRESTRESS LEVEL')                  
C                                                                               
      DO 180 II = 1,N9                                                          
         WRITE (ICARD,170)TAREA(II),X2(II),Y2(II),PRSTR(II)                     
  170    FORMAT(' ',F7.3,2X,F9.3,2X,F9.3,4X,F8.3)                               
  180    CONTINUE                                                               
C                                                                               
  185 WRITE (ICARD,190) NT2,YLDST                                               
  190 FORMAT(/,' NUMBER OF INDIV REBAR AREAS = ',I3,' REBAR FY = ',F7.3)        
      IF (NT2 .EQ. 0) GO TO 230                                                 
      WRITE (ICARD,200)                                                         
  200 FORMAT('   AREA       X          Y')                                      
C                                                                               
      DO 220 JJ = 1,NT2                                                         
         WRITE (ICARD,210) RAREA(JJ),X3(JJ),Y3(JJ)                              
  210    FORMAT(' ',F7.3,2X,F9.3,2X,F9.3)                                       
  220    CONTINUE                                                               
C                                                                               
  230 WRITE (ICARD,240) NW1,YLDST                                               
  240 FORMAT(/,' NUMBER OF LINES OF REBAR = ',I2,' REBAR FY = ',F7.3)           
      IF (NW1 .EQ. 0) GO TO 280                                                 
      WRITE (ICARD,250)                                                         
  250 FORMAT('   AREA       X1         Y1    NO. OF BARS     X2                 
     *Y2')                                                                      
C                                                                               
      DO 270 KK = 1,NW1                                                         
         WRITE (ICARD,260) AREA(KK),X4(KK),Y4(KK),NBAR(KK),X5(KK),Y5(KK)        
  260    FORMAT(' ',F7.3,2X,F9.3,2X,F9.3,5X,I2,6X,F9.3,2X,F9.3)                 
  270    CONTINUE                                                               
C                                                                               
C  OUTPUT LISTING                                                               
C                                                                               
  280 DO 615 JK = 1,NOAXL                                                       
         READ(INUNIT,END=640,ERR=620,FMT=*)AXLOAD                               
         IREAD = IREAD + 10                                                     
C                                                                               
         P1 = AXLOAD / RFACTR                                                   
C                                                                               
         WRITE (ICARD,290)                                                      
  290    FORMAT(///,16X,'ULTIMATE MOMENTS ABOUT X AND Y AXES')                  
         WRITE (ICARD,300) RFACTR,AXLOAD,XCENT,YCENT                            
  300    FORMAT(//,' PHI = ',F5.2,'  PULT = ',F11.2,'   X = ',F9.3,'  Y         
     *= ',F9.3)                                                                 
         WRITE (ICARD,310)                                                      
  310    FORMAT('   SLOPE         MX            MY',/,'   OF NA        U        
     *LT           ULT')                                                        
C                                                                               
C  COMPUTE THE COORDINATES AND AREA OF REBAR IN A LINE                          
C                                                                               
         IF (NW1 .EQ. 0) GO TO 330                                              
         NW2 = NT2                                                              
C                                                                               
         DO 320 I = 1,NW1                                                       
            NUMBAR = NBAR(I)                                                    
            DO 320 K = 1,NUMBAR                                                 
               NW2 = NW2 + 1                                                    
               IF (NBAR(I) .NE. 1) GO TO 315                                    
               X3(NW2) = X4(I)                                                  
               Y3(NW2) = Y4(I)                                                  
               GO TO 320                                                        
  315          DIVD = (NBAR(I) -1)                                              
               COUNTR = K - 1                                                   
               RAREA(NW2) = AREA(I)                                             
               X3(NW2) = X4(I) + (X5(I) - X4(I)) / DIVD * COUNTR                
               Y3(NW2) = Y4(I) + (Y5(I) - Y4(I)) / DIVD * COUNTR                
  320          CONTINUE                                                         
C                                                                               
         N7 = NW2                                                               
         GO TO 340                                                              
  330    N7 = NT2                                                               
  340    FC1 = FC(1)                                                            
         C3 = 1.05 - 0.05 * FC1                                                 
         IF (C3 - 0.85) 360,360,350                                             
  350    C3 = 0.85                                                              
  360    C4 = -0.003                                                            
C                                                                               
         DO 370 I = 1,N2                                                        
  370       CFC(I) = 0.85 * FC(I)                                               
C                                                                               
C  SET UP CLOSURE AREAS                                                         
C                                                                               
         DO 380 J = 1,N2                                                        
            N5 = NVERT(J) + 1                                                   
            X1(N5,J) = X1(1,J)                                                  
  380       Y1(N5,J) = Y1(1,J)                                                  
C                                                                               
C  ROTATE ANGLE OF NEUTRAL AXIS                                                 
C                                                                               
         DO 610 IZ = 1,NOR                                                      
            IF (NOR .EQ. 1) GO TO 390                                           
            RIZ = IZ - 1                                                        
            RNOR = NOR - 1                                                      
            ANU2 = RIZ * (DEGRE / RNOR)                                         
            GO TO 400                                                           
  390       ANU2 = 0.0                                                          
  400       ANU3 = ANU2 * 2.0 * 3.14159265358D0 / 360.0                         
C                                                                               
C  TRANSFORM COORDINATES                                                        
C                                                                               
            C2 = DCOS(ANU3)                                                     
            SX = DSIN(ANU3)                                                     
C                                                                               
            DO 410 IY = 1,N2                                                    
               N5 = NVERT(IY) + 1                                               
               DO 410 IX = 1,N5                                                 
                  P(IX,IY) = X1(IX,IY) * C2 + Y1(IX,IY) *SX                     
  410             Q(IX,IY) = Y1(IX,IY) * C2 - X1(IX,IY) *SX                     
            IF (N9 .EQ. 0) GO TO 425                                            
            DO 420 IW = 1,N9                                                    
  420          SC(IW) = Y2(IW) * C2 - X2(IW) * SX                               
  425       IF (N7 .EQ. 0) GO TO 435                                            
            DO 430 K = 1,N7                                                     
  430          RC(K) = Y3(K) * C2 - X3(K) * SX                                  
C                                                                               
C  LOCATE THE COMPRESSION FACE                                                  
C                                                                               
  435       V2 = Q(1,1)                                                         
C                                                                               
            DO 440 KZ = 1,N2                                                    
               N4 = NVERT(KZ)                                                   
               DO 440 KY = 1,N4                                                 
                  IF (Q(KY,KZ) .LE. V2) GO TO 440                               
                  V2 = Q(KY,KZ)                                                 
  440          CONTINUE                                                         
C                                                                               
C  GO THROUGH SUCCESSIVE APPROXIMATIONS FOR LOCATING NEUTRAL AXIS               
C                                                                               
            G1 = 0.0001                                                         
C                                                                               
            DO 590 JA = 1,4                                                     
  445          G1 = G1 - (1.0 / (-10.0)**JA)                                    
               IF ((G1 - 1.57) .GE. 0.0) GO TO 450                              
               IF (FC1.GT. 0.0) GO TO 465                                       
C                                                                               
  450          WRITE (ICARD,460) ANU2                                           
  460          FORMAT(' WHEN THE ANGLE OF NEUTRAL AXIS = ',F6.2,' EQUILI        
     *RIUM CANNOT BE FOUND')                                                    
C                                                                               
      IOUT=1                                                                    
               GO TO 610                                                        
  465          C1 = 100.0 * (DSIN(G1) / DCOS(G1))                               
               V4 = V2 - C1                                                     
               V1 = V2 - C3 * C1                                                
C                                                                               
C  COMPUTE THE FORCES IN CONCRETE                                               
C                                                                               
               C5 = 0.0                                                         
               Z1 = 0.0                                                         
               ZY1 = 0.0                                                        
C                                                                               
               DO 520 JB = 1,N2                                                 
                  TA = 0.0                                                      
                  S9 = 0.0                                                      
                  S8 = 0.0                                                      
                  N4 = NVERT(JB)                                                
                  DO 510 JC = 1,N4                                              
                     IF ((V1 - Q(JC,JB)) .LT. 0.0) GO TO 470                    
                     IF (V1 - Q(JC+1,JB)) 480,510,510                           
  470                IF ((V1 - Q(JC+1,JB)) .LE. 0.0)GO TO 490                   
                     U1 = P(JC,JB)                                              
                     U2 = P(JC,JB) +(P(JC+1,JB) - P(JC,JB)) * (V1 -             
     *                Q(JC,JB)) / (Q(JC+1,JB) - Q(JC,JB))                       
                     V5 = Q(JC,JB)                                              
                     V6 = V1                                                    
                     GO TO 500                                                  
  480                U1 = P(JC,JB) + (P(JC+1,JB) - P(JC,JB)) * (V1 -            
     *                Q(JC,JB)) / (Q(JC+1,JB) - Q(JC,JB))                       
                     U2 = P(JC+1,JB)                                            
                     V5 = V1                                                    
                     V6 = Q(JC+1,JB)                                            
                     GO TO 500                                                  
  490                U1 = P(JC,JB)                                              
                     U2 = P(JC+1,JB)                                            
                     V5 = Q(JC,JB)                                              
                     V6 = Q(JC+1,JB)                                            
C                                                                               
C  COMPUTE THE AREA AND MOMENT OF AREA                                          
C                                                                               
  500                D1 = V6 - V5                                               
                     ANU4 = U1 * D1                                             
                     ANU5 = (U2 - U1) * D1 / 2.0                                
                     TA = TA + ANU4 + ANU5                                      
                     S9 = S9 + ANU4 * U1 / 2.0 + ANU5 * (U1 + (U2 - U1)         
     *                / 3.0)                                                    
                     S8 = S8 + ANU4 * (V5 + D1 / 2.0) + ANU5 * (V5 +            
     *                2.0 * D1 / 3.0)                                           
  510             CONTINUE                                                      
C                                                                               
                  C5 = C5 + TA * CFC(JB)                                        
                  V3 = S9 * CFC(JB)                                             
                  UA = S8 * CFC(JB)                                             
                  Z1 = Z1 + UA * C2 + V3 * SX                                   
  520             ZY1 = ZY1 + V3 * C2 - UA * SX                                 
C                                                                               
C  COMPUTE THE TENSION IN TYPE 270K STRAND                                      
C                                                                               
               S6 = 0.0                                                         
               S4 = 0.0                                                         
               S2 = 0.0                                                         
               IF (N9 .LE. 0) GO TO 540                                         
C                                                                               
               DO 530 K1 = 1,N9                                                 
                  D(K1) = C4 * (V4 - SC(K1)) / (V4 - V2)                        
                  PRST(K1) = PRSTR(K1) / 27400.0D0                              
                  G(K1) = D(K1) + PRST(K1)                                      
                  E1 = G(K1)                                                    
C                                                                               
                  CALL ULTSS (E1,F2)                                            
C                                                                               
                  FCB(K1) = F2                                                  
                  O(K1) = FCB(K1) * TAREA(K1)                                   
                  S4 = S4 + O(K1) * Y2(K1)                                      
                  S2 = S2 + O(K1) * X2(K1)                                      
  530             S6 = S6 + O(K1)                                               
C                                                                               
C  COMPUTE THE TENSION IN REBAR                                                 
C                                                                               
  540          S5 = 0.0                                                         
               S3 = 0.0                                                         
               S1 = 0.0                                                         
               IF (N7 .LE. 0) GO TO 580                                         
C                                                                               
               DO 570 K2 = 1,N7                                                 
                  F(K2) = C4 * (V4 - RC(K2)) / (V4 - V2)                        
                  ST(K2) = F(K2) * 29000.0D0                                    
                  IF (ST(K2) .LE. YLDST) GO TO 550                              
                  ST(K2) = YLDST                                                
  550             IF (ST(K2) .GE. -YLDST) GO TO 560                             
                  ST(K2) = -YLDST                                               
  560             FOR(K2) = ST(K2) * RAREA(K2)                                  
                  S3 = S3 + FOR(K2) * Y3(K2)                                    
                  S1 = S1 + FOR(K2) * X3(K2)                                    
  570             S5 = S5 + FOR(K2)                                             
C                                                                               
C  CHECK EQUILIBRIUM OF FORCES                                                  
C                                                                               
  580          IF ((P1 + C5 + S6 + S5) * (-1.0)**JA .LT. 0.0) GO TO 445         
  590       CONTINUE                                                            
C                                                                               
            Z3 = P1 * YCENT                                                     
            ZY3 = P1 * XCENT                                                    
            Z2 = -1.0 * (Z3 + Z1 + S4 + S3) / 12.0 * RFACTR                     
            ZY2 = -1.0 * (ZY3 + ZY1 + S2 + S1) / 12.0 * RFACTR                  
C                                                                               
            WRITE (ICARD,600) ANU2,Z2,ZY2                                       
  600       FORMAT(2X,F6.2,3X,F11.2,3X,F11.2)                                   
      X6(IZ)=Z2                                                                 
      Y6(IZ)=ZY2                                                                
C                                                                               
  610    CONTINUE                                                               
  615 WRITE(ITAPE,616)AXLOAD,(X6(IZ),Y6(IZ),IZ=1,NOR)                           
  616 FORMAT(6F12.2/6F12.2/6F12.2/6F12.2/6F12.2/6F12.2/6F12.2/6F12.2)           
      IF(IPLOT.EQ.0) GO TO 10                                                   
      IF(IPLOT.EQ.9) GO TO 617                                                  
      GO TO 619                                                                 
  617 IF(IOUT.EQ.0) GO TO 618                                                   
      IOUT=9                                                                    
      GO TO 619                                                                 
618   IOUT=8                                                                    
619   CALL ULTPL (IOUT,DATE,ERUNIT,OUNIT)                                       
      GO TO 10                                                                  
C                                                                               
620   WRITE(ERUNIT,*)' INPUT ERROR!! CHECK YOUR DATA FILE!!!'                   
      GO TO 680                                                                 
  640 WRITE (ERUNIT,650)                                                        
  650 FORMAT(//,' ******  PROGRAM REACHES END OF FILE BEFORE PROCESSING         
     *IS COMPLETE')                                                             
      GO TO 680                                                                 
  660 WRITE (ICARD,670)                                                         
  670 FORMAT(///,' PROCESSING COMPLETED')                                       
C                                                                               
  680 STOP                                                                      
      END                                                                       
C                                                                               
C                                                                               
C                                                                               
C                                                                               
C                                                                               
C                                                                               
C                                                                               
C                                                                               
      SUBROUTINE CIRGA   (D,X,Y,NNODES,KK,I,DELST)                              
      IMPLICIT REAL*8 (A-H,O-Z)                                                 
      DIMENSION X(50,15),Y(50,15)                                               
      R=D/2.                                                                    
      DELST=DELST*3.14159/180.                                                  
C  KK IS 1 FOR CLOCKWISE, 0 FOR COUNTER-CLOCKWISE                               
      DO 20 J=1,NNODES                                                          
      IF(KK.EQ.1)THEN                                                           
      DEG=2.*3.14159*(NNODES-J)/NNODES-DELST                                    
      ELSE                                                                      
      DEG=2.*3.14159*(J-1)/NNODES+DELST                                         
      END IF                                                                    
      X(J,I)=R*COS(DEG)                                                         
20    Y(J,I)=R*SIN(DEG)                                                         
      RETURN                                                                    
      END                                                                       
C                                                                               
C                                                                               
C                                                                               
C                                                                               
      SUBROUTINE CIRGB   (D,X,Y,NNODES,KK,DELST)                                
      IMPLICIT REAL*8 (A-H,O-Z)                                                 
      DIMENSION X(300),Y(300)                                                   
      R=D/2.                                                                    
      DELST=DELST*3.14159/180.                                                  
C  KK IS 1 FOR CLOCKWISE, 0 FOR COUNTER-CLOCKWISE                               
      DO 20 J=1,NNODES                                                          
      IF(KK.EQ.1)THEN                                                           
      DEG=2.*3.14159*(NNODES-J)/NNODES-DELST                                    
      ELSE                                                                      
      DEG=2.*3.14159*(J-1)/NNODES+DELST                                         
      END IF                                                                    
      X(J)=R*COS(DEG)                                                           
20    Y(J)=R*SIN(DEG)                                                           
      RETURN                                                                    
      END                                                                       
C                                                                               
C                                                                               
C                                                                               
C                                                                               
C                                                                               
C                                                                               
C                                                                               
      SUBROUTINE ULTSS (E1,F2)                                                  
C                                                                               
C  SUBROUTINE ULTSS DETERMINES THE ULTIMATE STEEL STRESS OR 270K STD            
C                                                                               
      REAL*8 ST(15), TV(15)                                                     
C                                                                               
      ST(1) = 0.0                                                               
      ST(2) = 180.0                                                             
      ST(3) = 190.0                                                             
      ST(4) = 200.0                                                             
      ST(5) = 210.0                                                             
      ST(6) = 220.0                                                             
      ST(7) = 230.0                                                             
      ST(8) = 240.0                                                             
      ST(9) = 245.0                                                             
      ST(10) = 250.0                                                            
      ST(11) = 255.5                                                            
      ST(12) = 260.5                                                            
      ST(13) = 263.0                                                            
      ST(14) = 264.0                                                            
      ST(15) = 270.0                                                            
C                                                                               
      TV(1) = 0.0D0                                                             
      TV(2) = 0.00657D0                                                         
      TV(3) = 0.007D0                                                           
      TV(4) = 0.00743D0                                                         
      TV(5) = 0.00786D0                                                         
      TV(6) = 0.00835D0                                                         
      TV(7) = 0.00890D0                                                         
      TV(8) = 0.00968D0                                                         
      TV(9) = 0.0102D0                                                          
      TV(10) = 0.011D0                                                          
      TV(11) = 0.0125D0                                                         
      TV(12) = 0.015D0                                                          
      TV(13) = 0.0175D0                                                         
      TV(14) = 0.020D0                                                          
      TV(15) = 0.035D0                                                          
C                                                                               
      IH = 2                                                                    
      IF (E1 .GE. 0.0) GO TO 10                                                 
      E1 = -E1                                                                  
      IH = 1                                                                    
   10 IF ((E1 - 0.035) .LT. 0.0) GO TO 20                                       
      F2 = 270.0 * ((-1.0)**IH)                                                 
      GO TO 50                                                                  
C                                                                               
   20 DO 30 N = 2,15                                                            
         IN = N                                                                 
         IF ((TV(N) - E1) .GT. 0.0) GO TO 40                                    
   30    CONTINUE                                                               
C                                                                               
   40 F2 = ((-1.0)**IH) * (ST(IN) - (ST(IN) - ST(IN-1)) * (TV(IN) - E1)         
     1   / (TV(IN) - TV(IN-1)))                                                 
C                                                                               
   50 RETURN                                                                    
      END                                                                       
      SUBROUTINE ULTPL (IOUT,DATE,ERUNIT,OUNIT)                                 
C                                                                               
C                                                                               
      INTEGER*4 OUNIT,ERUNIT                                                    
      CHARACTER*1 IBCD                                                          
      CHARACTER*8 DATE                                                          
      DIMENSION NOVR(15),XCOV(50,15),YCOV(50,15),WORK(2000),                    
     *       PAXL(10),XCOBI(200),YCOBI(200),AMX(200,10),AMY(200,10),            
     *       XCBL1(50),YCBL1(50),XCBL2(50),YCBL2(50),XSTR(50),YSTR(50)          
C      IF(ERUNIT.NE.9)THEN                                                      
C      WRITE(*,*) 'PC PLOTS NOT AVAILABLE YET, PROGRAM CONTINUES.'              
C      RETURN                                                                   
C      END IF                                                                   
      ICARD=OUNIT                                                               
      ITAPE=7                                                                   
      IBIG1=0                                                                   
      IBIG2=0                                                                   
      REWIND 7                                                                  
      IF(IOUT.EQ.9) GO TO 8                                                     
      IF(IOUT.EQ.8) GO TO 9                                                     
      GO TO 10                                                                  
8     IOUT=1                                                                    
      GO TO 10                                                                  
9     IOUT=0                                                                    
      IBIG1=1                                                                   
10    CONTINUE                                                                  
      READ(ITAPE,11)NARE                                                        
 11   FORMAT(I4)                                                                
C###                                                                            
      DO 20 I=1,MAX(ISTART,NARE)                                                
      READ(ITAPE,11)NOVR(I)                                                     
      NV=NOVR(I)                                                                
      READ(ITAPE,22)(XCOV(J,I),YCOV(J,I),J=1,NV)                                
 22   FORMAT(8F7.2/10F7.2/10F7.2/10F7.2/10F7.2/10F7.2/10F7.2)                   
20    CONTINUE                                                                  
C###                                                                            
      READ(ITAPE,11)NSTR                                                        
      IF(NSTR.EQ.0) GO TO 40                                                    
      READ(ITAPE,33)(XSTR(K),YSTR(K),K=1,NSTR)                                  
 33   FORMAT(8F7.2/8F7.2/8F7.2/8F7.2/8F7.2/8F7.2/8F7.2/8F7.2)                   
C###                                                                            
40    READ(ITAPE,11)NBAI                                                        
      IF(NBAI.EQ.0) GO TO 50                                                    
      READ(ITAPE,33)(XCOBI(L),YCOBI(L),L=1,NBAI)                                
C###                                                                            
50    READ(ITAPE,11)NBAL                                                        
      IF(NBAL.EQ.0) GO TO 60                                                    
      READ(ITAPE,33)(XCBL1(I),YCBL1(I),XCBL2(I),YCBL2(I),I=1,NBAL)              
C###                                                                            
60    READ(ITAPE,44)NORE,DEGR,NOAX,XCAX,YCAX                                    
 44   FORMAT(I4,F7.2,I4,2F7.2)                                                  
      IF(IOUT.EQ.1) GO TO 488                                                   
      DO 70 JK=1,NOAX                                                           
 70   READ(ITAPE,55)PAXL(JK),(AMX(IZ,JK),AMY(IZ,JK),IZ=1,NORE)                  
 55   FORMAT(6F12.2/6F12.2/6F12.2/6F12.2/6F12.2/6F12.2/6F12.2/6F12.2)           
488   CALL PLOTS(0,0,8)                                                         
      IPLOT2=0                                                                  
      YMIN=0.                                                                   
      YMAX=0.                                                                   
      XMIN=0.                                                                   
      XMAX=0.                                                                   
      DO 496 I=1,MAX(ISTART,NARE)                                               
      NV=NOVR(I)                                                                
      DO 496 J=1,NV                                                             
      YMIN=AMIN1(YMIN,YCOV(J,I))                                                
      YMAX=AMAX1(YMAX,YCOV(J,I))                                                
      XMIN=AMIN1(XMIN,XCOV(J,I))                                                
      XMAX=AMAX1(XMAX,XCOV(J,I))                                                
496   CONTINUE                                                                  
498   CONTINUE                                                                  
      YMIN=AINT(YMIN-.99)                                                       
      XMIN=AINT(XMIN-.99)                                                       
      DDY=(YMAX-YMIN)/8.                                                        
      DDX=(XMAX-XMIN)/6.                                                        
      IF(DDY-DDX)501,501,502                                                    
501   DY=DDX                                                                    
      GO TO 503                                                                 
502   DY=DDY                                                                    
503   DY=AINT(DY+.99)                                                           
      IF(IPLOT2.EQ.1) GO TO 1000                                                
C###                                                                            
504   YOR=1.5-(YMIN/DY)+(8.-(YMAX-YMIN)/DY)/2.                                  
      XOR=0.0-(XMIN/DY)+(6.-(XMAX-XMIN)/DY)/2.                                  
      YO=AINT(YOR*10.+.99)                                                      
      YOR=YO/10.                                                                
      IF(IPLOT2.EQ.1) GO TO 1500                                                
C###                                                                            
505   CALL PLOT (0.,1.1,3)                                                      
      CALL PLOT (1.,1.1,2)                                                      
C     CALL NUMBER(0.,1.15,.14,ID,0.,4)                                          
      CALL SYMBOL(0.,.7,.1,'SCALE: ',INTEQ,0.,7)                                
      CALL SYMBOL(.4,.4,.1,'SECTION= ',INTEQ,0.,9)                              
      CALL NUMBER(999.,999.,.1,DY,0.,-1)                                        
      CALL SYMBOL(999.,999.,.1,' PER INCH',INTEQ,0.,9)                          
      IF(IOUT.EQ.1) GO TO 529                                                   
      CALL PLOT(2.,1.1,3)                                                       
      CALL PLOT(2.5,1.1,2)                                                      
      CALL SYMBOL(2.,1.15,.1,'LOADS:  P( ',INTEQ,0.,11)                         
      CALL NUMBER(999.,999.,.1,XCAX,0.,2)                                       
      CALL SYMBOL(999.,999.,.1,' , ',INTEQ,0.,3)                                
      CALL NUMBER(999.,999.,.1,YCAX,0.,2)                                       
      CALL SYMBOL(999.,999.,.1,' ) ',INTEQ,0.,3)                                
      HR=2.5                                                                    
      VY=.9                                                                     
      DO 528 I=1,NOAX                                                           
      XI=I*1.                                                                   
      IF(I.GT.10) GO TO 528                                                     
      CALL NUMBER(HR,VY,.1,XI,0.,0)                                             
      HOR=HR+.3                                                                 
      CALL NUMBER(HOR,VY,.1,PAXL(I),0.,2)                                       
      IF(I.NE.5) GO TO 518                                                      
      VY=.9                                                                     
      HR=4.5                                                                    
      GO TO 528                                                                 
518   VY=VY-.2                                                                  
528   CONTINUE                                                                  
529   CONTINUE                                                                  
      IF(IBIG2.EQ.1) GO TO 6100                                                 
      HT0=9.7                                                                   
531   HT1=HT0                                                                   
      HT22=HT0-.05                                                              
      HT33=HT0-.35                                                              
      CALL SYMBOL(0.,HT1,.14,'ULT2AX.PLOT',INTEQ,0.,11)                         
      CALL PLOT(0.,HT22,3)                                                      
      CALL PLOT(2.95,HT22,2)                                                    
      CALL SYMBOL(3.0,HT22,.1,'WASHINGTON STATE DEPT. OF TRANS.',INTEQ,0        
     *.,32)                                                                     
      CALL SYMBOL(0.,HT33,.1,'DATE: ',INTEQ,0.,6)                               
      CALL SYMBOL(999.,999.,.1,DATE,INTEQ,0.,8)                                 
      IF(IBIG2.EQ.1) GO TO 1701                                                 
      CALL PLOT(XOR,YOR,-3)                                                     
      DO 549 I=1,MAX(ISTART,NARE)                                               
      NV=NOVR(I)                                                                
      DO 548 J=2,NV                                                             
      XL1=XCOV((J-1),I)/DY                                                      
      YL1=YCOV((J-1),I)/DY                                                      
      XL2=XCOV(J,I)/DY                                                          
      YL2=YCOV(J,I)/DY                                                          
      CALL PLOT(XL1,YL1,3)                                                      
      CALL PLOT(XL2,YL2,2)                                                      
548   CONTINUE                                                                  
      XI1=XCOV(1,I)/DY                                                          
      YI1=YCOV(1,I)/DY                                                          
      CALL PLOT(XL2,YL2,3)                                                      
549   CALL PLOT(XI1,YI1,2)                                                      
      IF(NBAL.LE.0) GO TO 578                                                   
      DO 558 I=1,NBAL                                                           
      XX1= XCBL1(I)/DY                                                          
      XX2= XCBL2(I)/DY                                                          
      YY1= YCBL1(I)/DY                                                          
      YY2= YCBL2(I)/DY                                                          
      CALL SYMBOL(XX1,YY1,.07,IBCD,004,0.,-1)                                   
      CALL PLOT(XX1,YY1,3)                                                      
      CALL PLOT(XX2,YY2,2)                                                      
      CALL SYMBOL(XX2,YY2,.07,IBCD,004,0.,-1)                                   
558   CONTINUE                                                                  
578   IF(NBAI.LE.0) GO TO 608                                                   
      DO 598 I=1,NBAI                                                           
      XI1=XCOBI(I)/DY                                                           
      YI1=YCOBI(I)/DY                                                           
598   CALL SYMBOL(XI1,YI1,.07,IBCD,004,0.,-1)                                   
608   IF(NSTR.EQ.0) GO TO 658                                                   
      DO 648 I=1,NSTR                                                           
      XI1=XSTR(I)/DY                                                            
      YI1=YSTR(I)/DY                                                            
648   CALL SYMBOL(XI1,YI1,.07,IBCD,000,0.,-1)                                   
658   CONTINUE                                                                  
      CALL PLOT(-XOR,-YOR,-3)                                                   
      CALL PLOT(-.5,0.,3)                                                       
      CALL PLOT(-.5,10.,2)                                                      
      CALL PLOT(6.5,10.,2)                                                      
      CALL PLOT(6.5,0.,2)                                                       
      CALL PLOT(-.5,0.,2)                                                       
      CALL PLOT(-.5,1.5,3)                                                      
      CALL PLOT(6.5,1.5,2)                                                      
      WRITE(ICARD,700)                                                          
      WRITE(OUNIT,700)                                                          
700   FORMAT('1','**********'//' ','PLOTTING REQUESTED WITH THIS OUTPUT'        
     1//' ','**********'//)                                                     
      WRITE(OUNIT,710)                                                          
710   FORMAT(' ','     USE 11 INCH WIDE PAPER (CROSS SECTION UNLESS OTHE        
     1RWISE SPECIFIED)'/' ','     START PEN AT THE LOWEST LINE OF GRID (        
     2OR .5 INCH ABOVE LOWER'/' ','        EDGE OF PAPER)'//)                   
      IF(IOUT.EQ.1) GO TO 1600                                                  
         YMAX=0.                                                                
         YMIN=0.                                                                
         XMAX=0.                                                                
         XMIN=0.                                                                
      DO 800 J=1,NOAX                                                           
      DO 800 I=1,NORE                                                           
         YMAX=AMAX1(YMAX,AMY(I,J))                                              
         YMIN=AMIN1(YMIN,AMY(I,J))                                              
         XMAX=AMAX1(XMAX,AMX(I,J))                                              
         XMIN=AMIN1(XMIN,AMX(I,J))                                              
 800     CONTINUE                                                               
       IPLOT2=1                                                                 
       GO TO 498                                                                
1000  CONTINUE                                                                  
        IF(DY.GT.10.) GO TO 1030                                                
        DY=10.                                                                  
        GO TO 1070                                                              
 1030   IF(DY.GT.20.) GO TO 1034                                                
        DY=20.                                                                  
        GO TO 1070                                                              
 1034   IF(DY.GT.30.) GO TO 1036                                                
        DY=30.                                                                  
        GO TO 1070                                                              
 1036   IF(DY.GT.40.) GO TO 1038                                                
        DY=40.                                                                  
        GO TO 1070                                                              
 1038   IF(DY.GT.50.) GO TO 1040                                                
        DY=50.                                                                  
        GO TO 1070                                                              
 1040   IF(DY.GT.60.) GO TO 1042                                                
        DY=60.                                                                  
        GO TO 1070                                                              
 1042   IF(DY.GT.100.) GO TO 1044                                               
        DY=100.                                                                 
        GO TO 1070                                                              
 1044   IF(DY.GT.200.) GO TO 1046                                               
        DY=200.                                                                 
        GO TO 1070                                                              
 1046   IF(DY.GT.400.) GO TO 1048                                               
        DY=400.                                                                 
        GO TO 1070                                                              
 1048   IF(DY.GT.500.) GO TO 1050                                               
        DY=500.                                                                 
        GO TO 1070                                                              
 1050   IF(DY.GT.1000.) GO TO 1052                                              
        DY=1000.                                                                
        GO TO 1070                                                              
 1052   IF(DY.GT.1200.) GO TO 1054                                              
        DY=1200.                                                                
        GO TO 1070                                                              
 1054   IF(DY.GT.2000.) GO TO 1056                                              
        DY=2000.                                                                
        GO TO 1070                                                              
 1056   IF(DY.GT.4000.) GO TO 1058                                              
        DY=4000.                                                                
        GO TO 1070                                                              
 1058   IF(DY.GT.5000.) GO TO 1062                                              
        DY=5000.                                                                
        GO TO 1070                                                              
 1062   IF(DY.GT.10000.) GO TO 1064                                             
        DY=10000.                                                               
        GO TO 1070                                                              
 1064   IF(DY.GT.20000.) GO TO 1084                                             
        DY=20000.                                                               
        GO TO 1070                                                              
C###                                                                            
 1070   SCAY1=AINT((YMAX-YMIN)/(DY-.99)+.99)                                    
        IF(SCAY1 .GT. 9.) GO TO 1084                                            
        IPLOT2=1                                                                
        GO TO 504                                                               
C###                                                                            
 1084   WRITE(ICARD,1086)DY                                                     
      WRITE(ICARD,1085)                                                         
1085  FORMAT('1','MOMENT CURVES NOT PLOTTED')                                   
 1086   FORMAT(/' ','SCALE Y =',F8.0,' WILL PLOT BEYOND A 11 IN. PAPER')        
      IOUT=1                                                                    
         GO TO 1600                                                             
1500  AOT=AINT(XOR * 2.)                                                        
      XOR=AOT * 0.5                                                             
      AOT=AINT(YOR * 2.)                                                        
      YOR=AOT * 0.5                                                             
        CALL SYMBOL(.4,.2,.1,'MOMENTS= ',INTEQ,0.,9)                            
        CALL NUMBER(999.,999.,.1,DY,0.,-1)                                      
        CALL SYMBOL(999.,999.,.1,' PER INCH',INTEQ,0.,9)                        
         CALL PLOT(XOR,YOR,-3)                                                  
         CALL SYMBOL(0.,0.,.50,IBCD,003,0.,-1)                                  
         XLF=XMIN/DY                                                            
         XRT=XMAX/DY                                                            
         YDN=YMIN/DY                                                            
         YUP=YMAX/DY                                                            
         CALL PLOT(XLF,0.,3)                                                    
         CALL PLOT(XRT,0.,2)                                                    
         CALL SYMBOL(XRT,0.,.14,' + M',INTEQ,0.,4)                              
         CALL SYMBOL(999.,999.,.1,'XX',INTEQ,0.,2)                              
         YUP1=YUP+.10                                                           
         CALL PLOT(0.,YDN,3)                                                    
         CALL PLOT(0.,YUP,2)                                                    
         CALL SYMBOL(0.,YUP1,.14,' + M',INTEQ,90.,4)                            
         CALL SYMBOL(999.,999.,.1,'YY',INTEQ,90.,2)                             
         ICN=0                                                                  
      DO 1300 J=1,NOAX                                                          
         ICN=ICN + 1                                                            
      IF (INC.GE.NORE) INC=1                                                    
      DO 1200 I=2,NORE                                                          
      XL1=AMX((I-1),J)/DY                                                       
      YL1=AMY((I-1),J)/DY                                                       
      XL2=AMX(I,J)/DY                                                           
      YL2=AMY(I,J)/DY                                                           
      CALL PLOT(XL1,YL1,3)                                                      
      CALL PLOT(XL2,YL2,2)                                                      
 1200    CONTINUE                                                               
         AOJ=ICN*1.                                                             
         IOO=ICN + 1                                                            
         CX=AMX((IOO),J)/DY                                                     
         CY=AMY((IOO),J)/DY                                                     
         ADDX= 0.1                                                              
         ADDY= 0.1                                                              
         IF(CX.LT.0.)ADDX= -0.1                                                 
         IF(CY.LT.0.)ADDY= -0.1                                                 
         CX= CX + ADDX                                                          
         CY= CY + ADDY                                                          
1300     CALL NUMBER(CX,CY,.10,AOJ,45.,-1)                                      
         CALL PLOT(-XOR,-YOR,-3)                                                
      IF(IBIG2.EQ.1) GO TO 505                                                  
1600     CALL PLOT(6.60,0.,3)                                                   
         CALL SYMBOL(6.60,.3,.07,'N.J.C. - REVISION DATE:  4-24-86',INTE        
     *    Q,90.,32)                                                             
      IF(IOUT.EQ.1) GO TO 1800                                                  
1701     AORE=NORE*1.                                                           
      CALL SYMBOL(.5,1.6,.1,'N.A. HAS BEEN ROTATED ',INTEQ,0.,22)               
      CALL NUMBER(999.,999.,.1,DEGR,0.,0)                                       
      CALL SYMBOL(999.,999.,.1,' DEGREES USING ',INTEQ,0.,15)                   
      CALL NUMBER(999.,999.,.1,AORE,0.,-1)                                      
      CALL SYMBOL(999.,999.,.1,' READINGS',INTEQ,0.,9)                          
      IF(IBIG2.EQ.1) GO TO 6300                                                 
      IF(DY.GE.2000.) GO TO 5000                                                
      IF(IBIG1.EQ.1) GO TO 5000                                                 
1800     CALL PLOT(8.,0.,999)                                                   
      IF(IOUT.EQ.0) GO TO 1807                                                  
      WRITE(ICARD,1805)                                                         
1805  FORMAT(' ','ULT2AX INPUT PLOTTED')                                        
      GO TO 9999                                                                
5000  SCAY=12.                                                                  
3033  DY=(YMAX-YMIN)/SCAY                                                       
      DY=AINT(DY+.99)                                                           
      DX=(XMAX-XMIN)/SCAY                                                       
      DX=AINT(DX+.99)                                                           
      IF(DX.GT.DY) DY=DX                                                        
      IF(DY.GT.10.) GO TO 3062                                                  
      DY=10.                                                                    
      GO TO 3069                                                                
3062  IF(DY.GT.20.) GO TO 3063                                                  
      DY=20.                                                                    
      GO TO 3069                                                                
3063  IF(DY.GT.30.) GO TO 3064                                                  
      DY=30.                                                                    
      GO TO 3069                                                                
3064  IF(DY.GT.40.) GO TO 3065                                                  
      DY=40.                                                                    
      GO TO 3069                                                                
3065  IF(DY.GT.50.) GO TO 3066                                                  
      DY=50.                                                                    
      GO TO 3069                                                                
3066  IF(DY.GT.60.) GO TO 3067                                                  
      DY=60.                                                                    
      GO TO 3069                                                                
3067  IF(DY.GT.100.) GO TO 3073                                                 
      DY=100.                                                                   
      GO TO 3069                                                                
3073  IF(DY.GT.200.) GO TO 3074                                                 
      DY=200.                                                                   
      GO TO 3069                                                                
3074  IF(DY.GT.400.) GO TO 3075                                                 
      DY=400.                                                                   
      GO TO 3069                                                                
3075  IF(DY.GT.500.) GO TO 3076                                                 
      DY=500.                                                                   
      GO TO 3069                                                                
3076  IF(DY.GT.1000.) GO TO 3077                                                
      DY=1000.                                                                  
      GO TO 3069                                                                
3077  IF(DY.GT.1200.) GO TO 3078                                                
      DY=1200.                                                                  
      GO TO 3069                                                                
3078  IF(DY.GT.2000.) GO TO 3079                                                
      DY=2000.                                                                  
      GO TO 3069                                                                
3079  IF(DY.GT.4000.) GO TO 3068                                                
      DY=4000.                                                                  
      GO TO 3069                                                                
3068  SCAY=SCAY+3.                                                              
      GO TO 3033                                                                
3069  SCAY1=AINT((YMAX-YMIN)/(DY-.99)+.99)                                      
      SCAYX=AINT((XMAX-XMIN)/(DY-.99)+.99)                                      
      IF(SCAY1.GT.28.) GO TO 3071                                               
      IF(SCAYX.GT.36.) GO TO 3071                                               
      SCAY=SCAY1                                                                
      GO TO 3043                                                                
3071  WRITE(ICARD,3072)DY                                                       
3072  FORMAT('1','SCALE =',F5.0,' WILL PLOT BEYOND A 29X36 IN. PAPER')          
      GO TO 9999                                                                
3043  CALL PLOT(10.,0.,-3)                                                      
      YOR=1.5-(YMIN/DY)                                                         
      XOR=1.0-(XMIN/DY)                                                         
      YO=AINT(YOR*10.+.99)                                                      
      YOR=YO/10.                                                                
      IBIG2=1                                                                   
      GO TO 1500                                                                
6100  HT0=SCAY+2.5                                                              
      GO TO 531                                                                 
6300  TOT=SCAYX+2.                                                              
      IF(TOT.LT.6.5) TOT=6.5                                                    
      CALL PLOT(-.5,0.,3)                                                       
      HI3=SCAY+3.                                                               
      CALL PLOT(-.5,HI3,2)                                                      
      CALL PLOT(TOT,HI3,2)                                                      
      CALL PLOT(TOT,0.,2)                                                       
      CALL PLOT(-.5,0.,2)                                                       
      TTT=TOT+.1                                                                
      CALL PLOT(TTT,0.,3)                                                       
      CALL SYMBOL(TTT,.3,.07,'N.J.C. - REVISION DATE:  4-24-86',INTEQ,90        
     * ,32)                                                                     
      TOT2=TOT+5.                                                               
      CALL PLOT(TOT2,0.,-3)                                                     
      N99=11                                                                    
      N22=22                                                                    
      N29=29                                                                    
      IF(SCAY.GT.9.) GO TO 2193                                                 
      WRITE(OUNIT,2192)N99                                                      
2192  FORMAT(/'     USE ',I2,' INCH WIDE CROSS SECTION PAPER INSTEAD')          
      GO TO 2195                                                                
2193  IF(SCAY.GT.20.) GO TO 2194                                                
      WRITE(OUNIT,2192)N22                                                      
      GO TO 2195                                                                
2194  WRITE(OUNIT,2192)N29                                                      
2195  GO TO 1800                                                                
1807     WRITE(ICARD,1899)                                                      
 1899    FORMAT(' ','ULT2AX INPUT-OUTPUT PLOTTED')                              
 9999    RETURN                                                                 
         END                                                                    
C                                                                               
C                                                                               
C                                                                               
C                                                                               
C                                                                               
C                                                                               
      SUBROUTINE PARM(OUNIT,ERUNIT)                                             
      CHARACTER*16 FLNM1, FLNM2, FLNM3                                          
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
C  STANDARD ERROR OUTPUT UNIT                                                   
      ERUNIT=6                                                                  
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC                                           
C     OPEN(IIUNIT,FILE='CON')                                                   
C     OPEN(IOUNIT,FILE='CON')                                                   
10    WRITE(*,210)                                                              
210   FORMAT(//,                                                                
     1' Enter input file name:',/)                                              
           READ(*,ERR=10,FMT='(A)')FLNM1                                        
           IF(FLNM1.EQ.'CON')GO TO 25                                           
           IF(FLNM1.EQ.'con')GO TO 25                                           
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
           IF(FLNM2.EQ.'CON')GO TO 60                                           
           IF(FLNM2.EQ.'con')GO TO 60                                           
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
60        WRITE(*,FMT='(/,1X,A16,'' used for output  '',/// )')FLNM2            
C65    WRITE(*,265)                                                             
C265   FORMAT(//,                                                               
C     1' Enter plot file name:',/)                                              
C        READ(*,ERR=65,FMT='(A)')FLNM3                                          
C           DO 70 I=1,16                                                        
C            IF(FLNM3(I:I).EQ.'.')GOTO 90                                       
C70          CONTINUE                                                           
C           DO 80 I=1,16                                                        
C            IF(FLNM3(I:I).EQ.' ')THEN                                          
C             IE = I+3                                                          
C             FLNM3(I:IE) = '.plo'                                              
C             GOTO 90                                                           
C            END IF                                                             
C80         CONTINUE                                                            
C90      WRITE(*,FMT='(/,'' '',1X,A16,'' used for plot file'',///)')FLNM        
C          OPEN(8,FILE=FLNM3,STATUS='NEW')                                      
          OPEN(OUNIT,FILE=FLNM2,STATUS='NEW')                                   
          OPEN(INUNIT,FILE=FLNM1,STATUS='OLD')                                  
      IF(FLNM1.EQ.'CON'.OR.FLNM1.EQ.'con')IIUNIT=5                              
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')ERUNIT=6                              
      IF(FLNM2.EQ.'CON'.OR.FLNM2.EQ.'con')IOUNIT=6                              
      OPEN(7,STATUS='SCRATCH')                                                  
      RETURN                                                                    
      END                                                                       
C                                                                               
C                                                                               
C                                                                               
C                                                                               
C                  TEMPORARY FAKE PLOT CALLS TO FOOL PC COMPILER                
C	In addition to this, argument checking is turned off in settings to enable a clean
C	compile without 300 mismatched errors that mean nothing because these functions are fake.                                                                      
         
      SUBROUTINE PLOTS (RL1,IN1,IN2)                                                
      IMPLICIT REAL*8 (A-H,O-Z) 
	OPTIONAL IN2                                                
      RETURN                                                                    
      END                                                                       
      SUBROUTINE PLOT (RL1,RL2,IN1)                                             
      IMPLICIT REAL*8 (A-H,O-Z)                                                 
      RETURN                                                                    
      END                                                                       
      SUBROUTINE SYMBOL (RL1,RL2,RL3,CAR1,RL4,IN1,IN2)                               
      IMPLICIT REAL*8 (A-H,O-Z)                      
	OPTIONAL IN2                          
      CHARACTER*40 CAR1                                                         
      RETURN                                                                    
      END                                                                       
      SUBROUTINE NUMBER (RL1,RL2,RL3,RL4,RL5,IN1)                              
      IMPLICIT REAL*8 (A-H,O-Z)                                                
      RETURN                                                                   
      END                                                                      
                                                                        
