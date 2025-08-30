#!/bin/sh
chmod 755 /opt/cmakeg/cmakeg

chmod -R a+r /opt/cmakeg/assets
find /opt/cmakeg/assets -type d -exec chmod a+x {} \;

ln -sf /opt/cmakeg/cmakeg /usr/bin/cmakeg