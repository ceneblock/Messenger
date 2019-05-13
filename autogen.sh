#!/bin/sh
autoreconf --install

aclocal

touch NEWS README AUTHORS ChangeLog

automake --add-missing --copy >/dev/null 2>&1
