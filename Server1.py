#!/usr/bin/env python
import socket , string, re,time,os
from threading import Thread 
from SocketServer import ThreadingMixIn 
from datetime import datetime

thshld = 400


# Multithreaded Python server : TCP Server Socket Thread Pool
class ClientThread(Thread): 
 
    def __init__(self,ip,port): 
        Thread.__init__(self) 
        self.ip = ip 
        self.port = port 
        if (os.system("ping -c 1 " + ip + " >/dev/null 2>&1")) == 1:
	  print "\033[1H [+]\033[1;38m + {}  is down : \033[0m".format(ip) 
        
        print "\033[1H [+]" + ip + ": \033[0m" + str(port) 
        
 
    def run(self): 
        while True :
	  

     	  try :
	  
	    data = conn.recv(2048)

            if "tmp" in data:
	      
              ID=(re.search(r'\d+',data).group())
              conn.send(ID)
              temp=(re.search(r'[-+]?\d*\.\d+',data).group())

              print "\033[3H\033[1;30m ID: {}	Tempearture \033[3;34m >> {}  \033[0;0m ".format(ID,temp)

######################################################################################################		
######################################################################################################
######################################################################################################              
           
  
            if "mq9" in data:  
	      	      
	      s = time.mktime(datetime.today().timetuple())
              #ID=(re.search(r'\d+',data).group())
	      try:
		
		mq_val= data.split("mq9",1)[1].split(":",1)[1]
		line = int(ID)+9
		
		keyword='{} .mq9   :'.format(ID)
		indx=(data.split(keyword))
		gas_value=int(indx[1].split('\'',2)[0])
		ON_MESSAGE="\033[1;33m ON"
		OFF_MESSAGE="\033[1;38m OFF"
		
		if gas_value < thshld:
		  
		  
		  MESSAGE="{},OFF".format(ID)
		  conn.send(MESSAGE)
		  text="AB"
		  PRNT_MSG=OFF_MESSAGE
		  
		if gas_value > thshld+1 :
		  MESSAGE="{},ON".format(ID)
		  conn.send(MESSAGE) 
		  PRNT_MSG=ON_MESSAGE
		print "\033[{}H\033[1;31m ID: {}, MQ9 VALUE: \033[3;34m   >> {} {} \t{},  \033[0;0m".format(line,ID,gas_value,PRNT_MSG,s)  
	      except:
		
		gas_value=""
		
	      
		
######################################################################################################		
######################################################################################################
######################################################################################################
           
  
  
            if "mq2" in data:  
              s1 = time.mktime(datetime.today().timetuple())
              #ID=(re.search(r'\d+',data).group())
	      try:
		
		mq_val= data.split("mq2",1)[1].split(":",1)[1]
		line1 = int(ID)+10
		
		keyword='{} .mq2   :'.format(ID)
		indx=(data.split(keyword))
		gas_value=int(indx[1].split('\'',2)[0])
		ON_MESSAGE="\033[1;33m ON"
		OFF_MESSAGE="\033[1;38m OFF"
		
		if gas_value < thshld:
		  
		  
		  MESSAGE="{},OFF".format(ID)
		  conn.send(MESSAGE)
		  text="AB"
		  PRNT_MSG=OFF_MESSAGE
		  
		if gas_value > thshld+1 :
		  MESSAGE="{},ON".format(ID)
		  conn.send(MESSAGE) 
		  PRNT_MSG=ON_MESSAGE
		  
		print "\033[{}H\033[1;31m ID: {}, MQ2 VALUE: \033[3;34m   >> {} {} \t{}\033[0;0m".format(line1,ID,gas_value,PRNT_MSG,s1)  
	      except:
		
		gas_value=""
		
		
######################################################################################################		
######################################################################################################
######################################################################################################
  
            if "mq5" in data:  
              s2 = time.mktime(datetime.today().timetuple())
              #ID=(re.search(r'\d+',data).group())
	      try:
		
		mq_val= data.split("mq5",1)[1].split(":",1)[1]
		line2 = int(ID)+11
		
		keyword='{} .mq5   :'.format(ID)
		indx=(data.split(keyword))
		gas_value=int(indx[1].split('\'',2)[0])
		ON_MESSAGE="\033[1;33m ON"
		OFF_MESSAGE="\033[1;38m OFF"
		
		if gas_value < thshld:
		  
		  
		  MESSAGE="{},OFF".format(ID)
		  conn.send(MESSAGE)
		  text="AB"
		  PRNT_MSG=OFF_MESSAGE
		  
		if gas_value > thshld+1 :
		  MESSAGE="{},ON".format(ID)
		  conn.send(MESSAGE) 
		  PRNT_MSG=ON_MESSAGE
		  
		print "\033[{}H\033[1;31m ID: {}, MQ5 VALUE: \033[3;34m   >> {} {} \t{} \033[0;0m".format(line2,ID,gas_value,PRNT_MSG,s2)  
	      except:
		
		gas_value=""
		
		
######################################################################################################		
######################################################################################################
######################################################################################################


	  except:
  
	    data = ""
		
	    #print "\033[10H \033[1;38m {} \033[1;33m .................................................... \033[1;38m  \033[0;0m".format(data)
        #time.sleep(100)   

# Multithreaded Python server : TCP Server Socket Program Stub
TCP_IP = '0.0.0.0' 
TCP_PORT = 80 
BUFFER_SIZE = 200  # Usually 1024, but we need quick response 

tcpServer = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
tcpServer.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) 
tcpServer.bind((TCP_IP, TCP_PORT)) 
threads = [] 
 
while True: 
    tcpServer.listen(4) 

    (conn, (ip,port)) = tcpServer.accept() 
    newthread = ClientThread(ip,port) 
    newthread.start() 
    threads.append(newthread) 
 
for t in threads: 
    t.join() 


