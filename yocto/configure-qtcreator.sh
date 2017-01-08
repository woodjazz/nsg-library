#!/bin/bash
############################################################################
##
## Copyright (C) 2016 The Qt Company Ltd.
## Contact: https://www.qt.io/licensing/
##
## This file is part of the Boot to Qt meta layer.
##
## $QT_BEGIN_LICENSE:GPL$
## Commercial License Usage
## Licensees holding valid commercial Qt licenses may use this file in
## accordance with the commercial license agreement provided with the
## Software or, alternatively, in accordance with the terms contained in
## a written agreement between you and The Qt Company. For licensing terms
## and conditions see https://www.qt.io/terms-conditions. For further
## information use the contact form at https://www.qt.io/contact-us.
##
## GNU General Public License Usage
## Alternatively, this file may be used under the terms of the GNU
## General Public License version 3 or (at your option) any later version
## approved by the KDE Free Qt Foundation. The licenses are as published by
## the Free Software Foundation and appearing in the file LICENSE.GPL3
## included in the packaging of this file. Please review the following
## information to ensure the GNU General Public License requirements will
## be met: https://www.gnu.org/licenses/gpl-3.0.html.
##
## $QT_END_LICENSE$
##
############################################################################
# Updated for nsg-library

set -e

ABI="arm-linux-generic-elf-32bit"
CONFIG=""

printUsage ()
{
    echo "Usage: $0 --config <environment-setup-file> [--remove] [--qtcreator <path>] [--name <basename>]"
}

while test -n "$1"; do
  case "$1" in
    "--remove")
      REMOVEONLY=1
      ;;
    "--qtcreator")
      shift
      QTCREATOR=$1
      ;;
    "--config")
      shift
      CONFIG=$1
      ;;
    "--name")
      shift
      NAME=$1
      ;;
    *)
      printUsage
      exit 0
      ;;
  esac
  shift
done

if [ ! -f "$CONFIG" ]; then
   printUsage
   exit 1
fi

if [ -z "${QTCREATOR}" ]; then
    SDKTOOL="${HOME}/Qt/Tools/QtCreator/libexec/qtcreator/sdktool"
else
    SDKTOOL="${QTCREATOR}/libexec/qtcreator/sdktool"
fi
if [ ! -x ${SDKTOOL} ]; then
    echo "Cannot find 'sdktool' from QtCreator"
    printUsage
    exit 1
fi

source $CONFIG

MKSPEC=${QMAKESPEC}
MKSPECPATH=$(find ${OECORE_TARGET_SYSROOT} -name $(basename ${MKSPEC}))
if [ ! -d "${MKSPECPATH}" ]; then
    echo "Error: could not find mkspec ${MKSPEC} from the toolchain"
    exit 1
fi

#MACHINE=$(grep '^MACHINE' ${MKSPECPATH}/../../qdevice.pri | cut -d'=' -f2 | tr -d ' ')
MACHINE=beaglebone
RELEASE=$(qmake -query QT_VERSION)

NAME=${NAME:-"Custom Qt ${RELEASE} ${MACHINE}"}
BASEID="byos.${RELEASE}.${MACHINE}"

${SDKTOOL} rmKit --id ${BASEID}.kit 2>/dev/null || true
${SDKTOOL} rmQt --id ${BASEID}.qt || true
${SDKTOOL} rmTC --id ProjectExplorer.ToolChain.Gcc:${BASEID}a.tc || true
${SDKTOOL} rmTC --id ProjectExplorer.ToolChain.Gcc:${BASEID}b.tc || true
${SDKTOOL} rmDev --id ${BASEID}.dev || true
${SDKTOOL} rmDebugger --id ${BASEID}.debugger || true

if [ -n "${REMOVEONLY}" ]; then
    echo "Kit removed: ${NAME}"
    exit 0
fi

ABI="x86-linux-generic-elf-32bit"

${SDKTOOL} addTC \
    --id "ProjectExplorer.ToolChain.Gcc:${BASEID}a.tc" \
    --language "1" \
    --name "GCC (${NAME})" \
    --path "$(type -p ${OE_QMAKE_CC})" \
    --abi "${ABI}" 

${SDKTOOL} addTC \
    --id "ProjectExplorer.ToolChain.Gcc:${BASEID}b.tc" \
    --language "2" \
    --name "G++ (${NAME})" \
    --path "$(type -p ${OE_QMAKE_CXX})" \
    --abi "${ABI}" 

${SDKTOOL} addDebugger \
    --id "${BASEID}.debugger" \
    --name "GDB (${NAME})" \
    --engine "1" \
    --binary "$(type -p ${GDB})" \
    --abis "${ABI}"    

${SDKTOOL} addQt \
    --id "${BASEID}.qt" \
    --name "${NAME}" \
    --qmake "$(type -p qmake)" \
    --type "RemoteLinux.EmbeddedLinuxQt" 

${SDKTOOL} addDev \
    --id "${BASEID}.dev" \
    --name "Generic Linux Device" \
    --type "0" \
    --authentication "3" \
    --freePorts "10000-10100" \
    --host "192.168.7.2" \
    --origin "0" \
    --osType "GenericLinuxOsType" \
    --password "abcd" \
    --sshPort "22" \
    --timeout "10" \
    --uname "root"

${SDKTOOL} addKit \
    --id "${BASEID}.kit" \
    --name "${NAME}" \
    --debuggerengine 1 \
    --debugger ${GDB} \
    --devicetype GenericLinuxOsType \
    --sysroot ${SDKTARGETSYSROOT} \
    --Ctoolchain ProjectExplorer.ToolChain.Gcc:${BASEID}a.tc \
    --Cxxtoolchain ProjectExplorer.ToolChain.Gcc:${BASEID}b.tc \
    --qt "${BASEID}.qt" \
    --mkspec "${QMAKESPEC}" \


echo "Configured Qt Creator with new kit: ${NAME}"