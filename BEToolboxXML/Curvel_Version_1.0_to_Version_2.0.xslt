<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml"/>

  <!-- 
This transform maps BEToolbox:Curvel version 1 files to BEToobox:Curvel version 2 files.
This transform can be safely applied to BEToolbox:Curvel version 2 files
-->

  <!-- This template matches the document level element -->
  <xsl:template match="/">
    <xsl:choose>
      <!-- 
When the Curvel data is a version 1, perform the transform otherwise do an identity transform
-->
      <xsl:when test="BEToolbox/Curvel/@version = 1">
        <Curvel version="2">
          <!-- Update Curvel version number -->
          <xsl:apply-templates/>
        </Curvel>
      </xsl:when>
      <xsl:otherwise>
        <xsl:copy-of select="." />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <!-- Map Vertical Curve Data into the new document (no changes) -->
  <xsl:template match="VerticalCurveData">
    <xsl:copy-of select="." />
  </xsl:template>


  <!-- Map Superelevation Data. If CorrectForSuperelevation has a value of -1 map the superelevation 
data, otherwise omit it -->
  <xsl:template match="SuperelevationData">
    <xsl:if test="CorrectForSuperelevation = -1">
      <SuperelevationData version="2">
        <!-- update version number because child elements have changed -->
        <xsl:copy-of select="ProfileGradeOffset"/>
        <!-- no change to ProfileGradeOffset -->
        <xsl:apply-templates/>
      </SuperelevationData>
    </xsl:if>
  </xsl:template>

  <!-- Map SuperelevationProfilePoint data -->
  <xsl:template match="SuperelevationProfilePoint">
    <CrownSlope version="1">
      <!-- Added CrownSlope element -->
        <xsl:call-template name="NewSuperelevationProfilePoint"/>
    </CrownSlope>
  </xsl:template>

  <xsl:template name="NewSuperelevationProfilePoint">
    <SuperelevationProfilePoint version="2">
      <xsl:call-template name="PPStation1" />
  </SuperelevationProfilePoint>
    <SuperelevationProfilePoint version="2">
      <xsl:call-template name="PPStation2" />
    </SuperelevationProfilePoint>
    <SuperelevationProfilePoint version="2">
      <xsl:call-template name="PPStation3" />
    </SuperelevationProfilePoint>
  </xsl:template>

  <!-- Map Superelevation Profile Point Data (Station, Left Slope, Right Slope)
     Use explicit XPath to the nodes so that the transform doesn't get the other Station nodes -->
  <xsl:template name="PPStation1">
    <xsl:copy-of select="/BEToolbox/Curvel/SuperelevationData/SuperelevationProfilePoint/Station[position()=1]"/>
    <xsl:call-template name="PPLeftSlope1" />
  </xsl:template>
  <xsl:template name="PPStation2">
    <xsl:copy-of select="/BEToolbox/Curvel/SuperelevationData/SuperelevationProfilePoint/Station[position()=2]"/>
    <xsl:call-template name="PPLeftSlope2" />
  </xsl:template>
  <xsl:template name="PPStation3">
    <xsl:copy-of select="/BEToolbox/Curvel/SuperelevationData/SuperelevationProfilePoint/Station[position()=3]"/>
    <xsl:call-template name="PPLeftSlope3" />
  </xsl:template>

  <xsl:template name="PPLeftSlope1">
    <xsl:copy-of select="/BEToolbox/Curvel/SuperelevationData/SuperelevationProfilePoint/LeftSlope[position()=1]"/>
    <xsl:call-template name="PPRightSlope1" />
  </xsl:template>
  <xsl:template name="PPLeftSlope2">
    <xsl:copy-of select="/BEToolbox/Curvel/SuperelevationData/SuperelevationProfilePoint/LeftSlope[position()=2]"/>
    <xsl:call-template name="PPRightSlope2" />
  </xsl:template>
  <xsl:template name="PPLeftSlope3">
    <xsl:copy-of select="/BEToolbox/Curvel/SuperelevationData/SuperelevationProfilePoint/LeftSlope[position()=3]"/>
    <xsl:call-template name="PPRightSlope3" />
  </xsl:template>

  <xsl:template name="PPRightSlope1">
    <xsl:copy-of select="/BEToolbox/Curvel/SuperelevationData/SuperelevationProfilePoint/RightSlope[position()=1]"/>
  </xsl:template>
  <xsl:template name="PPRightSlope2">
    <xsl:copy-of select="/BEToolbox/Curvel/SuperelevationData/SuperelevationProfilePoint/RightSlope[position()=2]"/>
  </xsl:template>
  <xsl:template name="PPRightSlope3">
    <xsl:copy-of select="/BEToolbox/Curvel/SuperelevationData/SuperelevationProfilePoint/RightSlope[position()=3]"/>
  </xsl:template>

  <!-- Map the Individual Station data -->
  <xsl:template match="/BEToolbox/Curvel/IndividualStations">
    <xsl:if test="NumPoints != 0">
      <IndividualStations version="2">
        <!-- update version number, eliminated the NumPoints element-->
        <xsl:for-each select="IndividualStation">
          <xsl:copy-of select="." />
        </xsl:for-each>
      </IndividualStations>
    </xsl:if>
  </xsl:template>

  <!-- Map station ranges -->
  <xsl:template match="StationRanges">
    <xsl:if test="NumPoints != 0">
      <StationRanges version="2">
        <!-- update version number, eliminated the NumPoints element-->
        <xsl:for-each select="StationRange">
          <xsl:copy-of select="." />
        </xsl:for-each>
      </StationRanges>
    </xsl:if>
  </xsl:template>

  <!-- Map skew lines -->
  <xsl:template match="SkewLines">
    <xsl:if test="NumPoints != 0">
      <SkewLines version="2">
        <!-- update version number, eliminated the NumPoints element-->
        <xsl:for-each select="SkewLine">
          <SkewLine version="2">
            <!-- Changed OffsetType from 0 or 1 to a keyword -->
            <Station>
              <xsl:value-of select="Station" />
            </Station>
            <OffsetType>
              <xsl:choose>
                <xsl:when test="OffsetType = 0">RadialFromCrownLine</xsl:when>
                <xsl:otherwise>AlongSkewFromLLine</xsl:otherwise>
              </xsl:choose>
            </OffsetType>
            <Offset>
              <xsl:value-of select="Offset" />
            </Offset>
            <SkewAngle>
              <xsl:value-of select="SkewAngle" />
            </SkewAngle>
            <Radius>
              <xsl:value-of select="Radius" />
            </Radius>
            <CrownOffset>
              <xsl:value-of select="CrownOffset" />
            </CrownOffset>
          </SkewLine>
        </xsl:for-each>
      </SkewLines>
    </xsl:if>
  </xsl:template>

  <!-- These elements no longer exist, map them to nothing -->
  <xsl:template match="CorrectForSuperelevation" />
  <xsl:template match="ProfileGradeOffset" />
  <xsl:template match="NumPoints" />
  <xsl:template match="/BEToolbox/Curvel/SuperelevationData/SuperelevationProfilePoint/Station" />
  <xsl:template match="/BEToolbox/Curvel/SuperelevationData/SuperelevationProfilePoint/LeftSlope" />
  <xsl:template match="/BEToolbox/Curvel/SuperelevationData/SuperelevationProfilePoint/RightSlope" />

</xsl:stylesheet>
