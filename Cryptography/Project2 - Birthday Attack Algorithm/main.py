# -*- coding: utf-8 -*-
"""
Created on Wed Apr  1 13:35:07 2020

@author: anuma
"""

import sys                      # To access the list of commandline argumnets passed
import random                   # To choose random integer for generating input messages
import hashlib                  # Library that contains the implementation of SHA256
from datetime import datetime   # To compute total time taken


'''Initializing an empty list (charList) for all possible characters that can be part of the input'''
charList = [] 
charList = list(map(chr, range(32, 126)))   # Range of ASCII values of characters
charListLength = len(charList)              # length of the generated character list


'''Below is the implementation of binary tree structure to speedup the collision search '''
class Tree(): 
    # The varibale self refers to the object itself    
    # We do not give a value for this parameter when we call the method, Python will provide it
    
    # This __init__ method is run as soon as an object of a class is initiated
    def __init__(self, message='', hashValue=''):
        self.left = None
        self.right = None
        self.message = message
        self.hashValue = hashValue
        if (self.hashValue):
            self.id = int(self.hashValue, 16)  # integer representation of hash
        else:
            self.id = 0 

    # This function adds a new node to the tree
    def addNode(self, other):
        if (self.id > other.id):
            if (self.left):
                self.left.addNode(other)
            else:
                self.left = other
        else:
            if (self.right):
                self.right.addNode(other)
            else:
                self.right = other
                
    # This function finds a node (other) in the tree and returns it if found, else None
    def findNode(self, other):
        if (self.id == other.id):
            return self
        if (self.right and self.id < other.id):
            return self.right.findNode(other)
        if ( self.left and self.id > other.id):
            return self.left.findNode(other)

    # This function represents the node in string format
    def __str__(self):
        return '{} {}'.format(self.message, self.hashValue)
    

'''This Badhash40 function computes SHA256 hash of input message and returns the first 40 bits(5 bytes) of it.'''
def BadHash40(message):    
    if (isinstance(message, str)):    # Check to ensure that the message is a byte string
        message = message.encode('utf-8')    # encode() converts the string into bytes to be acceptable by hash function  
        hashString = hashlib.sha256(message).hexdigest()    # hexdigest() return the encoded data in hexadecimal format
    return hashString[0:10].upper()     # Returns the first 40 bits of SHA256 hash digest of the message in hex format


if __name__ == "__main__":    
    
    start_time = datetime.now() 
    
    # sys.argv list contains the command-line arguments passed to the script
    # Retrieves NN parameter if passed, which denotes the number of messages to be generated and checked against each other
    lhash = 0
    if (len(sys.argv) == 3 and sys.argv[1] == '-lhash'):
        lhash = int(sys.argv[2])   
        
    # This text file contains the list of random messages and their corresponding hash values 
    fileName = open('hash.data', 'w') 
    
    root = None
    stopIteration = True
    iterations = 1
    messagesCount = 0   
      
    # Generates list of random messages and hash values, and looks for collision in that list
    # Random messages are generated until the hash values of any two of them matches (when lhash isn't given)
    # If lhash is passed, random messages are generated until the lhash count is reached, if collision is not found it restarts 
    while stopIteration:
        message = ''
        messageLength = 32   # Message of length 256 bits, Update to change the input message length
        for i in range(messageLength):  
            message += charList[random.randint(0, charListLength-1)]    # Generates a random message
        messagesCount += 1
        hashValue = BadHash40(message)
        node = Tree(message, hashValue)
        if (root is None):
            root = node
            continue        
        print(node, file=fileName, flush=True)   # Writes the random message and its hash value to the file
        collisionNode = root.findNode(node)
        if (collisionNode and node.message != collisionNode.message):
            stopIteration = False 
            print(node)
            print(collisionNode)                      
            print('Collision is found in iteration number {}.'.format(iterations))
            print('Hex format of the two messages are:')
            print('Message_1 : {}'.format(node.message[0:messageLength].encode('utf-8').hex().upper()))
            print('Message_2 : {}'.format(collisionNode.message[0:messageLength].encode('utf-8').hex().upper()))
            print('And their output is:')
            print('Output : {}'.format(node.hashValue))
        else:
            root.addNode(node)  # Add node to the tree if collision is not found
        # Collision is not in the list, program will start over    
        if (lhash > 0 and messagesCount >= lhash):
            iterations += 1
            iterationCount = 0            
            root = None
    fileName.close()
    time_elapsed = datetime.now() - start_time 
    if (lhash == 0):
        print('lhash = 0 and the numner of messages in the list : {}.'.format(messagesCount))
    print('Time elapsed (hh:mm:ss.ms) {}'.format(time_elapsed))