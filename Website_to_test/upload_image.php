#!/usr/bin/php-cgi
<?php

if(isset($_FILES['file']))
{
    $uploadedFile = $_FILES['file'];
    $uploadPath = 'uploads/' . $_FILES['file']['name'];

    // Dosyayı belirtilen yola kaydedin
    if (move_uploaded_file($_FILES['file']['tmp_name'], $uploadPath)) {
        echo "Resim basariyla yuklendi.";
    } else {
        echo "Resim yukleme hatasi.";
    }   
}
else{
    echo "file bulunamadi";
}
?>