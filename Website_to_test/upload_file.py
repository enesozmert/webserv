#!/usr/bin/python
# -*- coding: utf-8 -*-

import cgi, cgitb

cgitb.enable()

# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get the uploaded file
file_item = form['file']
# file_item = os.environ['file']

if file_item.filename:
    # Set the path to save the uploaded file
    file_path = 'uploads/' + file_item.filename

    # Save the file to the specified path
    with open(file_path, 'wb') as file:
        file.write(file_item.file.read())

    print('Content-Type: text/html')
    print()
    print('<html>')
    print('<head>')
    print('<title>File Upload</title>')
    print('</head>')
    print('<body>')
    print('<h2>File Upload Successful</h2>')
    print('<p>The file has been uploaded and saved as: {}</p>'.format(file_path))
    print('</body>')
    print('</html>')
else:
    print('Content-Type: text/html')
    print()
    print('<html>')
    print('<head>')
    print('<title>File Upload</title>')
    print('</head>')
    print('<body>')
    print('<h2>Error: No file was uploaded</h2>')
    print('</body>')
    print('</html>')
