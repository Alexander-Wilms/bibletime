#handbook (install images from en/ to all languages)
FILE(GLOB INSTALL_HANDBOOK_IMAGES "${CMAKE_CURRENT_SOURCE_DIR}/docs/handbook/en/html/*.png")
FOREACH(HANDBOOK_LOCALE_LANG ${HANDBOOK_LOCALE_LANGS} "en")
    FILE(GLOB INSTALL_HANDBOOK_HTML_FILES_${HANDBOOK_LOCALE_LANG} "${CMAKE_CURRENT_SOURCE_DIR}/docs/handbook/${HANDBOOK_LOCALE_LANG}/html/*.html")
    INSTALL(FILES ${INSTALL_HANDBOOK_HTML_FILES_${HANDBOOK_LOCALE_LANG}}
        DESTINATION "${BT_SHARE_PATH}bibletime/docs/handbook/${HANDBOOK_LOCALE_LANG}/"
    )
    INSTALL(FILES ${INSTALL_HANDBOOK_IMAGES}
        DESTINATION "${BT_SHARE_PATH}bibletime/docs/handbook/${HANDBOOK_LOCALE_LANG}/"
    )
ENDFOREACH(HANDBOOK_LOCALE_LANG ${HANDBOOK_LOCALE_LANGS} "en")

#howto (does not have images)
FOREACH(HOWTO_LOCALE_LANG ${HOWTO_LOCALE_LANGS} "en")
    FILE(GLOB INSTALL_HOWTO_HTML_FILES_${HOWTO_LOCALE_LANG} "${CMAKE_CURRENT_SOURCE_DIR}/docs/howto/${HOWTO_LOCALE_LANG}/html/*.html")
    INSTALL(FILES ${INSTALL_HOWTO_HTML_FILES_${HOWTO_LOCALE_LANG}}
        DESTINATION "${BT_SHARE_PATH}bibletime/docs/howto/${HOWTO_LOCALE_LANG}/"
    )
ENDFOREACH(HOWTO_LOCALE_LANG ${HOWTO_LOCALE_LANGS} "en")

IF(CMAKE_SYSTEM MATCHES "BSD")
	SET(BT_DOCBOOK_XSL "${CMAKE_CURRENT_SOURCE_DIR}/cmake/docs/docs_freebsd.xsl")
ELSE(CMAKE_SYSTEM MATCHES "BSD")
	SET(BT_DOCBOOK_XSL "${CMAKE_CURRENT_SOURCE_DIR}/cmake/docs/docs.xsl")
ENDIF(CMAKE_SYSTEM MATCHES "BSD")

# Update handbook
ADD_CUSTOM_TARGET("handbook")

ADD_CUSTOM_TARGET("handbook_translations"
	COMMAND po4a -v --no-backups -k 0 cmake/docs/handbook_po4a.conf
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

FOREACH(HANDBOOK_LOCALE_LANG ${HANDBOOK_LOCALE_LANGS} "en")
	ADD_CUSTOM_TARGET("handbook_${HANDBOOK_LOCALE_LANG}"
		COMMAND xsltproc --stringparam l10n.gentext.default.language ${HANDBOOK_LOCALE_LANG} ${BT_DOCBOOK_XSL} ../docbook/index.docbook
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/docs/handbook/${HANDBOOK_LOCALE_LANG}/html/")
	ADD_DEPENDENCIES("handbook_${HANDBOOK_LOCALE_LANG}" "handbook_translations")
	ADD_DEPENDENCIES("handbook" "handbook_${HANDBOOK_LOCALE_LANG}")
ENDFOREACH(HANDBOOK_LOCALE_LANG ${HANDBOOK_LOCALE_LANGS})

IF(CMAKE_SYSTEM MATCHES "BSD")
	SET(BT_DOCBOOK_PDF_XSL "${CMAKE_CURRENT_SOURCE_DIR}/cmake/docs/pdf_freebsd.xsl")
ELSE(CMAKE_SYSTEM MATCHES "BSD")
    IF (APPLE)
    	SET(BT_DOCBOOK_PDF_XSL "${CMAKE_CURRENT_SOURCE_DIR}/cmake/docs/pdf_mac.xsl")
    ELSE (APPLE)
    	SET(BT_DOCBOOK_PDF_XSL "${CMAKE_CURRENT_SOURCE_DIR}/cmake/docs/pdf.xsl")
    ENDIF (APPLE)
ENDIF(CMAKE_SYSTEM MATCHES "BSD")

# Update handbook pdf
ADD_CUSTOM_TARGET("handbook_pdf")

FOREACH(HANDBOOK_LOCALE_LANG ${HANDBOOK_LOCALE_LANGS} "en")
	ADD_CUSTOM_TARGET("handbook_pdf_${HANDBOOK_LOCALE_LANG}"
	   COMMENT "Generating PDF handbook for ${HANDBOOK_LOCALE_LANG}"
	   COMMAND fop -xml ../../${HANDBOOK_LOCALE_LANG}/docbook/index.docbook -xsl ${BT_DOCBOOK_PDF_XSL} -pdf ../../${HANDBOOK_LOCALE_LANG}/pdf/handbook.pdf
	   WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/docs/handbook/en/html/")
	ADD_DEPENDENCIES("handbook_pdf" "handbook_pdf_${HANDBOOK_LOCALE_LANG}")
ENDFOREACH(HANDBOOK_LOCALE_LANG ${HANDBOOK_LOCALE_LANGS})

# Update howto
ADD_CUSTOM_TARGET("howto")
ADD_CUSTOM_TARGET("howto_translations"
	COMMAND po4a -v --no-backups -k 0 cmake/docs/howto_po4a.conf
	WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
ADD_DEPENDENCIES("howto" "howto_translations")


FOREACH(HOWTO_LOCALE_LANG ${HOWTO_LOCALE_LANGS} "en")
	ADD_CUSTOM_TARGET("howto_${HOWTO_LOCALE_LANG}"
		COMMAND xsltproc --stringparam l10n.gentext.default.language ${HOWTO_LOCALE_LANG} ${BT_DOCBOOK_XSL} "../docbook/index.docbook"
		WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/docs/howto/${HOWTO_LOCALE_LANG}/html/")
	ADD_DEPENDENCIES("howto_${HOWTO_LOCALE_LANG}" "howto_translations")
	ADD_DEPENDENCIES("howto" "howto_${HOWTO_LOCALE_LANG}")
ENDFOREACH(HOWTO_LOCALE_LANG ${HOWTO_LOCALE_LANGS})