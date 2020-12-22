#!/bin/sh

prefix=/usr/local
debugsym=true

for arg in "$@"; do
    case "$arg" in
    --prefix=*)
        prefix=`echo $arg | sed 's/--prefix=//'`
        ;;

    --help)
        echo 'usage: ./configure [options]'
        echo 'options:'
        echo '  --prefix=<path>: installation prefix'
        echo 'all invalid options are silently ignored'
        exit 0
        ;;
    esac
done

echo 'generating Makefile ...'
echo "PREFIX = $prefix" >Makefile
if $debugsym; then
    echo 'dbg = -g' >>Makefile
fi
cat Makefile.in >>Makefile
echo 'configuration complete, type make to build.'
