#!/usr/bin/php-cgi
<?php

// Kullanıcı tarafından gönderilen POST verilerini alın
$uploadedFile = $_FILES['file'];
$uploadPath = '/Users/faozturk/Desktop/webserv/Website_to_test/uploads' . $uploadedFile['name'];

// Dosyayı belirtilen yola kaydedin
if (move_uploaded_file($uploadedFile['tmp_name'], $uploadPath)) {
    echo "Resim basarıyla yuklendi.";
} else {
    echo "Resim yukleme hatasi.";
}