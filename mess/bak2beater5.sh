#! /bin/sh
# backup local folder /home/xunzhang/ to beater5:~/xunzhang
echo 'start running bak2beater5'
echo 'bandwidth may be full, be patient'
rsync -avrz /home/xunzhang wuhong@beater5.intra.douban.com:~/bakT430
echo 'finish running bak2beater5'
echo 'happy coding'
