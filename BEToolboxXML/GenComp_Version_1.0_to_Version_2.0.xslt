<?xml version="1.0"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="xml"/>

  <!-- 
This transform maps BEToolbox:GenComp version 1 files to BEToobox:GenComp version 2 files.
This transform can be safely applied to BEToolbox:GenComp version 2 files
-->

<!-- This template matches the document level element -->
<xsl:template match="/">
  <xsl:choose>
<!-- 
When the GenComp data is a version 1, perform the transform otherwise do an identity transform
-->
   <xsl:when test="BEToolbox/GenComp/@version = 1">
     <GenComp version="2">
       <!-- Update GenComp version number -->
       <xsl:apply-templates/>
     </GenComp>
   </xsl:when>
   <xsl:otherwise>
     <xsl:copy-of select="." />
   </xsl:otherwise>
  </xsl:choose>
</xsl:template>

<xsl:template match="Units">
   <Units>
   <xsl:choose>
      <xsl:when test="Units=1">SI</xsl:when>
      <xsl:otherwise>US</xsl:otherwise>
   </xsl:choose>
   </Units>
</xsl:template>

<xsl:template match="ModularRatio">
   <xsl:copy-of select="." />
</xsl:template>

<xsl:template match="PrimaryShape">
   <PrimaryShape version="2">
      <xsl:for-each select="Point">
         <xsl:copy-of select="." />
      </xsl:for-each>
   </PrimaryShape>
</xsl:template>

<xsl:template match="SecondaryShape">
   <SecondaryShape version="2">
      <xsl:for-each select="Point">
         <xsl:copy-of select="." />
      </xsl:for-each>
   </SecondaryShape>
</xsl:template>

<xsl:template match="PointCount" />
</xsl:stylesheet>
