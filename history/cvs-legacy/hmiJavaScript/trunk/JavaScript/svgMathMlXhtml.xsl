<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
	xmlns="http://www.w3.org/1999/xhtml"
	xmlns:math="http://www.w3.org/1998/Math/MathML"
	xmlns:svg="http://www.w3.org/2000/svg"
	version="1.0">
<xsl:output method="xml" encoding="iso8859-1"/>
	<xsl:template match="/">
		<xsl:copy-of select="node()"/>
	</xsl:template>
</xsl:stylesheet>
