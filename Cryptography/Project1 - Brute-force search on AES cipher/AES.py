# -*- coding: utf-8 -*-
"""
Created on Mon Feb 24 00:21:04 2020

@author: Mrudula
"""


#import json
#from base64 import b64encode, b64decode
#from Crypto.Cipher import AES
#from Crypto.Util.Padding import pad, unpad
#import time
import argparse
#from random import randint
#from python_utils import *
#from itertools import zip_longest
import utils_demo             # To import encrypt, decrypt, read to file, write to file functions
#import filecmp
from datetime import datetime # To compute total time taken

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Setup for Bruteforce attack against randomized AES-128-CTR.')
    parser.add_argument('-n', type=int,
                        help='Effective key length in bytes.', default=3)

    args = parser.parse_args()
    #The input value for brute force attack in bits. 24 bits is equal to 3 bytes.
    length_postfix = args.n * 8
    #Reading plaintexts,ciphertexts,nonces from files. 
    plaintext1 = utils_demo.read_file(fn = "files/m1.txt").encode()
    plaintext2 = utils_demo.read_file(fn = "files/m2.txt").encode()
    plaintext3 = utils_demo.read_file(fn = "files/m3.txt").encode()
    ciphertext1 = utils_demo.read_bytes(fn = "files/c1.bin")
    ciphertext2 = utils_demo.read_bytes(fn = "files/c2.bin")
    ciphertext3 = utils_demo.read_bytes(fn = "files/c3.bin")
    nonce1 = utils_demo.read_bytes(fn = "files/nonce1.bin")
    nonce2 = utils_demo.read_bytes(fn = "files/nonce2.bin")
    nonce3 = utils_demo.read_bytes(fn = "files/nonce3.bin")
    
    start_time = datetime.now() 
    for i in range(12634807,12634813):
    #for i in range(0,2**length_postfix-1): # because we have 2 raised to 24 possibilities
        #Defining the key for decryption.
        main_key = bin(2 ** 127 + i)
        #Decrypting cipher-texts.
        ptxt1 = utils_demo.decryptor_CTR(ctxt=ciphertext1, nonce=nonce1, key=utils_demo.bitstring_to_bytes(main_key))
        if(ptxt1 == plaintext1):
            print(ptxt1)
            ptxt2 = utils_demo.decryptor_CTR(ctxt=ciphertext2, nonce=nonce2, key=utils_demo.bitstring_to_bytes(main_key))
            if(ptxt2 == plaintext2):
                print(ptxt2)
                ptxt3 = utils_demo.decryptor_CTR(ctxt=ciphertext3, nonce=nonce3, key=utils_demo.bitstring_to_bytes(main_key))
                if(ptxt3 == plaintext3):
                    print(ptxt3)
                    utils_demo.write_file(fn = "files/key.bin", value = main_key) # writing the key to a bin file
                    print("Here is the key !!")
                    print(i)   # 12634810 - integer value, C0CABA - hex value is the final key
                    print(hex(i))
                    break
    time_elapsed = datetime.now() - start_time 
    print('Time elapsed (hh:mm:ss.ms) {}'.format(time_elapsed))    