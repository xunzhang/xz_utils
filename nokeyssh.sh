#! /bin/sh
# ./nokeyssh beater1
# arg $1 must be a IP address or a hostname
echo $1
cd ~/.ssh
if [ ! -f "id_rsa.pub"]; then
  ssh-keygen -t rsa
fi
ssh-copy-id $USER@$1
