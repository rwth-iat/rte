<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">

<xsl:output method="text"/>

<xsl:template match="text()|@*"/>

<xsl:template match="ACPLT_OV_InstanceModel">
          <xsl:for-each select="Instance">
	      	  <xsl:call-template name="Instance"/>
	  </xsl:for-each>
</xsl:template>

<xsl:template name="Instance">
<xsl:text>INSTANCE </xsl:text><xsl:value-of select="@ID"/><xsl:text> : CLASS </xsl:text><xsl:value-of select="@ClassID"/>
<xsl:text>
</xsl:text>
<xsl:if test="@CreationTime">
<xsl:text>      CREATION_TIME = </xsl:text><xsl:value-of select="@CreationTime"/><xsl:text>;
</xsl:text>
</xsl:if>
<xsl:if test="@Flags">
<xsl:text>      FLAGS         = "</xsl:text><xsl:value-of select="@Flags"/><xsl:text>";
</xsl:text>
</xsl:if>
<xsl:if test="@Comment">
<xsl:text>      COMMENT       = "</xsl:text><xsl:value-of select="@Comment"/><xsl:text>";
</xsl:text>
</xsl:if>
<xsl:if test="VariableValue">
<xsl:text>      VARIABLE_VALUES
</xsl:text>
<xsl:for-each select="VariableValue">
<xsl:call-template name="VariableValue"/>
</xsl:for-each>
<xsl:text>      END_VARIABLE_VALUES;
</xsl:text>
</xsl:if>
<xsl:if test="Instance">
<xsl:text>      PART_INSTANCES
</xsl:text>
<xsl:for-each select="Instance">
<xsl:call-template name="PartInstance"/>
</xsl:for-each>
<xsl:text>      END_PART_INSTANCES;
</xsl:text>
</xsl:if>
<xsl:if test="LinkValue">
<xsl:text>      LINK_VALUES
</xsl:text>
<xsl:for-each select="LinkValue">
<xsl:call-template name="LinkValue"/>
</xsl:for-each>
<xsl:text>      END_LINK_VALUES;
</xsl:text>
</xsl:if>
<xsl:apply-templates/>
<xsl:text>END_INSTANCE;

</xsl:text>
</xsl:template>

<xsl:template name="PartInstance">
<xsl:text>PART_INSTANCE </xsl:text><xsl:value-of select="@ID"/><xsl:text> : CLASS </xsl:text><xsl:value-of select="@ClassID"/>
<xsl:text>
</xsl:text>
<xsl:if test="@CreationTime">
<xsl:text>      CREATION_TIME = </xsl:text><xsl:value-of select="@CreationTime"/><xsl:text>;
</xsl:text>
</xsl:if>
<xsl:if test="@Flags">
<xsl:text>      FLAGS         = "</xsl:text><xsl:value-of select="@Flags"/><xsl:text>";
</xsl:text>
</xsl:if>
<xsl:if test="@Comment">
<xsl:text>      COMMENT       = "</xsl:text><xsl:value-of select="@Comment"/><xsl:text>";
</xsl:text>
</xsl:if>
<xsl:if test="VariableValue">
<xsl:text>      VARIABLE_VALUES
</xsl:text>
<xsl:for-each select="VariableValue">
<xsl:call-template name="VariableValue"/>
</xsl:for-each>
<xsl:text>      END_VARIABLE_VALUES;
</xsl:text>
</xsl:if>
<xsl:if test="Instance">
<xsl:text>      PART_INSTANCES
</xsl:text>
<xsl:for-each select="Instance">
<xsl:call-template name="PartInstance"/>
</xsl:for-each>
<xsl:text>      END_PART_INSTANCES;
</xsl:text>
</xsl:if>
<xsl:if test="LinkValue">
<xsl:text>      LINK_VALUES
</xsl:text>
<xsl:for-each select="LinkValue">
<xsl:call-template name="LinkValue"/>
</xsl:for-each>
<xsl:text>      END_LINK_VALUES;
</xsl:text>
</xsl:if>
<xsl:apply-templates/>
<xsl:text>END_PART_INSTANCE;

</xsl:text>
</xsl:template>


<xsl:template name="VariableValue">
<xsl:text>          </xsl:text><xsl:value-of select="@ID"/><xsl:text> = </xsl:text><xsl:value-of select="text()"/>
<xsl:if test="@Time">
<xsl:text>          TIME = </xsl:text><xsl:value-of select="@Time"/>
</xsl:if>
<xsl:if test="@State">
<xsl:text>          STATE = </xsl:text><xsl:value-of select="@State"/>
</xsl:if>
<xsl:if test="@Flags">
<xsl:text>          FLAGS = "</xsl:text><xsl:value-of select="@Flags"/><xsl:text>"</xsl:text>
</xsl:if>
<xsl:if test="@Unit">
<xsl:text>          UNIT = "</xsl:text><xsl:value-of select="@Unit"/><xsl:text>"</xsl:text>
</xsl:if>
<xsl:if test="@Comment">
<xsl:text>          COMMENT = "</xsl:text><xsl:value-of select="@Comment"/><xsl:text>"</xsl:text>
</xsl:if>
<xsl:text>;
</xsl:text>
</xsl:template>

<xsl:template name="LinkValue">
<xsl:text>          </xsl:text><xsl:value-of select="@ID"/><xsl:text> = {</xsl:text><xsl:value-of select="text()"/><xsl:text>}
</xsl:text>
</xsl:template>

</xsl:stylesheet>
