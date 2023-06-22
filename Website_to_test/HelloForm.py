#!/usr/bin/env python3
import cgi

# HTML formundan gelen verileri al
form = cgi.FieldStorage()

# Formdaki input alanlarından verileri al
first_name = form.getvalue('fname')
last_name = form.getvalue('lname')
second_name = form.getvalue('second')
third_name = form.getvalue('Third')

# Gelen verileri kullanarak HTML yanıtını oluştur
print("Content-type:text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>Form Response</title>")
print("</head>")
print("<body>")
print("<h2>Merhaba</h2>")
print("<p>First Name: {}</p>".format(first_name))
print("<p>Last Name: {}</p>".format(last_name))
print("<p>Second Name: {}</p>".format(second_name))
print("<p>Third Name: {}</p>".format(third_name))
print("</body>")
print("</html>")