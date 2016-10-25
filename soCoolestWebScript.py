import urllib.request
import urllib.response
import urllib.parse
import http.cookiejar
import http.client
import re

def findSumOfFirstNPrimeNumbers(N):
	sum = 0
	n = 0
	num = 1
	while n < N:
		# prime numbers are greater than 1
		if num > 1:
			for i in range(2,num):
				if (num % i) == 0:
					break
			else:
				sum += num
				n = n+1
		num = num+1
	return sum

conn = http.client.HTTPConnection("hack.bckdr.in")
response = conn.request("GET", 'http://hack.bckdr.in/2013-MISC-75/misc75.php')
response = conn.getresponse()
text = response.read()
print(text)
num_in_page = [int(s) for s in text.split() if s.isdigit()]
number = num_in_page[1]
sum = findSumOfFirstNPrimeNumbers(number)
headers = response.getheaders()
cookie = headers[3][1]
print(cookie)
headers = {"Content-type": "application/x-www-form-urlencoded"
			, "Cookie": cookie
			, "Host": "hack.bckdr.in"
			, "Connection" : "keep-alive"
			, "Cache-Control" : "max-age=0"
			, "Origin": "http://hack.bckdr.in"
			, "Referer": "http://hack.bckdr.in/2013-MISC-75/misc75.php"
			, "User-Agent": "Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.118 Safari/537.36"}
params = urllib.parse.urlencode({'answer': sum, 'submit': 'Submit'})
response = conn.request("POST", 'http://hack.bckdr.in/2013-MISC-75/misc75.php', params, headers)
response = conn.getresponse()
text = response.read()
print(text)
#NAPRIMER