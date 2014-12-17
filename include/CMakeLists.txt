file(GLOB headers "moneta/*.h*")
install(FILES ${headers} DESTINATION ${MONETA_INCLUDE_INSTALL_PATH}/moneta COMPONENT dev)
