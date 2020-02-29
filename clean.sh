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

echo ""
