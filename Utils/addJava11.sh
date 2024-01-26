#!/bin/bash

file=~/.bashrc

# Java 11
if ! grep -q "export PATH=/usr/lib/jvm/openjdk-11/bin" "$file"; then
    echo "export PATH=/usr/lib/jvm/openjdk-11/bin:\$PATH" >> "$file"
    echo "Importato Java 11 in $file"
else
    echo "Java 11 già presente in $file"
fi

# Aggiorna il PATH corrente nella sessione attuale
source "$file"
