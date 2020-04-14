import socket , string, re,time
from threading import Thread 
from SocketServer import ThreadingMixIn 



# Multithreaded Python server : TCP Server Socket Thread Pool
class ClientThread(Thread): 
 
    def __init__(self,ip,port): 
        Thread.__init__(self) 
        self.ip = ip 
        self.port = port 
        print "\033[1H [+]" + ip + ": \033[0m" + str(port) 
 
    def run(self): 
        while True : 
            #data = conn.recv(1024) 
     	    try :
	      data = conn.recv(2048)
	    except:
	      data = ""
	      
            time.sleep(10)
            #print data
            
            
            
            if "tmp" in data:
              #print "tmp found in ", data
              ID=(re.search(r'\d+',data).group())
              conn.send(ID)
              temp=(re.search(r'[-+]?\d*\.\d+',data).group())
              #print "\033[1;31m................... {} ...................\n \033[0m".format(ID)
              print "\033[3H\033[1;30m 			Temperature \033[3;34m >> {}  \033[0;0m ".format(temp)
              
              
            if "mq9" in data:  
              
              ID=(re.search(r'\d+',data).group())
              #print "\033[1;35m................... {} ...................\n \033[0m".format(ID)
              #print "\033[1;35m {} +++++ \n \033[0m".format(data)
              #print "\033[1;35m...................0000...................\n \033[0m"
              #try:
		 #= re.match('{} .mq9'.format(ID) ,data ).group()
	      try:
		mq_val = data.split("mq9",1)[1].split(":",1)[1]
	        gas_value = int(mq_val)
	      except:
		exit(0)
	      #except:
		#gas_value = 
	      
	      #print " VALUE is {}".format(gas_value)
	      line = int(ID)+4
	      if gas_value < 400:
		  
		print "\033[{}H \033[1;38m {},OFF ==>\033[3;34m	{}\033[0;0m".format(line,ID,mq_val)
		#print "\033[1;38m ID OFF for valu of the ---------- \033[0m",ID,gas_value

		MESSAGE="{},OFF".format(ID)
		conn.send(MESSAGE)
		text="AB"
		#conn.send(text)
		#time.sleep(10)
		#conn.send(text)

	      #print type(gas_value)		  
	      if gas_value > 401 :
		
		MESSAGE="{},ON".format(ID)
		conn.send(MESSAGE) 
		print "\033[{}H \033[1;35m {} \033[1;33m ON \033[2;35m >>\033[3;38m {}  \033[0;0m".format(line,ID , gas_value)
		#text="AB"
		#conn.send(text)
		#time.sleep(10)
		#conn.send(text)

	    #time.sleep(10)
              

            ###MESSAGE = raw_input("Multithreaded Python server : Enter Response from Server/Enter exit:")
            #MESSAGE = "SADEGH"
            #if MESSAGE == 'exit':
                #break
            #conn.send("IOT")  
            
            #conn.send(MESSAGE)
            #conn.send("ON")
        time.sleep(100)   

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
    #print "Multithreaded Python server : Waiting for connections from TCP clients..." 
    (conn, (ip,port)) = tcpServer.accept() 
    newthread = ClientThread(ip,port) 
    newthread.start() 
    threads.append(newthread) 
 
for t in threads: 
    t.join() 
