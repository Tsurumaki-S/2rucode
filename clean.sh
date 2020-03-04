#!/bin/bash

for DELETE_DIR in "bin" "MAKE" ;do

	if [ -d "${DELETE_DIR}" ];then
		rm -r ${DELETE_DIR} \
		&& \
		cat<<-EOS

		"${DELETE_DIR}" was deleted without issues.
		EOS

	else
		cat<<-EOS

		There is no directory named "${DELETE_DIR}".
		EOS

	fi

done

for DELETE_FILE in "makefile" ;do

	if [ -e "${DELETE_FILE}" ];then
		rm ${DELETE_FILE} \
		&& \
		cat<<-EOS

		"${DELETE_FILE}" was deleted without issues.
		EOS

	else
		cat<<-EOS

		There is no file named "${DELETE_FILE}".
		EOS

	fi

done

echo ""
