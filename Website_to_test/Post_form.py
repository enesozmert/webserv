#!/usr/bin/python
# -*- coding: utf-8 -*-

# Import modules for CGI handling
import cgi,os ,cgitb

# Create instance of FieldStorage

# Get data crom fields
fname = os.environ['fname']
lname = os.environ['lname']
second = os.environ['second']
Third = os.environ['Third']

print '<!DOCTYPE html>'
print '<html>'
print '<head>'
print '<p> Hello GET - CGI Program</title <p>'
print '</head>'
print '<body>'
print '<h2>Hello %s %s %s %s </h2>' % (fname.replace("+", " "), lname.replace("+", " "),second.replace("+", " "), Third.replace("+", " "))
print '</body>'
print '</html>'
cgitb.enable()

