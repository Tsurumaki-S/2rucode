#!/bin/bash

DELETE_DIR="bin"

if [ -d "${DELETE_DIR}" ];then
	rm -r ${DELETE_DIR} \
	&& \
	cat<<-EOS

	"${DELETE_DIR} was deleted without issues."

	EOS

else
	cat<<-EOS

	There is no directory named "${DELETE_DIR}".
	This program finished without changing anything.

	EOS

fi
