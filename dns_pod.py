#!/usr/bin/python
#-*- coding:utf-8 -*-

import httplib, urllib
import socket
import time
import logging
import json

params = dict(
    login_email="xxx@xxx.com", # replace with your email
    login_password="xxxxx", # replace with your password
    format="json",
    domain_id=xxxxx, # replace with your domain_od, can get it by API Domain.List
    record_id=xxxxx, # replace with your record_id, can get it by API Record.List
    sub_domain="demo", # replace with your sub_domain
    record_line="默认",
)

def ddns(ip):
    params.update(dict(value=ip))
    headers = {"Content-type": "application/x-www-form-urlencoded", "Accept": "text/json"}
    conn = httplib.HTTPSConnection("dnsapi.cn")
    conn.request("POST", "/Record.Ddns", urllib.urlencode(params), headers)
    
    response = conn.getresponse()
    print response.status, response.reason
    data = response.read()
    print data
    conn.close()
    return response.status,data

def getip():
    sock = socket.create_connection(('ns1.dnspod.net', 6666))
    ip = sock.recv(16)
    sock.close()
    return ip

def main():
    current_ip = "0.0.0.0"
    while True:
        try:
            ip = getip()
            logging.info("get_ip %s"%ip)
            print "xx"
            if current_ip != ip:
                code,data = ddns(ip)
                print code,data
                print type(code)
                if(code == 200):
                    obj = json.loads(data)
                    if(obj['status']['code'] == "1" and ip == obj.get('record',{}).get('value',None)):
                        logging.info(obj['status']['message'])
                        current_ip = ip	
                    else:
                        time.sleep(5)
                        logging.warning("second update")
                        code,data = ddns(ip)
                        obj = json.loads(data)
                        if(code == 200 and obj['status']['code'] == '1' and obj.get('record',{}).get('value',None)):
                            current_ip = ip
                            logging.warning("second update success")
                        else:
                            logging.error("second update error")
                        
                else:
                    logging.error("Failed,code:%s"%code)
        except Exception, e:
            print e
            pass
        time.sleep(30)
if __name__ == '__main__':
	logging.basicConfig(level=logging.DEBUG,
		format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
		datefmt = '%a, %d %b %Y %H:%M:%S',
		filename='dns_pod.log',
		filemode='w')
	main()
