<?xml version="1.0" encoding="utf-8" ?>
<xsl:stylesheet version="2.0"  xmlns:w="http://schemas.openxmlformats.org/wordprocessingml/2006/main" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:foo="http://www.foo.org/" xmlns:bar="http://www.bar.org">

<xsl:template match="/">

  <html>
    <head>
      <meta charset="UTF-8"></meta>
    </head>
    <body>
        <xsl:apply-templates>
        </xsl:apply-templates>
    </body>
  </html>
</xsl:template>

  <xsl:template match="w:p">
    <p>
      <xsl:attribute name="style"><xsl:text>font-size: </xsl:text><xsl:value-of select="w:pPr/w:rPr/w:sz/@w:val"></xsl:value-of><xsl:text>px </xsl:text></xsl:attribute>
      <xsl:apply-templates>
      </xsl:apply-templates>
    </p>
  </xsl:template>

<xsl:template match="w:t">

    <xsl:value-of select="."></xsl:value-of>

</xsl:template>
</xsl:stylesheet>
