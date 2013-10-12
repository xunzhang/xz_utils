if __name__ == '__main__':
    import sys
    import hashlib
    m = hashlib.md5(sys.argv[1])
    print m.hexdigest()
