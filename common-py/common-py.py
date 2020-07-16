# -*- coding: UTF-8 -*-
import sys
import os
import struct
import argparse
import requests
import crypto
# for crypto
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

in_file = u"../data-io/测试报.txt"    # u"xxx" for 中文路径文件

'''
    读取二进制文件，并处理
'''
def read_file(in_file):
    '''
    #方法1：
    f_in = open(in_file, "rb")
    for i in range(in_file_size):
        data = f_in.read(1)      #每次输出一个字节
    f_in.close()
    '''
    # 方法2：
    with open(in_file, "rb") as f:
        byte_infile_buf = f.read()
        # 得到文件大小：
        in_file_size = os.path.getsize(in_file)
    # 打印文件二进制内容
    print "字符串形式显示文件内容："
    print(byte_infile_buf)
    print
    print "二进制形式显示文件内容："
    f_in = open(in_file, 'rb')
    f_in.seek(0, 0)
    index = 0
    for i in range(0, 16):
        print "%3s" % hex(i),
    print
    for i in range(0, 16):
        print "%-3s" % "#",
    print
    while True:
        tmp_onebyte = f_in.read(1)
        if len(tmp_onebyte) == 0:
            break
        else:
            print "%3s" % tmp_onebyte.encode('hex'),
            index += 1
        if index == 16:
            index = 0
            print
    f_in.close()

if __name__ == '__main__':
    print 'hello, python world'
    print in_file
    print
    # 打开文件读取：
    read_file(in_file)


