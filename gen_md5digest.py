#! /usr/bin/python
# used for doubanm hpp files

if __name__ == '__main__':
    import sys
    import hashlib
    m = hashlib.md5(sys.argv[1])
    res = m.hexdigest()
    lst = ['FILE', res[:8], res[8:12], res[12:16], res[16:20], res[20:], 'H']
    print '_'.join(lst)
