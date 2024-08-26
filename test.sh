#!/bin/bash

test_cd() {
    echo "Testing: cd $1"
    ./minishell <<EOF
cd $1
pwd
echo "Exit status: \$?"
EOF
    echo "-------------------"
}

test_cd "/home"
test_cd "/usr/local/bin"
test_cd ".."
test_cd "../.."
test_cd "Documents"
test_cd "/home/$USER/Documents/../Downloads"
test_cd "/"
test_cd "/path/that/does/not/exist"
test_cd "/etc/passwd"
test_cd "My Documents"
test_cd "~"
test_cd "~/Documents"