#!/usr/bin/php-cgi
<?php

// Kullanıcı tarafından gönderilen POST verilerini alın
if(isset($_FILES['file']))
{
    $uploadedFile = $_FILES['file'];
    $uploadPath = 'uploads/' . $_FILES['file']['name'];

    // Dosyayı belirtilen yola kaydedin
    if (move_uploaded_file($_FILES['file']['tmp_name'], $uploadPath)) {
        echo "Resim basarıyla yuklendi.";
    } else {
        echo "Resim yukleme hatasi.";
    }   
}
else{
    echo "Yok";
}
?>
<!DOCTYPE html>
<html lang="tr">
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Document</title>
</head>
<body>
<form action="#" method="post" enctype="multipart/form-data">
      <label for="file">Resim secin:</label>
      <input type="file" id="file" name="file"><br><br>
      <input type="submit" value="Yukle">
    </form>
</body>
</html>