<!--
  Generates chunked XHTML documents from DocBook XML source using DocBook XSL
  stylesheets.

  NOTE: The URL reference to the current DocBook XSL stylesheets is
  rewritten to point to the copy on the local disk drive by the XML catalog
  rewrite directives so it doesn't need to go out to the Internet for the
  stylesheets. This means you don't need to edit the <xsl:import> elements on
  a machine by machine basis.
-->
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/xhtml/chunkfast.xsl"/>
<xsl:import href="http://docbook.sourceforge.net/release/xsl/current/xhtml/math.xsl"/>

<xsl:param name="html.stylesheet" select="'resources/boostbook.css'"/>
<xsl:param name="navig.graphics" select="1"/>
<xsl:param name="navig.graphics.path">resources/</xsl:param>
<xsl:param name="navig.graphics.extension">.png</xsl:param>

<xsl:template name="user.header.navigation">
  <table cellpadding="2" width="100%"><tr>
    <td align="left">
      <img src="resources/currency_exchange.png" alt="currency"/>
    </td><td align="right">
      <a href="http://code.google.com/p/isomon/">code.google.com</a>
    </td>
  </tr></table>
</xsl:template>

<!-- Line break -->
<xsl:template match="processing-instruction('asciidoc-br')">
  <br/>
</xsl:template>

<!-- Horizontal ruler -->
<xsl:template match="processing-instruction('asciidoc-hr')">
  <hr/>
</xsl:template>

</xsl:stylesheet>
