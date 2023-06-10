<?php
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $targetDir = 'uploads/';
    $fileName = $_FILES['file']['name'];
    $targetFile = $targetDir . $fileName;

    echo "Uploaded File Name: " . $fileName . "<br>";
    
    // Dosyayı belirtilen dizine kaydet
    if (move_uploaded_file($_FILES['file']['tmp_name'], $targetFile)) {
        echo "Dosya basarıyla yuklendi";
    } else {
        echo "Dosya yukleme hatasi.";
    }
}
?>
