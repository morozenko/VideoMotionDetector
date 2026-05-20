<xsl:stylesheet version="1.0" 
                xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
                xmlns:wix="http://wixtoolset.org">

  <xsl:output method="xml" indent="yes" />

  <!-- Копіювати все -->
  <xsl:template match="@*|node()">
    <xsl:copy>
      <xsl:apply-templates select="@*|node()"/>
    </xsl:copy>
  </xsl:template>

  <!-- Виключити конкретну DLL -->
  <xsl:template match="wix:Component[contains(wix:File/@Source, 'libstdc++-6.dll')]" />

</xsl:stylesheet>