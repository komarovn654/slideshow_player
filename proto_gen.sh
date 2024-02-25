#!/bin/bash

WAYLAND_SCANNER=$(pkg-config --variable=wayland_scanner wayland-scanner)

XDG_SHELL_PROTOCOL=/usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml
XDG_SHELL_PROTOCOL_HEADER=wayland_client/xdg-shell-client-protocol.h
XDG_SHELL_PROTOCOL_CODE=wayland_client/xdg-shell-protocol.c

$WAYLAND_SCANNER client-header $XDG_SHELL_PROTOCOL $XDG_SHELL_PROTOCOL_HEADER
$WAYLAND_SCANNER private-code $XDG_SHELL_PROTOCOL $XDG_SHELL_PROTOCOL_CODE